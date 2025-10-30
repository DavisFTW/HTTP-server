//
// Created by Davis on 10/29/2025.
//

#ifndef UNTITLED_SERVER_H
#define UNTITLED_SERVER_H

#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <sstream>

class server {
private:
    const WORD wVersionRequested = MAKEWORD(2, 2);

    const int port = 5051;
    WSADATA wsaData;
    SOCKET sock;
    struct sockaddr_in  local_sin{};

    const int backlog = 5;



public:
    int init();
    int start();
    int cleanup();

};


#endif //UNTITLED_SERVER_H
