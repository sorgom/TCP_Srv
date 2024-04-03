#include "TCP_Srv_Base.h"

#ifdef _WIN32
//  timevel can be used const in VS
using tval = const timeval;
//  required lib for Winsock
#pragma comment(lib, "ws2_32")
#else
#include <arpa/inet.h>
#include <unistd.h>
#define closesocket close
constexpr SOCKET INVALID_SOCKET = -1;
//  timevel must be non const gcc
using tval = timeval;
#endif

#include <thread>
#include <ciso646>
#include <stdlib.h> // atoi

#include <iostream>
using std::cout, std::cerr, std::endl, std::flush;
#include <iomanip>
using std::setw;

#include <regex>
using std::regex, std::regex_match;

#include <filesystem>
using std::filesystem::path;

//  general trace macro
#ifdef VERBOSE
#define TRACE(MSG) cout << MSG << endl;
#else
#define TRACE(MSG)
#endif

void TCP_Srv_Base::run(const INT32 argc, const CONST_C_STRING* const argv)
{
    if (argc > 1)
    {
        //  check for help: exit if called
        regex rxHelp {"^-[hH]$"};
        for (INT32 n = 1; n < argc; ++n)
        {
            if (regex_match(argv[n], rxHelp))
            {
                help(path(argv[0]).filename().string());
                return;
            }
        }
        //  no help call in args - check for port or other args
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
                return;
            }
        }
        run(port);
    }
    else
    {
        run();
    }
}

void TCP_Srv_Base::run(const UINT16 port)
{
    std::cout << "..." << endl;
    TRACE("timeout:" << setw(6) << SELECT_MILLI_SECONDS << " ms")
    TRACE("buffer :" << setw(6) << buffSize << " bytes")

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
    //  locally used mutex locked trace macro with thread number
    #ifdef VERBOSE
    #define TRACE_TM(MSG) { mutexlock lock(mMtxOut); TRACE(setw(3) << nr << ' ' << MSG) }
    #else
    #define TRACE_TM(MSG)
    #endif

    
    TRACE_TM("CON")

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
            TRACE_TM("ERR select")
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
                    TRACE_TM("<- " << size)
                    process(clientSocket, buff, size, nr);
                    size = recv(clientSocket, buff, sizeof(Buffer), 0);
                } while (size > 0);
            }
            
            //  otherwise client has closed connection
            else
            {
                TRACE_TM("EX")
                cont = false;
            }
        }
    }
    closesocket(clientSocket);
    endOfThread();

    //  end of locally used trace macro
    #undef TRACE_TM
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
            TRACE("--- no clients ---")
        }
        displayThreads();
    }
}

void TCP_Srv_Base::displayThreads() const
{
#ifndef VERBOSE
    cout << "threads:" << setw(6) << mCnt << '\r' << flush;
#endif
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
