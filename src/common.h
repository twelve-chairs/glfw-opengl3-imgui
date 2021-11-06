//
// Created by VokamisAir on 1/9/21.
//

#ifndef ZXPAINT_COMMON_H
#define ZXPAINT_COMMON_H


#include <string>
#include <random>
#include <iostream>
#include <fstream>
#include <cmath>
#include <cstdio>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <GL/glew.h>

#include "imgui.h"
#include "include/imgui_impl_glfw.h"
#include "include/imgui_impl_opengl3.h"

#include <GLFW/glfw3.h> // Will drag system OpenGL headers

#include <spdlog/spdlog.h>

int randomInteger(int to, int from = 0);
float randomFloat(float to, float from = 0);

void printShaderLog(GLuint shader);
void printProgramLog(int program);
bool checkOpenGLError();
std::string readShaderSource(const char* filePath);

#endif //COMMON_H
