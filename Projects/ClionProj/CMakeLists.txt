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

include_directories(${PROJECT_SOURCE_DIR}/../../libs/soil)
add_subdirectory(${PROJECT_SOURCE_DIR}/../../libs/soil ${CMAKE_CURRENT_BINARY_DIR}/soil)

include_directories(${PROJECT_SOURCE_DIR}/../../libs/wavefront)
add_subdirectory(${PROJECT_SOURCE_DIR}/../../libs/wavefront ${CMAKE_CURRENT_BINARY_DIR}/wavefront)

include_directories(${PROJECT_SOURCE_DIR}/../../libs/bullet)
add_subdirectory(${PROJECT_SOURCE_DIR}/../../libs/bullet ${CMAKE_CURRENT_BINARY_DIR}/bullet)

include_directories(${PROJECT_SOURCE_DIR}/../../libs/lua)
add_subdirectory(${PROJECT_SOURCE_DIR}/../../libs/lua ${CMAKE_CURRENT_BINARY_DIR}/lua)

macro(add_exe target prefix)
    file(GLOB TARGET_SRC
            "${prefix}/*.h"
            "${prefix}/*.cpp"
            "${prefix}/**/*.h"
            "${prefix}/**/*.cpp"
    )
    add_executable(${target} ${prefix}/main.cpp ${TARGET_SRC})
    target_link_libraries(${target} EZGLib ${GLUT_LIBRARY} ${OPENGL_LIBRARIES} glfw glad soil wavefront bullet lua)
endmacro(add_exe)



add_exe(EZGL .)
add_exe(TestBaseWindow BaseWindow)
add_exe(TestShader Shader)
add_exe(FPSGame "FPS Game")
add_exe(FPSDemo2 "FPS Demo2")
add_exe(LuaTest "LuaTest")