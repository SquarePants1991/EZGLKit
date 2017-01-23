//
// Created by wangyang on 16/11/24.
//

#include "ELFileUtil.h"
#include "ELAssets.h"

#include <fstream>
#include <regex>

std::string ELFileUtil::stringContentOfFile(const char *filePath) {
    std::ifstream fileStream(filePath);
    std::string fileContent;
    std::string line;
    if (fileStream.is_open()) {
        fileStream.seekg(0, std::ios::end);
        long fileSize = fileStream.tellg();
        fileStream.seekg(0, std::ios::beg);
        char *content = new char[fileSize + 1];
        fileStream.read(content,fileSize);
        content[fileSize] = '\0';
        return std::string(content);
    }
    fileStream.close();
    return "";
}

std::string ELFileUtil::stringContentOfShader(const char *filePath) {
    std::string fileContent = stringContentOfFile(filePath);
    std::regex includeRegex("#include[[:space:]]?\\<(([[:alpha:]]|_|\\.)+)\\>");
    auto resultBegin = std::sregex_iterator(fileContent.begin(), fileContent.end(),includeRegex);
    auto resultEnd = std::sregex_iterator();

    std::vector<std::string> includeStatements;
    std::vector<std::string> includeFilePaths;
    for (std::sregex_iterator i = resultBegin; i != resultEnd; ++i) {
        std::smatch match = *i;
        std::string includeStatement = match.str();
        std::string includeFileName = match.str(1);
        std::string includeFilePath = ELAssets::shared()->findFile(includeFileName);
        includeStatements.push_back(includeStatement);
        includeFilePaths.push_back(includeFilePath);
    }
    for (int j = 0; j < includeFilePaths.size(); ++j) {
        std::string includeFilePath = includeFilePaths.at(j);
        std::string includeStatement = includeStatements.at(j);
        if (includeFilePath.compare("") != 0) {
            std::string replaceShaderContent = stringContentOfShader(includeFilePath.c_str());
            std::size_t found = fileContent.rfind(includeStatement);
            if (found != std::string::npos) {
                fileContent = fileContent.replace(found,includeStatement.size(),replaceShaderContent);
            } else {
                printf("ReplaceStatement [%s] not exists!!!",includeStatement.c_str());
            }
        } else {
            printf("Shader File [%s] not exists!!!",includeFilePath.c_str());
        }
    }
    return fileContent;
}