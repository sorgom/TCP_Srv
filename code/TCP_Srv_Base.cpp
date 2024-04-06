#include "TCP_Srv_Base.h"
#include "Trace.h"

#ifdef _WIN32
//  required lib for Winsock
#pragma comment(lib, "ws2_32")
#else
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#define closesocket close
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

using mutexlock = std::unique_lock<std::mutex>;

void TCP_Srv_Base::run(const INT32 argc, const CONST_C_STRING* const argv)
{
    if (argc > 1)
    {
        UINT16 port = defPort;
        regex rxPort {"^\\d{2,5}$"};
        regex rxHelp {"^-[hH]$"};
        bool cont = true;
        for (INT32 n = 1; cont and n < argc; ++n)
        {
            if (regex_match(argv[n], rxHelp))
            {
                help(path(argv[0]).filename().string());
                cont = false;
            }
        }
        //  no help call in args - check for port or other args
        for (INT32 n = 1; cont and n < argc; ++n)
        {
            if (regex_match(argv[n], rxPort)) port = static_cast<UINT16>(atoi(argv[n]));
            else cont = handleArg(argv[n]);
        }
        if (cont) run(port);
    }
    else run();
}

void TCP_Srv_Base::run(const UINT16 port)
{
    std::cout << "..." << endl;
    Trace()
        << "timeout:" << setw(6) << SELECT_MILLI_SECONDS << " ms" << endl
        << "buffer :" << setw(6) << buffSize << " bytes" << endl;

    //  indicator for continuation
    bool ok = true;
 
#ifdef _WIN32
    // check for Windows Sockets version 2.2
    {
        WSADATA wsaData;
        if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
        {
            cerr << "ERR WSAStartup" << endl;
            ok = false;
        }
    }
#endif
    //  create socket
    if (ok)
    {
        listenSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
        if (listenSocket < 0) 
        {
            cerr << "ERR socket" << endl;
            ok = false;
        }
    }
    //  bind socket to port
    if (ok)
    {
        sockaddr_in addr;
        addr.sin_family = AF_INET;
        addr.sin_addr.s_addr = INADDR_ANY;
        addr.sin_port = htons(port);

        if (bind(listenSocket, (const sockaddr*)&addr, sizeof(addr)) < 0)
        {
            cerr << "ERR bind: port " << port << endl;
            ok = false;
        }
    }
    //  listen to socket
    if (ok and (listen(listenSocket, SOMAXCONN) < 0))
    {
        cerr << "ERR listen" << endl;
        ok = false;
    }

    //  display port if successful sofar
    if (ok) 
    {
        cout << "port   :" << setw(6) << port << endl;
        displayThreads();
    }
    
    //  select and other tasks loop
    while (ok)
    {

    //  select and accept loop
        bool clients = false;
        do 
        {
            clients = false;
            //  select
            fd_set lset;
            FD_ZERO(&lset);
            FD_SET(listenSocket, &lset);
            timeval tv { tmSec, tmMic};

            if (select(listenSocket + 1, &lset, nullptr, nullptr, &tv) < 0)
            {
                cerr << "ERR listen select" << endl;
                ok = false;
            }


            //  accept to new client socket if listen socket is set 
            else if (FD_ISSET(listenSocket, &lset))
            {
                SOCKET clientSocket = accept(listenSocket, nullptr, nullptr);

                if (clientSocket < 0) 
                {
                    cerr << "ERR accept" << endl;
                    ok = false;
                }
                //  start thread with client socket
                else
                {
                    startThread(clientSocket);
                    clients = true;
                }
            }
        } while (clients);

        //  other tasks
        otherTasks();
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

void TCP_Srv_Base::handleClient(const SOCKET clientSocket, const UINT32 nr)
{
    //  brackets required for scope of TraceLock object
    { TraceLock(nr) << "CON" << endl; }

    Buffer buff;
    size_t size = 0;
    while ((size = recv(clientSocket, buff, sizeof(Buffer), 0)) > 0)
    {
        { TraceLock(nr) << "<- " << size << endl; }
        process(clientSocket, buff, size, nr);
    }
    { TraceLock(nr) << (size == 0 ? "EX" : "ERR read") << endl; }
    closesocket(clientSocket);
    endOfThread();
}

void TCP_Srv_Base::startThread(SOCKET clientSocket)
{
    mutexlock lock(mMtxStat);
    ++mCnt;
    ++mNum;
    std::thread(&TCP_Srv_Base::handleClient, this, clientSocket, mNum).detach();
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
    if constexpr (not Trace::isOn)
    {
        cout << "threads:" << setw(6) << mCnt << '\r' << flush;
    }
}

void TCP_Srv_Base::help(const std::string&& argv0) const
{
    cout 
        << endl
        << "usage : " << argv0 << " [-h] [port]";
    addUsage();
    cout << endl
        << "-h    : this help" << endl
        << "port  : 2-5 digits, default: " << defPort << endl;
    addHelp();
}
