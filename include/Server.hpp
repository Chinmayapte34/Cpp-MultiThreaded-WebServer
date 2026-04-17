#ifndef SERVER_HPP
#define SERVER_HPP

#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <string>
#include "ThreadPool.hpp"

class Server {
public:
    Server(int port);
    ~Server();
    void start();

private:
    int port;
    int server_fd;
    struct sockaddr_in address;
    void handleRequest(int client_socket);
    
    // Create a pool with, for example, 4 worker threads
    ThreadPool pool{4}; 
};

#endif