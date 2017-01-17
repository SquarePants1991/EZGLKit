file(GLOB SRC
        "*.h"
        "*.c"
        "*.cc"
        "*.hpp"
        "*.cpp"
)

add_library(lua ${SRC})