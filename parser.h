//
// Created by Davis on 10/30/2025.
//

#ifndef UNTITLED_PARSER_H
#define UNTITLED_PARSER_H
#include <fstream>
#include <filesystem>
#include <iostream>
namespace fs = std::filesystem;

class parser {
public:
    std::string parseFile(std::string requestLine, std::filesystem::path projectPath, int& statusCode, std::string &mime);
    std::string getFileType(const fs::path& fullPath);


    bool fileExists(const std::string& filename);
};


#endif //UNTITLED_PARSER_H