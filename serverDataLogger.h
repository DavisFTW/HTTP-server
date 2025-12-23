//
// Created by Davis on 11/19/2025.
//

#ifndef UNTITLED_SERVERDATALOGGER_H
#define UNTITLED_SERVERDATALOGGER_H

#include <windows.h>
#include <string>
#include <chrono>
#include <map>
struct ConnectionInfo {
   SOCKET socket_fd;
   std::string client_ip;
   uint16_t client_port;
   std::chrono::system_clock::time_point connected_at;
   std::chrono::system_clock::time_point last_activity;
};

struct RequestInfo {
   std::string method;          // GET, POST, etc.
   std::string path;            // /index.html
   std::string http_version;    // HTTP/1.1
   int status_code;             // 200, 404, 500
   size_t bytes_sent;
   std::chrono::milliseconds response_time;
   std::chrono::system_clock::time_point timestamp;
   std::string client_ip;
};

struct ServerStats {
   uint64_t total_connections;
   uint64_t total_requests;
   uint64_t total_bytes_sent;
   uint64_t error_count;
   std::chrono::system_clock::time_point server_start_time;

   // Current state
   size_t active_connections;

   // Per-status code counts
   std::map<int, uint64_t> status_code_counts;  // 200: 1543, 404: 23, etc.
};

class serverDataLogger {
private:
public:

   void startTimer();
   void stopTimer();

};


#endif //UNTITLED_SERVERDATALOGGER_H