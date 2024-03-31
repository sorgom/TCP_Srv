#include <TCP_Srv/TCP_Srv_Base.h>

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
using std::cout;
using std::cerr;
using std::endl;
#include <iomanip>
using std::setw;

//  tracing
#ifdef TRACE_ON
//  general trace
#define TRACE(MSG) cout << MSG << endl;
//  mutex locked trace with thread number
#define TRACE_TM(MSG) { mutexlock lock(mMtxOut); TRACE(setw(3) << nr << ' ' << MSG) }
#else
#define TRACE(MSG)
#define TRACE_TM(MSG)
#endif


void TCP_Srv_Base::tm(SOCKET clientSocket, const UINT32 nr)
{
    bool cont = true;
    TRACE_TM("CON")

    while(cont)
    {
        fd_set cset;
        FD_ZERO(&cset);
        FD_SET(clientSocket, &cset);
        tval tv { tmSec, tmMic };
        if (select(0, &cset, nullptr, nullptr, &tv) < 0)
        {
            TRACE_TM("ERR select")
            cont = false;
        }
        else if (FD_ISSET(clientSocket, &cset))
        {
            Buffer buff;
            size_t size = recv(clientSocket, buff, sizeof(Buffer), 0);
            if (size > 0)
            {
                TRACE_TM("<- " << size)
                process(clientSocket, buff, size, nr);
            }
            else
            {
                TRACE_TM("EX")
                cont = false;
            }
        }
    }
    closesocket(clientSocket);
    endOfThread();
}

void TCP_Srv_Base::run(const INT32 argc, const CONST_C_STRING* const argv)
{
    if (argc > 1)
    {
        run(static_cast<UINT16>(atoi(argv[1])));
    }
    else
    {
        run();
    }
}

//  trace error and exit loop
#define TRACE_ERR_X(MSG) { cerr << endl << MSG << endl; cont = false; }

void TCP_Srv_Base::run(const UINT16 port)
{
    std::cout << "..." << endl;
    bool cont = true;
    SOCKET listenSocket = INVALID_SOCKET;

#ifdef _WIN32
    // check for Windows Sockets version 2.2
    {
        WSADATA wsaData;
        if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
        TRACE_ERR_X("WSAStartup failed")
    }
#endif
    //  create socket
    if (cont)
    {
        listenSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
        if (listenSocket < 0) TRACE_ERR_X("socket failed")
    }
    //  bind socket to port
    if (cont)
    {
        sockaddr_in addr;
        addr.sin_family = AF_INET;
        addr.sin_addr.s_addr = INADDR_ANY;
        addr.sin_port = htons(port);

        if (bind(listenSocket, (const sockaddr*)&addr, sizeof(addr)) < 0)
        TRACE_ERR_X("bind failed: port " << port)
    }
    //  listen to socket
    if (cont and (listen(listenSocket, SOMAXCONN) < 0))
    TRACE_ERR_X("listen failed")

    //  select and accept loop
    TRACE("running on port " << port << endl << "press Ctrl+C to stop")
    while (cont)
    {
        //  select
        fd_set lset;
        FD_ZERO(&lset);
        FD_SET(listenSocket, &lset);
        tval tv { tmSec, tmMic };
        if (select(0, &lset, nullptr, nullptr, &tv) < 0)
        TRACE_ERR_X("listen select failed")

        //  accept to new client socket if listen socket is set 
        else if (FD_ISSET(listenSocket, &lset))
        {
            SOCKET clientSocket = accept(listenSocket, nullptr, nullptr);
            if (clientSocket < 0) TRACE_ERR_X("accept failed")
            //  start thread with client socket
            else
            {
                startThread(clientSocket);
            }
        }
    }
    //  should not be reached:
    //  clean up in case of error
    if (listenSocket != INVALID_SOCKET)
    {
        closesocket(listenSocket);
    }
#ifdef _WIN32
    WSACleanup();
#endif
}

void TCP_Srv_Base::startThread(SOCKET clientSocket)
{
    mutexlock lock(mMtxStat);
    ++mCnt;
    ++mNum;
    std::thread(&TCP_Srv_Base::tm, this, clientSocket, mNum).detach();
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
    }
}