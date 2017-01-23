//
// Created by wangyang on 16/11/28.
//

#ifndef EZGL_ELTEXTURE_H
#define EZGL_ELTEXTURE_H

#include <string>
#include "EZGLBase.h"

typedef GLuint (*ELTextureGenCallback)(const char* path);
class ELTexture {
public:
    GLuint value;

public:
    static ELTexture * texture(std::string path);
    static void config(ELTextureGenCallback callback);
private:
    static ELTextureGenCallback callback;
    static std::map<std::string, ELTexture *> textureCache;
    ELTexture(std::string path);
};


#endif //EZGL_ELTEXTURE_H
