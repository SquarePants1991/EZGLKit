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
        "component/*.h"
        "component/*.cpp"
        "component/**/*.h"
        "component/**/*.cpp"
        "prefab/*.h"
        "prefab/*.cpp"
        "lua interface/*.h"
        "lua interface/*.cpp"
)

add_library(EZGLib EZGL.h EZGL.cpp ${EL_SRC})

macro(copy_files target GLOBPAT0 GLOBPAT1 DESTINATION)
    file(GLOB COPY_FILES
            RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}
            ${GLOBPAT0}/*
            ${GLOBPAT0}/**/*
            ${GLOBPAT1}/*
            ${GLOBPAT1}/**/*)
    make_directory(${DESTINATION})
    add_custom_target(${target} ALL
            COMMENT "Copying files: ${GLOBPAT}")
    foreach(FILENAME ${COPY_FILES})
        message("Copying ${CMAKE_CURRENT_SOURCE_DIR}/${FILENAME}")
        set(SRC "${CMAKE_CURRENT_SOURCE_DIR}/${FILENAME}")
        get_filename_component(fileName ${FILENAME} NAME)
        set(DST "${DESTINATION}/${fileName}")

        add_custom_command(
                TARGET ${target}
                COMMAND ${CMAKE_COMMAND} -E copy ${SRC} ${DST}
        )
    endforeach(FILENAME)
endmacro(copy_files)
copy_files(CopyShaders assets/shaders assets/textures ${CMAKE_BINARY_DIR})

add_dependencies(EZGLib CopyShaders)