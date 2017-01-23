//
// Created by wangyang on 16/11/28.
//

#include "ELTexture.h"
#include "EZGLBase.h"

std::map<std::string, ELTexture *> ELTexture::textureCache = std::map<std::string, ELTexture *>();
ELTextureGenCallback ELTexture::callback = NULL;

ELTexture::ELTexture(std::string path) {
#if Platform_OSX
    int width, height, channel;
    unsigned char *imageData = SOIL_load_image(path.c_str(), &width, &height, &channel, SOIL_LOAD_RGBA);

    glGenTextures(1, &value);
    glBindTexture(GL_TEXTURE_2D, value);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, (GLsizei)width, (GLsizei)height, 0, GL_RGBA, GL_UNSIGNED_BYTE, imageData);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glBindTexture(GL_TEXTURE_2D, 0);
#endif
    
    if (ELTexture::callback != NULL) {
        value = callback(path.c_str());
    }
}

void ELTexture::config(ELTextureGenCallback callback) {
    ELTexture::callback = callback;
}

ELTexture * ELTexture::texture(std::string path) {
    if (ELTexture::textureCache.find(path) != ELTexture::textureCache.end()) {
        return ELTexture::textureCache[path];
    }
    ELTexture *texture = new ELTexture(path);
    ELTexture::textureCache[path] = texture;
    return texture;
}

