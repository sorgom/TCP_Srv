//  ============================================================
//  sample TCP echo server
//  figured out from Tenouk's Winsock2 tutorial
//  ============================================================
//  see: https://www.tenouk.com/Winsock/Winsock2example2.html

//  header content

#ifdef _WIN32
#include <winsock2.h>
#pragma comment(lib, "ws2_32")
using tval = const timeval;
#else
#include <arpa/inet.h>
#include <sys/socket.h>  
#include <unistd.h>
using SOCKET = int;
constexpr SOCKET INVALID_SOCKET = -1;
#define closesocket close
using tval = timeval;
#endif

#include <TCP_Srv/BaseTypes.h>
#include <ciso646>
#include <stdlib.h>

class SampleSrv
{
public:
    inline SampleSrv() = default;
    ~SampleSrv();
    bool init(UINT16 port = 8080);
    inline bool init(const char* port) { return init(static_cast<UINT16>(atoi(port))); }
    bool run();
private:
    SOCKET mListenSocket = INVALID_SOCKET;
    bool cleanup();
};

#include <iostream>
using std::cout;
using std::endl;

bool SampleSrv::init(const UINT16 port)
{
    #ifdef _WIN32
    // check for Windows Sockets version 2.2
    {
        WSADATA wsaData;
        if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
        {
            return false;
        }
    }
    #endif
    mListenSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (mListenSocket == INVALID_SOCKET)
    {
        cout << "socket failed" << endl;
        return cleanup();
    }
    sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(port);

    if (bind(mListenSocket, (const sockaddr*)&addr, sizeof(addr)) < 0)
    {
        cout << "bind failed, port: " << port << endl;
        return cleanup();
    }

    return true;
}

bool SampleSrv::cleanup()
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

bool SampleSrv::run()
{
    cout << "run ..." << endl;
    bool ok = true;
    if (listen(mListenSocket, SOMAXCONN) < 0)
    {
        cout << "listen failed" << endl;
        ok = false;
    }
    while (ok)
    {
        fd_set lset;
        FD_ZERO(&lset);
        FD_SET(mListenSocket, &lset);
        {
            tval tv {0, 100000};
            if (ok and (select(0, &lset, nullptr, nullptr, &tv) < 0))
            {
                cout << "listen select failed" << endl;
                ok = false;
            }
        }
        if (ok and FD_ISSET(mListenSocket, &lset))
        {
            SOCKET clientSocket = accept(mListenSocket, nullptr, nullptr);
            if (clientSocket == INVALID_SOCKET)
            {
                cout << "accept failed" << endl;
                ok = false;
            }
            else
            {
                cout << "client connected" << endl;
                while(clientSocket != INVALID_SOCKET)
                {
                    fd_set cset;
                    FD_ZERO(&cset);
                    FD_SET(clientSocket, &cset);
                    tval tv {0, 100000};
                    if (select(0, &cset, nullptr, nullptr, &tv) < 0)
                    {
                        cout << "client select failed" << endl;
                        ok = false;
                    }
                    else if (FD_ISSET(clientSocket, &cset))
                    {
                        char buffer[1024];
                        int bytes = recv(clientSocket, buffer, sizeof(buffer), 0);
                        if (bytes > 0)
                        {
                            cout << "client sent " << bytes << " bytes" << endl;
                            send(clientSocket, buffer, bytes, 0);
                        }
                        else
                        {
                            cout << "client disconnected" << endl;
                            closesocket(clientSocket);
                            clientSocket = INVALID_SOCKET;
                        }
                    }
                }
            }
        }
    }
    return ok or cleanup();
}

SampleSrv::~SampleSrv()
{
    if (mListenSocket != INVALID_SOCKET)
    {
        closesocket(mListenSocket);
    }
#ifdef _WIN32
    WSACleanup();
#endif
}

int main()
{
    SampleSrv srv;
    if (not srv.init())
    {
        cout << "init failed" << endl;
        return 1;
    }
    if (not srv.run())
    {
        cout << "run failed" << endl;
        return 1;
    }
    cout << "test done." << endl;
    return 0;
}