//
// Created by VokamisAir on 3/18/21.
//

#include <filesystem>
#include <limits>
#include <iostream>
#include <fstream>
#include <ctime>
#include <vector>
#include <map>
#include <iterator>
#include <string>
#include <random>
#include <spdlog/spdlog.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#ifndef UNTITLED_COMMON_H
#define UNTITLED_COMMON_H

static std::string glsl_version;

void selectGLVersion();
std::string readShaderSource(std::string filePath);
void printShaderLog(GLuint shader);
void printProgramLog(int prog);
bool checkOpenGLError();
int randomInteger(int to, int from);

#endif //UNTITLED_COMMON_H
