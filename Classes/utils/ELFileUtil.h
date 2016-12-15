//
// Created by wangyang on 16/11/24.
//

#ifndef EZGL_ELFILEUTIL_H
#define EZGL_ELFILEUTIL_H

#include "EZGLBase.h"

class ELFileUtil {
public:
    static std::string stringContentOfFile(const char *filePath);
    static std::string stringContentOfShader(const char *filePath);
};


#endif //EZGL_ELFILEUTIL_H
