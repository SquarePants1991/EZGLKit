cmake_minimum_required(VERSION 2.8)

include_directories(${PROJECT_SOURCE_DIR}/../../libs/glad/include)
include_directories(${PROJECT_SOURCE_DIR}/../../libs/soil)
include_directories(${PROJECT_SOURCE_DIR}/../../libs/wavefront)
include_directories(${PROJECT_SOURCE_DIR}/../../libs/bullet)
include_directories(${PROJECT_SOURCE_DIR}/../../libs/lua)

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
