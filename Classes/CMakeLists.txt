cmake_minimum_required(VERSION 2.8)
set(CMAKE_MACOSX_RPATH 1)

include_directories(${PROJECT_SOURCE_DIR}/../../libs/)
include_directories(${PROJECT_SOURCE_DIR}/../../libs/bullet)

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

add_library(EZGLib EZGL.h EZGL.cpp EZGLBase.h ${EL_SRC})
target_compile_definitions(EZGLib PRIVATE Platform_OSX=1)

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