//  ============================================================
//  sample TCP echo server
//  figured out from Tenouk's Winsock2 tutorial
//  ============================================================
//  see: https://www.tenouk.com/Winsock/Winsock2example2.html

//  header content

#ifdef _WIN32
#include <winsock2.h>
#pragma comment(lib, "ws2_32")
#endif

#include <TCP_Srv/BaseTypes.h>
#include <ciso646>

class SampleSrv
{
public:
    inline SampleSrv() = default;
    ~SampleSrv();
    bool init(UINT16 port = 8080);
    bool run();
    void stop();
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

    if (bind(mListenSocket, (const sockaddr*)&addr, sizeof(addr)) == SOCKET_ERROR)
    {
        cout << "bind failed" << endl;
        return cleanup();
    }

    return true;
}

bool SampleSrv::cleanup()
{
#ifdef _WIN32
    if (mListenSocket != INVALID_SOCKET)
    {
        closesocket(mListenSocket);
    }
    WSACleanup();
#endif
    return false;
}

bool SampleSrv::run()
{
    cout << "run ..." << endl;
    bool ok = true;
    if (listen(mListenSocket, SOMAXCONN) == SOCKET_ERROR)
    {
        cout << "listen failed" << endl;
        ok = false;
    }
    while (ok)
    {
        fd_set lset;
        FD_ZERO(&lset);
        FD_SET(mListenSocket, &lset);
        const timeval tv {0, 100000};
        if (ok and (select(0, &lset, nullptr, nullptr, &tv) == SOCKET_ERROR))
        {
            cout << "listen select failed" << endl;
            ok = false;
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
                    select(0, &cset, nullptr, nullptr, &tv);
                    if (FD_ISSET(clientSocket, &cset))
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

//     struct addrinfo *result = nullptr;
//     struct addrinfo hints;

//     iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);

//     ZeroMemory(&hints, sizeof(hints));
//     hints.ai_family = AF_INET;
//     hints.ai_socktype = SOCK_STREAM;
//     hints.ai_protocol = IPPROTO_TCP;
//     hints.ai_flags = AI_PASSIVE;

//     iResult = getaddrinfo(nullptr, "8080", &hints, &result);
//     if (iResult != 0)
//     {
//         WSACleanup();
//         return false;
//     }

//     mListenSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
//     if (mListenSocket == INVALID_SOCKET)
//     {
//         freeaddrinfo(result);
//         WSACleanup();
//         return false;
//     }

//     iResult = bind(mListenSocket, result->ai_addr, (int)result->ai_addrlen);
//     if (iResult == SOCKET_ERROR)
//     {
//         freeaddrinfo(result);
//         closesocket(mListenSocket);
//         WSACleanup();
//         return false;
//     }

//     freeaddrinfo(result);

//     iResult = listen(mListenSocket, SOMAXCONN);
//     if (iResult == SOCKET_ERROR)
//     {
//         closesocket(mListenSocket);
//         WSACleanup();
//         return false;
//     }

//     return true;
// }

SampleSrv::~SampleSrv()
{
    if (mListenSocket != INVALID_SOCKET)
    {
        closesocket(mListenSocket);
    }
    WSACleanup();
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