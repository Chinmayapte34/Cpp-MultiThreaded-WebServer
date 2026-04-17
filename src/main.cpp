#include "Server.hpp"

int main() {
    Server myServer(8080);
    myServer.start();
    return 0;
}