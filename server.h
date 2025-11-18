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

    class socketRAII {
    private:
        SOCKET sock;
    public:
        socketRAII() : sock(INVALID_SOCKET) {}

        explicit socketRAII(SOCKET sock) : sock(sock) {}
        ~socketRAII() {
            if (this->sock != INVALID_SOCKET) {
                closesocket(this->sock);
            }
        }

        // copy constructor
        socketRAII(const socketRAII&) = delete;
        //copy operator
        socketRAII& operator=(const socketRAII&) = delete;

        // move constructor
        socketRAII(socketRAII&& other) noexcept : sock(other.sock) {
            other.sock = INVALID_SOCKET; // prevents destructor
        }
        // mnove assigment
        socketRAII& operator=(socketRAII&& other) noexcept {
            // no self assingnes
            if (this != &other) {
                // Close MY current socket (if I have one)
                if (this->sock != INVALID_SOCKET) {
                    closesocket(this->sock);
                }
                this->sock = other.sock;

                // Set other's socket to INVALID_SOCKET so its destructor won't close it
                other.sock = INVALID_SOCKET;
            }
            return *this;
        };

        SOCKET get() const { return this->sock; };
        bool validate() const;
    };


class wsaRAII {
private:
    int errCode;
    const WORD wVersionRequested = MAKEWORD(2, 2);
    WSADATA wsaData;
    bool wsaStatus = false;

public:
    wsaRAII();
    ~wsaRAII();

    bool getWSAStatus() const { return wsaStatus; };
};


    class server {
    private:

        // init classes
        wsaRAII wsa;
        socketRAII sock; // always init after wsa as socket is depentdant on this

        const int port = 5051;
        struct sockaddr_in  local_sin{};

        const int backlog = 5;

        std::filesystem::path projectPath;

        std::string buildHttpResponse(int bodyLength);

    public:

        server(const std::filesystem::path& projectPath);
        int init();
        int start();

    };
    #endif //UNTITLED_SERVER_H
