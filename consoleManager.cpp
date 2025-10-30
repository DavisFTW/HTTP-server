//
// Created by Davis on 10/31/2025.
//

#include "consoleManager.h"

void consoleManager::testLogger() {
    std::cout << "\n=== BASIC LOGGING ===\n";
    LOG("Simple text message");
    LOG("Another line of text");

    std::cout << "\n=== LOGGING VARIABLES ===\n";
    int number = 42;
    double pi = 3.14159;
    std::string name = "TestUser";
    bool flag = true;

    LOG(number);
    LOG(pi);
    LOG(name);
    LOG(flag);

    std::cout << "\n=== MIXED TEXT AND VARIABLES ===\n";
    LOG("Number:", number);
    LOG("Pi value is", pi);
    LOG("User:", name, "Number:", number);
    LOG("Multiple values:", number, pi, name, flag);

    std::cout << "\n=== COLORED OUTPUT ===\n";
    LOG.color(Color::RED)("This is red text");
    LOG.color(Color::GREEN)("This is green text");
    LOG.color(Color::YELLOW)("This is yellow text");
    LOG.color(Color::BLUE)("This is blue text");
    LOG.color(Color::MAGENTA)("This is magenta text");
    LOG.color(Color::CYAN)("This is cyan text");

    std::cout << "\n=== COLORED WITH VARIABLES ===\n";
    LOG.color(Color::RED)("Error value:", number);
    LOG.color(Color::GREEN)("Success! Pi =", pi);
    LOG.color(Color::YELLOW)("Warning:", name, "has", number, "items");
    LOG.color(Color::CYAN)("Debug:", "flag =", flag, "number =", number);

    std::cout << "\n=== WITHOUT TIMESTAMPS ===\n";
    LOG.noTimestamp()("No timestamp here");
    LOG.noTimestamp()("Another message without time");
    LOG.noTimestamp().color(Color::MAGENTA)("Colored without timestamp");

    std::cout << "\n=== BACK TO NORMAL (with timestamp) ===\n";
    LOG("Timestamp is back automatically");
    LOG("Everything returns to default after each call");

    std::cout << "\n=== COMPLEX DATA ===\n";
    float array[] = {1.1f, 2.2f, 3.3f};
    LOG("Array values:", array[0], array[1], array[2]);

    char letter = 'X';
    LOG("Character:", letter, "Number:", number);

    std::cout << "\n=== REAL-WORLD EXAMPLES ===\n";
    LOG("Server started on port", 8080);
    LOG.color(Color::GREEN)("Connection established");
    LOG.color(Color::YELLOW)("Warning: High memory usage:", 87.5, "%");
    LOG.color(Color::RED)("Error: Failed to connect to database");
    LOG.color(Color::CYAN)("Debug: Processing user", name, "with ID", number);

    std::cout << "\n=== RAPID LOGGING ===\n";
    for(int i = 0; i < 5; i++) {
        LOG("Loop iteration", i, "processing...");
    }

    std::cout << "\n=== PERFORMANCE TIMING EXAMPLE ===\n";
    LOG.color(Color::CYAN)("Starting heavy computation...");
    // Simulate work
    LOG.color(Color::GREEN)("Computation complete! Result:", 12345);
}

std::string consoleManager::getTimestamp() {
    auto now = std::chrono::system_clock::now();
    auto time = std::chrono::system_clock::to_time_t(now);
    auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(
        now.time_since_epoch()) % 1000;

    std::stringstream ss;
    ss << std::put_time(std::localtime(&time), "%H:%M:%S");
    ss << '.' << std::setfill('0') << std::setw(3) << ms.count();
    return ss.str();
}

void consoleManager::printPrefix() {
    if (include_timestamp) {
        std::cout << Color::GRAY << "[" << getTimestamp() << "] " << Color::RESET;
    }
}