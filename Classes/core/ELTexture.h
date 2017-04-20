//
// Created by wangyang on 16/11/28.
//

#ifndef EZGL_ELTEXTURE_H
#define EZGL_ELTEXTURE_H

#include <string>
#include "../EZGLBase.h"
#include "../types/EZGLTypes.h"

typedef GLuint (*ELTextureGenCallback)(const char* path, unsigned char **data, ELInt &width, ELInt &height);
typedef void (*ELTextureResetCallback)(uint8_t *imageData, ELInt texID, ELInt width, ELInt height, GLenum pixelFormat, GLenum dataType);
class ELTexture {
public:
    GLuint value;
    unsigned char *imgData;
    ELInt width;
    ELInt height;
public:
    static ELTexture * texture(std::string path, bool keepImgData = false);
    static void config(ELTextureGenCallback callback, ELTextureResetCallback resetCallback);
    static void clearCache();
    void reset(uint8_t *imgData, ELInt width, ELInt height, GLenum pixelFormat, GLenum dataType = GL_UNSIGNED_BYTE);
private:
    static ELTextureGenCallback callback;
    static ELTextureResetCallback resetCallback;
    static std::map<std::string, ELTexture *> textureCache;

    ~ELTexture();
    ELTexture(std::string path, bool keepImgData = false);
    void releaseImageData();
};


#endif //EZGL_ELTEXTURE_H
