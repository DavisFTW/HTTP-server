#include <iostream>
#include "server.h"
int main() {

    server server1("project_name");
    server1.init();
    server1.start();
    return 0;
}