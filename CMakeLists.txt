cmake_minimum_required(VERSION 3.17)

set(PROJECT_NAME "untitled")
set(CMAKE_CONFIGURATION_TYPES "MinSizeRel; Debug" CACHE STRING "" FORCE)
list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

project(${PROJECT_NAME})
set(CMAKE_CXX_STANDARD 17)

include(conanbuildinfo.cmake)
conan_basic_setup()

file(GLOB H_FILES "src/*.h")
file(GLOB CPP_FILES "src/*.cpp")
file(GLOB GLSL_FILES "src/*.glsl")

file(COPY ${GLSL_FILES} DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

add_executable(${PROJECT_NAME} ${H_FILES} ${CPP_FILES})

#include/imgui_impl_opengl3.cpp
#include/imgui_impl_glfw.cpp
target_link_libraries(${PROJECT_NAME} PRIVATE ${CONAN_LIBS})
