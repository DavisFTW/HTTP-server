//
// Created by Davis on 10/31/2025.
//

#ifndef UNTITLED_CONSOLEMANAGER_H
#define UNTITLED_CONSOLEMANAGER_H

// logger.h

// THIS WAS FULLY DONE BY AI, no need to waste time on this, I only modified this a tad bit and cleaned it up
#pragma once
#include <iostream>
#include <sstream>
#include <string>
#include <chrono>
#include <iomanip>

namespace Color {
    const std::string RESET = "\033[0m";
    const std::string RED = "\033[31m";
    const std::string GREEN = "\033[32m";
    const std::string YELLOW = "\033[33m";
    const std::string BLUE = "\033[34m";
    const std::string MAGENTA = "\033[35m";
    const std::string CYAN = "\033[36m";
    const std::string WHITE = "\033[37m";
    const std::string GRAY = "\033[90m";
}


class consoleManager {
private:
    std::string current_color;
    bool include_timestamp;

    std::string getTimestamp();
    void printPrefix();
    void logRecursive() { std::cout << Color::RESET << std::endl; }

    template<typename T, typename... Args>
    void logRecursive(const T& first, const Args&... rest) {
        std::cout << first;
        if constexpr (sizeof...(rest) > 0) {
            std::cout << " ";
            logRecursive(rest...);
        } else {
            logRecursive();
        }
    }

public:
    consoleManager() : current_color(Color::WHITE), include_timestamp(true) {}
    void testLogger();
    consoleManager& color(const std::string& c) {
        current_color = c;
        return *this;
    }

    consoleManager& noTimestamp() {
        include_timestamp = false;
        return *this;
    }

    template<typename... Args>
    void operator()(const Args&... args) {
        printPrefix();
        std::cout << current_color;
        logRecursive(args...);
        current_color = Color::WHITE;
        include_timestamp = true;
    }
};

inline consoleManager LOG;

// ============================================================================
// TEST FUNCTION WITH ALL POSSIBILITIES
// ============================================================================


#endif //UNTITLED_CONSOLEMANAGER_H