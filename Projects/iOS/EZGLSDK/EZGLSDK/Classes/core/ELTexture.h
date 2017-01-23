//
// Created by wangyang on 16/11/28.
//

#ifndef EZGL_ELTEXTURE_H
#define EZGL_ELTEXTURE_H

#include <string>
#include "EZGLBase.h"

class ELTexture {
public:
    GLuint value;

public:
    static ELTexture * texture(std::string path);

private:
    static std::map<std::string, ELTexture *> textureCache;
    ELTexture(std::string path);
};


#endif //EZGL_ELTEXTURE_H
