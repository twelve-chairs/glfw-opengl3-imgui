//
// Created by VokamisAir on 1/9/21.
//

#ifndef ZXPAINT_COMMON_H
#define ZXPAINT_COMMON_H


//#include <ctime>
//#include <vector>
//#include <map>
#include <string>
#include <random>
#include <iostream>
#include <fstream>
#include <cstdio>

#include <GL/glew.h>

#include "imgui.h"
#include "include/imgui_impl_glfw.h"
#include "include/imgui_impl_opengl3.h"

#include <GLFW/glfw3.h> // Will drag system OpenGL headers

#include <spdlog/spdlog.h>

int randomInteger(int to, int from = 0);
void printShaderLog(GLuint shader);
void printProgramLog(int program);
bool checkOpenGLError();
std::string readShaderSource(const char* filePath);

#endif //ZXPAINT_COMMON_H
