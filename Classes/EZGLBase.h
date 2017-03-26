//
// Created by wangyang on 16/11/24.
//

#ifndef EZGL_EZGLBASE_H
#define EZGL_EZGLBASE_H

#include <string>
#include <vector>
#include <map>

#if Platform_OSX
#include <glad/glad/glad.h>
#include <GLFW/glfw3.h>
#include <soil/SOIL.h>

#define glBindVertexArrayEL(value) glBindVertexArray(value)
#define glGenVertexArraysEL(num, value) glGenVertexArrays(num, value)

#elif Platform_iOS
#include <OpenGLES/ES2/glext.h>
#define glBindVertexArrayEL(value) glBindVertexArrayOES(value)
#define glGenVertexArraysEL(num, value) glGenVertexArraysOES(num, value)

#endif


#define BUFFER_OFFSET(i) ((char *)NULL + (i))

#endif //EZGL_EZGLBASE_H
