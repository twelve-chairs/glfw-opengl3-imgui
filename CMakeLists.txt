cmake_minimum_required(VERSION 3.17)

set(CMAKE_CONFIGURATION_TYPES "MinSizeRel; Debug" CACHE STRING "" FORCE)
list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

project(untitled)
set(CMAKE_CXX_STANDARD 17)

include(conanbuildinfo.cmake)
conan_basic_setup()

add_executable(untitled
        src/common.h
        src/common.cpp
        src/main.h
        src/main.cpp
        )

#include/imgui_impl_opengl3.cpp
#include/imgui_impl_glfw.cpp
target_link_libraries(untitled PRIVATE ${CONAN_LIBS})
