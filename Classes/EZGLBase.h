//
// Created by wangyang on 16/11/24.
//

#ifndef EZGL_EZGLBASE_H
#define EZGL_EZGLBASE_H

#define Platform_OSX 1

#include <string>
#include <vector>
#include <map>

#if Platform_OSX
#include "glad/glad.h"
#include <GLFW/glfw3.h>
#endif

#define glBindVertexArray(value) glad_glBindVertexArray(value)
#define glGenVertexArrays(num, value) glad_glGenVertexArrays(num, value)

#define BUFFER_OFFSET(i) ((char *)NULL + (i))

#endif //EZGL_EZGLBASE_H
