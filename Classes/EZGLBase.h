//
// Created by wangyang on 16/11/24.
//

#ifndef EZGL_EZGLBASE_H
#define EZGL_EZGLBASE_H

#include <string>
#include <vector>
#include <map>
#include <memory>

#if Platform_OSX
#include <glad/glad/glad.h>
#include <GLFW/glfw3.h>
#include <soil/SOIL.h>

#define glBindVertexArrayEL(value) glBindVertexArray(value)
#define glGenVertexArraysEL(num, value) glGenVertexArrays(num, value)
#define glDeleteVertexArraysEL(num, value) glDeleteVertexArrays(num, value)

#elif Platform_iOS
#include <OpenGLES/ES2/glext.h>
#define glBindVertexArrayEL(value) glBindVertexArrayOES(value)
#define glGenVertexArraysEL(num, value) glGenVertexArraysOES(num, value)
#define glDeleteVertexArraysEL(num, value) glDeleteVertexArraysOES(num, value)
#endif


#define BUFFER_OFFSET(i) ((char *)NULL + (i))

#define prop_strong(type,ptr) std::shared_ptr<type> ptr
#define prop_weak(type,ptr) std::weak_ptr<type> ptr
#define retain_ptr(type,ptr) std::shared_ptr<type>(ptr)
#define retain_ptr_init(type) std::shared_ptr<type>(new type())
#define retain_ptr_init_v(type, ...) std::shared_ptr<type>(new type(__VA_ARGS__))

#endif //EZGL_EZGLBASE_H
