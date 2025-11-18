//
// Created by Davis on 11/18/2025.
//


#include "server.h"
#include "consoleManager.h"


wsaRAII::wsaRAII() {

    if (this->errCode = WSAStartup(this->wVersionRequested, &this->wsaData); this->errCode != 0) {
        LOG.color(Color::RED)("WSAStartup failed with error:", this->errCode);
        // wsaStatus by default is set to false no need to do it twice
    }
    else {
        LOG.color(Color::GREEN)("WSAStartup successful");
        this->wsaStatus = true;
    }
}

wsaRAII::~wsaRAII() {
    if (this->wsaStatus) {
        WSACleanup();
    }
}



