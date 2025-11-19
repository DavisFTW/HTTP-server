//
// Created by Davis on 10/29/2025.
//
#include "server.h"
#include "consoleManager.h"
server::server(const std::filesystem::path& projectPath) {
    this->projectPath = projectPath;
}
int server::init() {
    if (!this->wsa.getWSAStatus()) {
        return -1;
    }

    auto raw = socket(AF_INET, SOCK_STREAM, 0);
    this->sock = socketRAII(raw);

    if (this->sock.get() == INVALID_SOCKET) {
        LOG.color(Color::RED)("Socket creation failed");
        return -1;
    }

    this->local_sin.sin_family = AF_INET;
    this->local_sin.sin_port = htons (this->port);
    this->local_sin.sin_addr.s_addr = htonl (INADDR_ANY);
    const auto bindErr = bind(this->sock.get(), reinterpret_cast<struct sockaddr * >(&local_sin), sizeof(local_sin));

    if (bindErr == SOCKET_ERROR) {
        const auto errCOde = WSAGetLastError();
        LOG.color(Color::RED)("Bind failed exiting with", errCOde, "error code");
        return -2;
    }

    const auto listenErrr = listen(this->sock.get(), this->backlog);
    if (listenErrr == SOCKET_ERROR) {
        LOG.color(Color::RED)("Listen failed with error", listenErrr);
        return -3;
    }
    return 1;
}

int server::start() {
    FD_ZERO(&this->master_set);
    FD_SET(this->sock.get(), &this->master_set);
    this->max_fd = this->sock.get();

    while (true) {
        fd_set copySet = this->master_set;
        int activity = select(max_fd + 1, &copySet, NULL, NULL, NULL);

        if (activity == SOCKET_ERROR) {
            const auto err = WSAGetLastError();
            LOG.color(Color::RED)("Select failed", err, "error code");
            return -1;
        }

        if (FD_ISSET(this->sock.get(), &copySet)) {
            struct sockaddr_in connecting_sin{};
            int addr_size = sizeof(connecting_sin);

            const auto rawclientSocket = accept(
                this->sock.get(),
                reinterpret_cast<struct sockaddr*>(&connecting_sin),
                &addr_size);

            if (rawclientSocket != INVALID_SOCKET) {
                FD_SET(rawclientSocket, &this->master_set);

                if (rawclientSocket > this->max_fd) {
                    this->max_fd = rawclientSocket;
                }
                this->clients.emplace(rawclientSocket, socketRAII(rawclientSocket));
                LOG.color(Color::GREEN)("New client connected, socket:", rawclientSocket);
            }
        }

        // iterate existing clients
        for (auto it = clients.begin(); it != clients.end(); ) {
            SOCKET clientSock = it->first;
            socketRAII& clientRAII = it->second;

            // is client ready
            if (FD_ISSET(clientSock, &copySet)) {
                char bufferRead[4096];
                const auto bytesRead = recv(clientRAII.get(), bufferRead, 4096, 0);

                if (bytesRead <= 0) {
                    // Client disconnected
                    LOG.color(Color::YELLOW)("Client disconnected, socket:", clientSock);
                    FD_CLR(clientSock, &this->master_set);
                    it = clients.erase(it);
                    continue;
                }

                // Process request
                std::string request(bufferRead, bytesRead);
                size_t firstLineEnd = request.find("\r\n");
                std::string requestLine = request.substr(0, firstLineEnd);

                parser parserInstance;
                const auto contents = parserInstance.parseFile(requestLine, this->projectPath);

                int bodyLength = contents.length();
                LOG.color(Color::GREEN)("Sending data to browser from server!");

                std::string response = this->buildHttpResponse(bodyLength);
                response += contents;

                const auto bytesSent = send(clientRAII.get(),
                                           response.c_str(),
                                           response.length(),
                                           0);

                if (bytesSent == SOCKET_ERROR) {
                    LOG.color(Color::RED)("Send failed");
                }

                // Close connection after response http1.0 style
                FD_CLR(clientSock, &this->master_set);
                it = clients.erase(it);
                continue;
            }

            ++it;  // client not ready
        }
    }
    return 0;
}

std::string server::buildHttpResponse(int bodyLength)  {
    std::string response = "HTTP/1.1 200 OK\r\n";
    response += "Content-Type: text/html\r\n";
    response += "Content-Length: " + std::to_string(bodyLength) + "\r\n";
    response += "Connection: close\r\n";
    response += "\r\n";
    return response;
}
