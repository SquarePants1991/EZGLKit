//
// Created by wangyang on 16/11/28.
//

#include "ELAssets.h"

#include <sys/stat.h>

ELAssets * ELAssets::_shared = NULL;

ELAssets * ELAssets::shared() {
    if (_shared == NULL) {
        _shared = new ELAssets();
    }
    return _shared;
}

void ELAssets::addSearchPath(std::string searchPath) {
    searchPaths.push_back(searchPath);
}

std::string ELAssets::findFile(std::string fileName) {
    struct stat buffer;
    std::vector<std::string> _searchPaths = searchPaths;
    if (stat (fileName.c_str(), &buffer) == 0) {
        return fileName;
    }
    _searchPaths.push_back("");
    for (int i = 0; i < _searchPaths.size(); ++i) {
        std::string path = _searchPaths.at(i) + fileName;
        if (stat (path.c_str(), &buffer) == 0) {
            return path;
        }
    }
    return "";
}
