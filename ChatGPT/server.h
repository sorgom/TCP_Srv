#ifndef SERVER_H
#define SERVER_H

#include <iostream>
#include <thread>
#include <vector>
#include <mutex>
#include <condition_variable>
#include <atomic>
#include <cstring>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/socket.h>

class TCPServer {
public:
    TCPServer(int port);
    ~TCPServer();

    void Start();
    void Stop();

private:
    void Listen();
    void HandleClient(int clientSocket);

    int port;
    int serverSocket;
    std::vector<std::thread> clientThreads;
    std::atomic<bool> running;
};

#endif // SERVER_H
