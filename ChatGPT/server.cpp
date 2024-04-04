#include "server.h"

TCPServer::TCPServer(int port) : port(port), running(false) {}

TCPServer::~TCPServer() {
    Stop();
}

void TCPServer::Start() {
    running = true;

    serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket == -1) {
        std::cerr << "Error creating socket\n";
        return;
    }

    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(port);

    if (bind(serverSocket, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) < 0) {
        std::cerr << "Bind failed\n";
        return;
    }

    if (listen(serverSocket, 5) < 0) {
        std::cerr << "Listen failed\n";
        return;
    }

    std::thread listenerThread(&TCPServer::Listen, this);
    listenerThread.detach();
}

void TCPServer::Stop() {
    running = false;
    close(serverSocket);

    for (auto& thread : clientThreads) {
        thread.join();
    }
}

void TCPServer::Listen() {
    while (running) {
        sockaddr_in clientAddr;
        socklen_t clientAddrLen = sizeof(clientAddr);
        int clientSocket = accept(serverSocket, (struct sockaddr *)&clientAddr, &clientAddrLen);
        if (clientSocket < 0) {
            std::cerr << "Accept failed\n";
            continue;
        }

        std::thread clientThread(&TCPServer::HandleClient, this, clientSocket);
        clientThreads.push_back(std::move(clientThread));
    }
}

void TCPServer::HandleClient(int clientSocket) {
    // Handle client communication here
    // For demonstration, just echo received messages back to the client
    char buffer[1024];
    int bytesRead;
    while ((bytesRead = recv(clientSocket, buffer, sizeof(buffer), 0)) > 0) {
        send(clientSocket, buffer, bytesRead, 0);
    }

    close(clientSocket);
}
