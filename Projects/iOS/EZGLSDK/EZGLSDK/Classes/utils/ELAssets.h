//
// Created by wangyang on 16/11/28.
//

#ifndef EZGL_ELASSETS_H
#define EZGL_ELASSETS_H

#include <vector>
#include <string>

class ELAssets {
public:
    static ELAssets *shared();
    void addSearchPath(std::string searchPath);
    std::string findFile(std::string fileName);

private:
    static ELAssets *_shared;
    std::vector<std::string> searchPaths;
};


#endif //EZGL_ELASSETS_H
