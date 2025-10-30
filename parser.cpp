//
// Created by Davis on 10/30/2025.
//

#include "parser.h"

std::string parser::parseFile(std::string requestLine, std::filesystem::path projectPath) {

    std::istringstream iss(requestLine);
    std::string method, pathStr, version;
    iss >> method >> pathStr >> version;

    // Remove leading '/'
    if (!pathStr.empty() && pathStr[0] == '/') {
        pathStr = pathStr.substr(1);
    }

    // Build the full path

    fs::path fullPath = fs::path("projects") / projectPath / pathStr;

    std::cout << "Full path: " << fullPath << std::endl;

    try {
        std::uintmax_t filesize = fs::file_size(fullPath);
        std::cout << "File size: " << filesize << " bytes" << std::endl;
    } catch (const fs::filesystem_error &err) {
        std::cout << "Error: " << err.what() << std::endl;
        // Handle 404
    }

    std::ifstream file(fullPath);
    std::cout << "opening read file !" << std::endl;
    if (!file.is_open()) {
        std::cout << "file open failed" << std::endl;
        // #FIXME: this is bad, if no file is found we dont get anything, what do we do here ?
    }
        std::string contents;
        std::stringstream buffer;
        buffer << file.rdbuf();
        contents = buffer.str();

        file.close();

        std::cout << "File contents:\n" << contents << std::endl;
        std::cout << "Total characters: " << contents.length() << std::endl;

        return contents;
    }

