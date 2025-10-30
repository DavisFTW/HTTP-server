//
// Created by Davis on 10/29/2025.
//
#include <fstream>
#include <filesystem>
#include "server.h"

int server::init() {

    if (const auto WSAStartupErr = WSAStartup(this->wVersionRequested, &this->wsaData); WSAStartupErr != 0) {
        std::cout << "WSAStartup failed with error:" << WSAStartupErr << std::endl;
        return -1;
    }

    this->sock = socket(AF_INET, SOCK_STREAM, 0);
    if (this->sock == INVALID_SOCKET) {
        std::cout << "socket creation failed" << std::endl;
        return -1;
    }

    this->local_sin.sin_family = AF_INET;
    this->local_sin.sin_port = htons (this->port);
    this->local_sin.sin_addr.s_addr = htonl (INADDR_ANY);
    const auto bindErr = bind(sock, reinterpret_cast<struct sockaddr * >(&local_sin), sizeof(local_sin));

    if (bindErr == SOCKET_ERROR) {
        const auto errCOde = WSAGetLastError();
        std::cout << "bind failed exiting with " << errCOde << " error code"<< std::endl;
        return -2;
    }

    const auto listenErrr = listen(sock, this->backlog);
    if (listenErrr == SOCKET_ERROR) {
        std::cout << "listen failed with error " << listenErrr << std::endl;
        return -3;
    }
    return 1;
}

int server::start() {
    while (true) {
        struct sockaddr_in  connecting_sin{};
        int addr_size = sizeof(connecting_sin);
       const auto clientSocket =  accept(sock, (struct sockaddr *) &connecting_sin, &addr_size);
        if (clientSocket == INVALID_SOCKET) {
            std::cout << "accept failed" << std::endl;
            return -1;
        }

        // receive data
        char bufferRead[4096];
        const auto bytesRead = recv(clientSocket, bufferRead, 4096, 0);
        if (bytesRead == SOCKET_ERROR) {
            std::cout << "recv failed" << std::endl;
            return -10;

        }
        else if ( bytesRead == 0 ) {
            std::cout << "client disconneted" << std::endl;
            closesocket(clientSocket);
            continue;
        }

        // received data = ok
        std::string request(bufferRead, bytesRead);
        std::cout << "Received:\n" << request << std::endl;

        // Parse request line
        size_t firstLineEnd = request.find("\r\n");
        std::string requestLine = request.substr(0, firstLineEnd);

        std::istringstream iss(requestLine);
        std::string method, path, version;
        iss >> method >> path >> version;

        std::cout << "path: " << path << std::endl;
        path = path.erase(path.find("/"), 1);

        char *cstr = path.data();
        std::uintmax_t filesize;
        try {
           filesize = std::filesystem::file_size(cstr);
        } catch (std::exception &err) {
            std::cout << err.what() << std::endl;
           // file is not found so what to do ? improve
        }


        std::ifstream file(path);
        std::cout << "opening read file !" << std::endl;
        if (!file.is_open()) {
            std::cout << "file open failed" << std::endl;
            continue;
        }


        std::string contents;
        std::stringstream buffer;
        buffer << file.rdbuf();
        contents = buffer.str();

        file.close();

        std::cout << "File contents:\n" << contents << std::endl;
        std::cout << "Total characters: " << contents.length() << std::endl;

        std::cout << "Method: " << method << std::endl;
        std::cout << "Path: " << path << std::endl;
        std::cout << "Version: " << version << std::endl;

        // build response
        int bodyLength = contents.length();

        // send data
        std::cout << "sending data !" << std::endl;
        std::string response = "HTTP/1.1 200 OK\r\n";
        response += "Content-Type: text/html\r\n";
        response += "Content-Length: " + std::to_string(bodyLength) + "\r\n";
        response += "Connection: close\r\n";
        response += "\r\n";
        response += contents;
        const auto bytesSent = send(clientSocket, response.c_str(), response.length(), 0);
        if (bytesSent == SOCKET_ERROR) {
            std::cout << "send failed" << std::endl;
            return -10;
        }
        closesocket(clientSocket);
    }

    return 0;
}

int server::cleanup() {
    if (closesocket(this->sock) == SOCKET_ERROR) {
        std::cout << "closesocket failed" << std::endl;
        return -6;
    }

    if (WSACleanup() != 0) {
        std::cout << "WSACleanup failed" << std::endl;
        return -4;
    }

    return 0;
}

