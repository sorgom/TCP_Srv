#include "TCP_Srv_Base.h"
#include "Trace.h"

#ifdef _WIN32
//  timevel can be used const in VS
using tval = const timeval;
//  required lib for Winsock
#pragma comment(lib, "ws2_32")
#else
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#define closesocket close
constexpr SOCKET INVALID_SOCKET = -1;
//  timevel must be non const gcc
using tval = timeval;
#endif

#include <thread>
#include <stdlib.h> // atoi

#include <iostream>
using std::cout, std::cerr, std::endl, std::flush;
#include <iomanip>
using std::setw;

#include <regex>
using std::regex, std::regex_match;

#include <filesystem>
using std::filesystem::path;

void TCP_Srv_Base::run(const INT32 argc, const CONST_C_STRING* const argv)
{
    if (argc > 1)
    {
        bool cont = true;
        //  check for help: exit if called
        regex rxHelp {"^-[hH]$"};
        for (INT32 n = 1; n < argc; ++n)
        {
            if (regex_match(argv[n], rxHelp))
            {
                help(path(argv[0]).filename().string());
                cont = false;
                break;
            }
        }
        //  no help call in args - check for port or other args
        if (cont)
        {
            UINT16 port = defPort;
            regex rxPort {"^\\d{2,5}$"};
            for (INT32 n = 1; n < argc; ++n)
            {
                if (regex_match(argv[n], rxPort))
                {
                    port = static_cast<UINT16>(atoi(argv[n]));
                }
                else if (not handlearg(argv[n]))
                {
                    cont = false;
                    break;
                }
            }
            if (cont)
            {
                run(port);
            }
        }
    }
    else
    {
        run();
    }
}

void TCP_Srv_Base::run(const UINT16 port)
{
    std::cout << "..." << endl;
    Trace()
        << "timeout:" << setw(6) << SELECT_MILLI_SECONDS << " ms" << endl
        << "buffer :" << setw(6) << buffSize << " bytes" << endl;

    //  indicator for continuation
    bool cont = true;
    //  listen socket
    SOCKET listenSocket = INVALID_SOCKET;

#ifdef _WIN32
    // check for Windows Sockets version 2.2
    {
        WSADATA wsaData;
        if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
        {
            cerr << "ERR WSAStartup" << endl;
            cont = false;
        }
    }
#endif
    //  create socket
    if (cont)
    {
        listenSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
        if (listenSocket < 0) 
        {
            cerr << "ERR socket" << endl;
            cont = false;
        }
    }
    //  bind socket to port
    if (cont)
    {
        sockaddr_in addr;
        addr.sin_family = AF_INET;
        addr.sin_addr.s_addr = INADDR_ANY;
        addr.sin_port = htons(port);

        if (bind(listenSocket, (const sockaddr*)&addr, sizeof(addr)) < 0)
        {
            cerr << "ERR bind: port " << port << endl;
            cont = false;
        }
    }
    //  listen to socket
    if (cont and (listen(listenSocket, SOMAXCONN) < 0))
    {
        cerr << "ERR listen" << endl;
        cont = false;
    }

    //  display port if successful sofar
    if (cont)
    {
        cout << "port   :" << setw(6) << port << endl;
    }

    //  select and accept loop
    while (cont)
    {
        //  select
        fd_set lset;
        FD_ZERO(&lset);
        FD_SET(listenSocket, &lset);
        tval tv { tmSec, tmMic };

        if (select(0, &lset, nullptr, nullptr, &tv) < 0)
        {
            cerr << "ERR listen select" << endl;
            cont = false;
        }

        //  accept to new client socket if listen socket is set 
        else if (FD_ISSET(listenSocket, &lset))
        {
            SOCKET clientSocket = accept(listenSocket, nullptr, nullptr);
            if (clientSocket < 0) 
            {
                cerr << "ERR accept" << endl;
                cont = false;
            }
            //  start thread with client socket
            else
            {
                startThread(clientSocket);
            }
        }
    }
    //  only reached in case of error: clean up
    if (listenSocket >= 0)
    {
        closesocket(listenSocket);
    }
#ifdef _WIN32
    WSACleanup();
#endif
}

void TCP_Srv_Base::tm(SOCKET clientSocket, const UINT32 nr)
{
    {
        TraceLock(nr) << "CON" << endl;
    }

    //  indicator for continuation
    bool cont = true;

    //  select and receive loop
    while(cont)
    {
        //  select
        fd_set cset;
        FD_ZERO(&cset);
        FD_SET(clientSocket, &cset);
        tval tv { tmSec, tmMic };

        if (select(0, &cset, nullptr, nullptr, &tv) < 0)
        {
            TraceLock(nr) << "ERR select" << endl;
            cont = false;
        }
        //  receive from client socket when select indicates read possible
        else if (FD_ISSET(clientSocket, &cset))
        {
            Buffer buff;
            size_t size = recv(clientSocket, buff, sizeof(Buffer), 0);
            
            //  if 1st recv delivers bytes client ist sending
            if (size > 0)
            {
                //  continue recv until no more bytes are sent
                do {
                    TraceLock(nr) << "<- " << size << endl;
                    process(clientSocket, buff, size, nr);
                    size = recv(clientSocket, buff, sizeof(Buffer), 0);
                } while (size > 0);
            }
            
            //  otherwise client has closed connection
            else
            {
                TraceLock(nr) << "EX" << endl;
                cont = false;
            }
        }
    }
    closesocket(clientSocket);
    endOfThread();
}

void TCP_Srv_Base::startThread(SOCKET clientSocket)
{
    mutexlock lock(mMtxStat);
    ++mCnt;
    ++mNum;
    std::thread(&TCP_Srv_Base::tm, this, clientSocket, mNum).detach();
    displayThreads();
}

void TCP_Srv_Base::endOfThread()
{
    mutexlock lock(mMtxStat);
    if (mCnt > 0) 
    {
        --mCnt; 
        if (mCnt == 0) 
        {
            mNum = 0;
            Trace() << "--- no clients ---" << endl;
        }
        displayThreads();
    }
}

void TCP_Srv_Base::displayThreads() const
{
    if constexpr (not Trace::verbose)
    {
        cout << "threads:" << setw(6) << mCnt << '\r' << flush;
    }
}

void TCP_Srv_Base::help(const std::string&& argv0) const
{
    cout 
        << endl
        << "usage : " << argv0 << " [-h] [port]";
    addusage(cout);
    cout << endl
        << "-h    : this help" << endl
        << "port  : 2-5 digits, default: " << defPort << endl;
    addhelp(cout);
}
