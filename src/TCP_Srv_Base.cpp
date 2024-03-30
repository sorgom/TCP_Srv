#include <TCP_Srv/TCP_Srv_Base.h>

#define TRACE_ON
#include <TCP_Srv/TraceMacros.h>

#include <thread>
#include <ciso646>

#include <iostream>
using std::cout;
using std::cerr;
using std::endl;

void TCP_Srv_Base::tm(SOCKET cs)
{
    bool cont = true;
    while(cont)
    {
        fd_set cset;
        FD_ZERO(&cset);
        FD_SET(cs, &cset);
        tval tv {SELECT_SECONDS, SELECT_MICRO_SECONDS};
        if (select(0, &cset, nullptr, nullptr, &tv) < 0)
        {
            TRACE("client select failed")
            cont = false;
        }
        else if (FD_ISSET(cs, &cset))
        {
            Buffer buff;
            size_t bytes = recv(cs, buff, READ_BUFFER_SIZE, 0);
            if (bytes > 0)
            {
                TRACE("client sent " << bytes << " bytes")
                process(cs, buff, bytes);
            }
            else
            {
                TRACE("client disconnected")
                cont = false;
            }
        }
    }
    closesocket(cs);
}

bool TCP_Srv_Base::run(const INT32 argc, const CONST_C_STRING* const argv)
{
    if (argc > 1)
    {
        return run(static_cast<UINT16>(atoi(argv[1])));
    }
    else
    {
        return run();
    }
}

bool TCP_Srv_Base::run(const UINT16 port)
{
    TRACE("run ...")
    #ifdef _WIN32
    // check for Windows Sockets version 2.2
    {
        WSADATA wsaData;
        if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
        {
            cerr << "WSAStartup failed" << endl;
            return cleanup;
        }
    }
    #endif
    mListenSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (mListenSocket == INVALID_SOCKET)
    {
        cerr << "socket failed" << endl;
        return cleanup();
    }
    sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(port);

    if (bind(mListenSocket, (const sockaddr*)&addr, sizeof(addr)) < 0)
    {
        cerr << "bind failed: port " << port << endl;
        return cleanup();
    }

    if (listen(mListenSocket, SOMAXCONN) < 0)
    {
        cerr << "listen failed" << endl;
        return cleanup();
    }

    bool cont = true;
    while (cont)
    {
        fd_set lset;
        FD_ZERO(&lset);
        FD_SET(mListenSocket, &lset);
        tval tv {SELECT_SECONDS, SELECT_MICRO_SECONDS};
        if (select(0, &lset, nullptr, nullptr, &tv) < 0)
        {
            cerr << "listen select failed" << endl;
            cont = false;
        }
        else if (FD_ISSET(mListenSocket, &lset))
        {

        }
    }
    

    // for (int i = 0; i < 10; ++i)
    // {
    //     std::thread t(&TCP_Srv_Base::tm, this, i);
    //     t.detach();
    // }
    return true;
}

bool TCP_Srv_Base::cleanup()
{
    if (mListenSocket != INVALID_SOCKET)
    {
        closesocket(mListenSocket);
    }
#ifdef _WIN32
    WSACleanup();
#endif
    return false;
}
