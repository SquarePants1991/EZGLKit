cmake_minimum_required(VERSION 2.8)
project(EZGL CXX)

find_package(glfw3 3.2 REQUIRED)

#########################################################
# FIND OPENGL
#########################################################
find_package(OpenGL REQUIRED)
include_directories(${OpenGL_INCLUDE_DIRS})
link_directories(${OpenGL_LIBRARY_DIRS})
add_definitions(${OpenGL_DEFINITIONS})
if(NOT OPENGL_FOUND)
    message(ERROR " OPENGL not found!")
endif(NOT OPENGL_FOUND)

include_directories(/usr/local/)
include_directories(/usr/local/include/)

include_directories(${PROJECT_SOURCE_DIR}/../../Classes/)
add_subdirectory(${PROJECT_SOURCE_DIR}/../../Classes/ ${CMAKE_CURRENT_BINARY_DIR}/EZGLib)

include_directories(${PROJECT_SOURCE_DIR}/../../libs/glad/include)
add_subdirectory(${PROJECT_SOURCE_DIR}/../../libs/glad ${CMAKE_CURRENT_BINARY_DIR}/glad)

macro(add_exe target prefix)
    add_executable(${target} ${prefix}/main.cpp)
    target_link_libraries(${target} EZGLib ${GLUT_LIBRARY} ${OPENGL_LIBRARIES} glfw glad)
endmacro(add_exe)



add_exe(EZGL .)
add_exe(TestBaseWindow BaseWindow)
add_exe(TestShader Shader)