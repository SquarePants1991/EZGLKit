cmake_minimum_required(VERSION 2.8)
project(EZGL CXX)

#########################################################
# FIND GLUT
#########################################################
find_package(GLUT REQUIRED)
include_directories(${GLUT_INCLUDE_DIRS})
link_directories(${GLUT_LIBRARY_DIRS})
add_definitions(${GLUT_DEFINITIONS})
if(NOT GLUT_FOUND)
    message(ERROR " GLUT not found!")
endif(NOT GLUT_FOUND)

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

include_directories(/usr/local/Cellar/freeglut/3.0.0/include)

include_directories(${PROJECT_SOURCE_DIR}/../../Classes/)
add_subdirectory(${PROJECT_SOURCE_DIR}/../../Classes/ ${CMAKE_CURRENT_BINARY_DIR}/EZGLib)

macro(add_exe target prefix)
    add_executable(${target} ${prefix}/main.cpp)
    target_link_libraries(${target} EZGLib ${GLUT_LIBRARY} ${OPENGL_LIBRARIES})
endmacro(add_exe)



add_exe(EZGL .)
add_exe(TestBaseWindow BaseWindow)
add_exe(TestShader Shader)