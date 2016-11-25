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
#include <GL/glut.h>
#endif

#define glBindVertexArray(value) glBindVertexArrayAPPLE(value)
#define glGenVertexArrays(num, value) glGenVertexArraysAPPLE(num, value)

#define BUFFER_OFFSET(i) ((char *)NULL + (i))

#endif //EZGL_EZGLBASE_H
