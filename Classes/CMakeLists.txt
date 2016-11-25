cmake_minimum_required(VERSION 2.8)

IF (WIN32)
    MESSAGE(STATUS "Now is windows")
    set (Platform_WIN 1)
ELSEIF (APPLE)
    MESSAGE(STATUS "Now is Apple systens.")
    set (Platform_OSX 1)
ELSEIF (UNIX)
    MESSAGE(STATUS "Now is UNIX-like OS's. Including aPPLE os x  and CygWin")
    set (Platform_UNIX 1)
ENDIF ()

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


file(GLOB EL_SRC
        "core/*.h"
        "core/*.cpp"
        "types/*.h"
        "types/*.cpp"
        "utils/*.h"
        "utils/*.cpp"
        "components/*.h"
        "components/*.cpp"
        "components/geometry/*.h"
        "components/geometry/*.cpp"
)

add_library(EZGLib EZGL.h EZGL.cpp ${EL_SRC})
