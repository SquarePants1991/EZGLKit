//
// Created by wangyang on 16/11/24.
//

#include "ELFileUtil.h"

#include <fstream>

std::string ELFileUtil::stringContentOfFile(char *filePath) {
    std::ifstream fileStream(filePath);
    std::string fileContent;
    std::string line;
    if (fileStream.is_open()) {
        fileStream.seekg(0, std::ios::end);
        long fileSize = fileStream.tellg();
        fileStream.seekg(0, std::ios::beg);
        char *content = new char[fileSize];
        fileStream.read(content,fileSize);
        return std::string(content);
    }
    fileStream.close();
    return "";
}