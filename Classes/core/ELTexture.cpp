//
// Created by wangyang on 16/11/28.
//

#include "ELTexture.h"
#include "../EZGLBase.h"

std::map<std::string, ELTexture *> ELTexture::textureCache = std::map<std::string, ELTexture *>();
ELTextureGenCallback ELTexture::callback = NULL;
ELTextureResetCallback ELTexture::resetCallback = NULL;

ELTexture::ELTexture(std::string path, bool keepImgData) {
#if Platform_OSX
    int channel;
    unsigned char *imageData = SOIL_load_image(path.c_str(), &width, &height, &channel, SOIL_LOAD_RGBA);

    glGenTextures(1, &value);
    glBindTexture(GL_TEXTURE_2D, value);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, (GLsizei)width, (GLsizei)height, 0, GL_RGBA, GL_UNSIGNED_BYTE, imageData);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glBindTexture(GL_TEXTURE_2D, 0);
    
    imgData = imageData;
    if (keepImgData == false) {
        releaseImageData();
    }
#endif
    
    if (ELTexture::callback != NULL) {
        if (keepImgData) {
            value = callback(path.c_str(), &imgData, width, height);
        } else {
            value = callback(path.c_str(), NULL, width, height);
        }
    }
}

ELTexture::~ELTexture() {
    this->releaseImageData();
    glDeleteTextures(1, &value);
}

void ELTexture::releaseImageData() {
    if (imgData != NULL) {
        delete imgData;
        imgData = NULL;
    }
}

void ELTexture::config(ELTextureGenCallback callback, ELTextureResetCallback resetCallback) {
    ELTexture::callback = callback;
    ELTexture::resetCallback = resetCallback;
}

void ELTexture::clearCache() {
    for (std::map<std::string, ELTexture *>::iterator iter = ELTexture::textureCache.begin(); iter != ELTexture::textureCache.end(); ++iter) {
        delete iter->second;
    }
    ELTexture::textureCache.clear();
}

ELTexture * ELTexture::texture(std::string path, bool keepImgData) {
    if (ELTexture::textureCache.find(path) != ELTexture::textureCache.end()) {
        return ELTexture::textureCache[path];
    }
    ELTexture *texture = new ELTexture(path, keepImgData);
    ELTexture::textureCache[path] = texture;
    return texture;
}

void ELTexture::reset(uint8_t *imgData, ELInt width, ELInt height, GLenum pixelFormat, GLenum dataType) {
    if (ELTexture::resetCallback) {
        resetCallback(imgData, this->value, width, height, pixelFormat, dataType);
    }
}
