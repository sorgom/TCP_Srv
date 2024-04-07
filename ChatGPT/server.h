#ifndef SERVER_H
#define SERVER_H

#include <atomic>
#include <condition_variable>
#include <cstdint>
#include <cstring>
#include <iostream>
#include <mutex>
#include <thread>
#include <vector>

#ifdef _WIN32
#include <winsock2.h>
#else
using SOCKET = int;
constexpr SOCKET INVALID_SOCKET = -1;
#endif


class TCPServer {
public:
    TCPServer(uint16_t port);
    ~TCPServer();

    void Start();
    void Stop();

private:
    void Listen();
    void HandleClient(SOCKET clientSocket);

    const uint16_t port;
    SOCKET serverSocket;
    std::vector<std::thread> clientThreads;
    std::atomic<bool> running = false;
};

#endif // SERVER_H
