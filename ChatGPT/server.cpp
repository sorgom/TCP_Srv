#include "server.h"

#ifdef _WIN32
//  required lib for Winsock
#pragma comment(lib, "ws2_32")
#else
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#define closesocket close
#endif

using std::cerr, std::cout, std::endl;

TCPServer::TCPServer(uint16_t port) : port(port), running(false) {}

TCPServer::~TCPServer() {
    Stop();
}

void TCPServer::Start() {
    running = true;

#ifdef _WIN32
    // check for Windows Sockets version 2.2
    {
        WSADATA wsaData;
        if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
        {
            cerr << "ERR WSAStartup" << endl;
            return;
        }
    }
#endif

    serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (serverSocket == INVALID_SOCKET) {
        cerr << "Error creating socket" << endl;
        return;
    }

    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(port);

    if (bind(serverSocket, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) < 0) {
        cerr << "Bind failed" << endl;
        return;
    }

    if (listen(serverSocket, 5) < 0) {
        cerr << "Listen failed" << endl;
        return;
    }

    std::thread listenerThread(&TCPServer::Listen, this);
    listenerThread.detach();
}

void TCPServer::Stop() {
    running = false;
    closesocket(serverSocket);

#ifdef _WIN32
    WSACleanup();
#endif
    for (auto& thread : clientThreads) {
        thread.join();
    }
    clientThreads.clear();
}

void TCPServer::Listen() {
    while (running) {
        SOCKET clientSocket = accept(serverSocket, nullptr, nullptr);
        if (clientSocket < 0) {
            if (running) cerr << "Accept failed" << endl;
            continue;
        }

        std::thread clientThread(&TCPServer::HandleClient, this, clientSocket);
        clientThreads.push_back(std::move(clientThread));
    }
}

void TCPServer::HandleClient(SOCKET clientSocket) {
    // Handle client communication here
    // For demonstration, just echo received messages back to the client
    char buffer[1024];
    int bytesRead;
    while ((bytesRead = recv(clientSocket, buffer, sizeof(buffer), 0)) > 0) {
        send(clientSocket, buffer, bytesRead, 0);
    }

    closesocket(clientSocket);
}

