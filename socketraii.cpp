//
// Created by Davis on 10/31/2025.
//

#include "server.h"

bool socketRAII::validate() const {
       if (this->sock == INVALID_SOCKET) {
              return false;
       }
       return true;
}
