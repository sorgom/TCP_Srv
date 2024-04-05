#include "server.h"

int main() {
    // Port number to listen on
    int port = 8080;

    // Create an instance of the server
    TCPServer server(port);

    // Start the server
    server.Start();

    // Wait for user input to stop the server
    std::cout << "Server is running. Press any key to stop.\n";
    std::cin.get();

    // Stop the server
    server.Stop();

    std::cout << "Server stopped.\n";
    return 0;
}
