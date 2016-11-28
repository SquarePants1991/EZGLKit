file(GLOB SRC
        "*.h"
        "*.c"
        "*.cc"
)

add_library(soil SOIL.h SOIL.cc ${SRC})
