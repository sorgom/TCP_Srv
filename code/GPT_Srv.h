//  ============================================================
//  code mainly from ChatGPT
//  small modifications:
//  - one code for both gcc and msvc
//  - some type changes to avoid warnings
//  - removed unused headers
//  - constness
//  ============================================================

#pragma once
#ifndef GPT_SRV_H
#define GPT_SRV_H

#include <atomic>
#include <cstdint>
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
