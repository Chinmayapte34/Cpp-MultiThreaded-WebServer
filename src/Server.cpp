#include "Server.hpp"
#include <cstring>
#include <thread>
#include "ThreadPool.hpp"
#include <vector>
#include <fstream>
#include <sstream>
#include <unistd.h>

Server::Server(int port) : port(port) {
    // 1. Create socket
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    // 2. Set socket options (Allow immediate reuse of the port)
    int opt = 1;
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    // 3. Define address
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(port);

    // 4. Bind
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("Bind failed");
        exit(EXIT_FAILURE);
    }
}

Server::~Server() {
    close(server_fd);
}

void Server::handleRequest(int client_socket) {
    char buffer[1024] = {0};
    read(client_socket, buffer, sizeof(buffer) - 1);

    // Parse the path (e.g., "GET /about.html HTTP/1.1")
    std::istringstream iss(buffer);
    std::string method, path, protocol;
    iss >> method >> path >> protocol;

    // 1. Map the URL path to our local 'public' directory
    std::string filename = "public" + path;
    if (path == "/") filename = "public/index.html";

    // 2. Try to open the file from disk
    std::ifstream file(filename);
    std::string response;

    if (file.is_open()) {
        std::stringstream content;
        content << file.rdbuf();
        
        // --- NEW LOGIC FOR CONTENT TYPE ---
        std::string contentType = "text/html"; // Default
        if (filename.find(".css") != std::string::npos) {
            contentType = "text/css";
        }
        // ----------------------------------

        response = "HTTP/1.1 200 OK\r\n"
                   "Content-Type: " + contentType + "\r\n"
                   "Content-Length: " + std::to_string(content.str().length()) + "\r\n"
                   "Connection: close\r\n\r\n" +
                   content.str();
    } else {
        // 3. If file doesn't exist, send 404
        std::string errorHTML = "<html><body><h1>404 Not Found</h1><p>The file you requested does not exist.</p></body></html>";
        response = "HTTP/1.1 404 Not Found\r\n"
                   "Content-Type: text/html\r\n"
                   "Content-Length: " + std::to_string(errorHTML.length()) + "\r\n"
                   "Connection: close\r\n\r\n" +
                   errorHTML;
    }

    // 4. Send the constructed response back to the browser
    send(client_socket, response.c_str(), response.length(), 0);
    close(client_socket);
}

void Server::start() {
    if (listen(server_fd, 10) < 0) {
        perror("Listen failed");
        exit(EXIT_FAILURE);
    }

    std::cout << "Server listening on port " << port << " (Multi-threaded)..." << std::endl;

    while (true) {
    int addrlen = sizeof(address);
    int client_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen);
    
    if (client_socket < 0) {
        perror("Accept failed");
        continue;
    }

    // --- PHASE 3: THREAD POOL INTEGRATION ---
    // Instead of std::thread t(...), we use the pool's enqueue method.
    // We pass a 'lambda' function [this, client_socket] { ... } 
    // which tells the pool worker to run handleRequest for this specific socket.
    pool.enqueue([this, client_socket] {
        this->handleRequest(client_socket);
    });
}
}