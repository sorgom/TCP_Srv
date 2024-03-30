#include <TCP_Srv/TCP_Srv_Base.h>

#define TRACE_ON
#include <TCP_Srv/TraceMacros.h>

#ifdef _WIN32
using tval = const timeval;
#pragma comment(lib, "ws2_32")
#else
#include <arpa/inet.h>
#include <unistd.h>
#define closesocket close
constexpr SOCKET INVALID_SOCKET = -1;
using tval = timeval;
#endif

#include <thread>
#include <ciso646>
#include <stdlib.h> // atoi

#include <iostream>
using std::cerr;
using std::endl;

void TCP_Srv_Base::tm(SOCKET clientSocket)
{
    bool cont = true;
    while(cont)
    {
        fd_set cset;
        FD_ZERO(&cset);
        FD_SET(clientSocket, &cset);
        tval tv = gettval();
        if (select(0, &cset, nullptr, nullptr, &tv) < 0)
        {
            TRACE("client select failed")
            cont = false;
        }
        else if (FD_ISSET(clientSocket, &cset))
        {
            Buffer buff;
            size_t bytes = recv(clientSocket, buff, sizeof(Buffer), 0);
            if (bytes > 0)
            {
                TRACE("client sent " << bytes << " bytes")
                process(clientSocket, buff, bytes);
            }
            else
            {
                TRACE("client disconnected")
                cont = false;
            }
        }
    }
    closesocket(clientSocket);
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

void TCP_Srv_Base::run(const UINT16 port)
{
    TRACE("run ...")
    bool cont = true;
    SOCKET listenSocket = INVALID_SOCKET;

#ifdef _WIN32
    // check for Windows Sockets version 2.2
    {
        WSADATA wsaData;
        if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
        {
            cerr << "WSAStartup failed" << endl;
            cont = false;
        }
    }
#endif
    if (cont)
    {
        listenSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
        if (listenSocket < 0)
        {
            cerr << "socket failed" << endl;
            cont = false;
        }
    }
    if (cont)
    {
        sockaddr_in addr;
        addr.sin_family = AF_INET;
        addr.sin_addr.s_addr = INADDR_ANY;
        addr.sin_port = htons(port);

        if (bind(listenSocket, (const sockaddr*)&addr, sizeof(addr)) < 0)
        {
            cerr << "bind failed: port " << port << endl;
            cont = false;
        }
    }
    if (cont and (listen(listenSocket, SOMAXCONN) < 0))
    {
        cerr << "listen failed" << endl;
        cont = false;
    }

    while (cont)
    {
        fd_set lset;
        FD_ZERO(&lset);
        FD_SET(listenSocket, &lset);
        tval tv = gettval();
        if (select(0, &lset, nullptr, nullptr, &tv) < 0)
        {
            cerr << "listen select failed" << endl;
            cont = false;
        }
        else if (FD_ISSET(listenSocket, &lset))
        {
            SOCKET clientSocket = accept(listenSocket, nullptr, nullptr);
            if (clientSocket < 0)
            {
                cerr << "accept failed" << endl;
                cont = false;
            }
            else
            {
                std::thread t(&TCP_Srv_Base::tm, this, clientSocket);
                t.detach();
            }
        }
    }
    if (listenSocket != INVALID_SOCKET)
    {
        closesocket(listenSocket);
    }
#ifdef _WIN32
    WSACleanup();
#endif
}

