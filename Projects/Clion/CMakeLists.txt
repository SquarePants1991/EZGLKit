cmake_minimum_required(VERSION 3.3)
project(EZGL)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES main.cpp ../../Classes/ ../../Classes/types/EZGLBasicTypes.h ../../Classes/types/EZGLMatrixTypes.h ../../Classes/types/EZGLMatrix4.h ../../Classes/types/EZGLQuaternion.h ../../Classes/types/ELVector2.h ../../Classes/types/ELVector3.h ../../Classes/types/ELVector4.h)
add_executable(EZGL ${SOURCE_FILES})