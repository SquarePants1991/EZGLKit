//
// Created by wangyang on 16/11/24.
//

#ifndef EZGL_ELPROGRAM_H
#define EZGL_ELPROGRAM_H

#include "../EZGLBase.h"

class ELProgram {
public:
    GLuint value;
    bool isValid;
public:
    ELProgram(const char *vertexShader,const char *fragmentShader);
    ~ELProgram();
    GLuint uniform(char *uniformName);

private:
    std::map<std::string,GLuint> uniformCache;
};


#endif //EZGL_ELPROGRAM_H
