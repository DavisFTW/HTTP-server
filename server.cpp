//
// Created by Davis on 10/29/2025.
//
#include "server.h"
#include "consoleManager.h"

server::server(const std::filesystem::path& projectPath) {
    this->projectPath = projectPath;
}
int server::init() {

    if (const auto WSAStartupErr = WSAStartup(this->wVersionRequested, &this->wsaData); WSAStartupErr != 0) {
        LOG.color(Color::RED)("WSAStartup failed with error:", WSAStartupErr);
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
    while (true) {
        struct sockaddr_in  connecting_sin{};
        int addr_size = sizeof(connecting_sin);
        const auto rawclientSocket =  accept(this->sock.get(), reinterpret_cast<struct sockaddr*>(&connecting_sin), &addr_size);
        socketRAII clientSocket(rawclientSocket);
        if (clientSocket.get() == INVALID_SOCKET) {
            LOG.color(Color::RED)("Accept failed");
            return -1;
        }

        // receive data
        char bufferRead[4096];
        const auto bytesRead = recv(clientSocket.get(), bufferRead, 4096, 0);
        if (bytesRead == SOCKET_ERROR) {
            LOG.color(Color::RED)("Recv failed");
            return -10;

        }
        else if ( bytesRead == 0 ) {
            LOG.color(Color::YELLOW)("Client disconnected");
            closesocket(clientSocket.get());
            continue;
        }

        // received data = ok send this data to parser, recive back content
        std::string request(bufferRead, bytesRead);

        // Parse request line
        size_t firstLineEnd = request.find("\r\n");
        std::string requestLine = request.substr(0, firstLineEnd);


        parser parserInstance;

        const auto contents = parserInstance.parseFile(requestLine, this->projectPath);

            // build response
            int bodyLength = contents.length();

            // send data
            LOG.color(Color::GREEN)("Sending data to browser from server !");

            std::string response = this->buildHttpResponse(bodyLength);
             response += contents;


            const auto bytesSent = send(clientSocket.get(), response.c_str(), response.length(), 0);
            if (bytesSent == SOCKET_ERROR) {
                LOG.color(Color::RED)("Send failed");
                return -10;
            }
        }
        return 0;
}

int server::cleanup() {
    if (WSACleanup() != 0) {
        LOG.color(Color::RED)("WSACleanup failed");
        return -4;
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
