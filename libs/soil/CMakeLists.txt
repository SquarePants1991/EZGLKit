set(CMAKE_MACOSX_RPATH 1)
file(GLOB SRC
        "*.h"
        "*.c"
        "*.cc"
)

add_library(soil SOIL.h SOIL.cc ${SRC})

find_package(OpenGL REQUIRED)
include_directories(${OpenGL_INCLUDE_DIRS})
link_directories(${OpenGL_LIBRARY_DIRS})
add_definitions(${OpenGL_DEFINITIONS})
if(NOT OPENGL_FOUND)
    message(ERROR " OPENGL not found!")
endif(NOT OPENGL_FOUND)

target_link_libraries(soil "-framework CoreFoundation" ${OPENGL_LIBRARIES})