//
// Created by Davis on 10/29/2025.
//

#ifndef UNTITLED_SERVER_H
#define UNTITLED_SERVER_H

#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <fstream>
#include <filesystem>

#include "parser.h"

class server {
private:
    const WORD wVersionRequested = MAKEWORD(2, 2);

    const int port = 5051;
    WSADATA wsaData;
    SOCKET sock;
    struct sockaddr_in  local_sin{};

    const int backlog = 5;

    std::filesystem::path projectPath;

    std::string buildHttpResponse(int bodyLength);

public:

    server(const std::filesystem::path& projectPath);
    int init();
    int start();
    int cleanup();

};


#endif //UNTITLED_SERVER_H
