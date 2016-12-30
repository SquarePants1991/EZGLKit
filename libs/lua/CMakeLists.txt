set(CMAKE_MACOSX_RPATH 1)
file(GLOB SRC
        "*.h"
        "*.c"
        "*.cc"
        "*.hpp"
        "*.cpp"
)

add_library(lua ${SRC})