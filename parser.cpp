//
// Created by Davis on 10/30/2025.
//

#include "parser.h"
#include "consoleManager.h"

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

    try {
        std::uintmax_t filesize = fs::file_size(fullPath);
    } catch (const fs::filesystem_error &err) {
        LOG.color(Color::RED)("Error:", err.what());
        // Handle 404
    }

    std::ifstream file(fullPath);
    if (!file.is_open()) {
        LOG.color(Color::RED)("File open failed");
        // #FIXME: send to server error page, for future this should be logged in a debug/err file somewhere

    }
    std::string contents;
    std::stringstream buffer;
    buffer << file.rdbuf();
    contents = buffer.str();

    file.close();

    return contents;
}

std::string parser::getFileType(const fs::path& fullPath) {
    const auto fileType = fullPath.extension().string();
    if (fileType == ".html" || fileType == ".htm") return "text/html";
    if (fileType == ".css") return "text/css";
    if (fileType == ".js") return "application/javascript";
}