//
// Created by VokamisAir on 3/18/21.
//
#define GLEW_STATIC

#include <iostream>
#include <fstream>
#include <ctime>
#include <vector>
#include <map>
#include <thread>
#include <string>
#include <random>
#include <spdlog/spdlog.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#ifndef OPENGL_COMMON_H
#define OPENGL_COMMON_H

static std::string glsl_version;

void selectGLVersion();
std::string readSourceFile(const std::string& filePath);
void printShaderLog(GLuint shader);
void printProgramLog(int prog);
bool checkOpenGLError();
int randomInteger(int to, int from);

glm::mat4 buildTranslate(float x, float y, float z);
glm::mat4 buildRotateX(float rad);
glm::mat4 buildRotateY(float rad);
glm::mat4 buildRotateZ(float rad);
glm::mat4 buildScale(float x, float y, float z);

#endif //OPENGL_COMMON_H
