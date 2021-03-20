#include "common.h"

void printShaderLog(GLuint shader){
    int len = 0;
    int chWrittn = 0;
    char* log;
    glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &len);
    if (len > 0) {
        log = (char*)malloc(len);
        glGetShaderInfoLog(shader, len, &chWrittn, log);
        spdlog::info("Shader Info Log: {}", log);
        free(log);
    }
}


void printProgramLog(int prog){
    int len = 0;
    int chWrittn = 0;
    char* log;
    glGetProgramiv(prog, GL_INFO_LOG_LENGTH, &len);
    if (len > 0){
        log = (char*)malloc(len);
        glGetProgramInfoLog(prog, len, &chWrittn, log);
        spdlog::info("Program Info Log", log);
        free(log);
    }
}


bool checkOpenGLError(){
    bool foundError = false;
    int glErr = glGetError();
    while (glErr != GL_NO_ERROR){
        spdlog::error(glErr);
        foundError = true;
        glErr = glGetError();
    }
    return foundError;
}


int randomInteger(int to, int from){
    std::random_device randomizerSeed;
    std::default_random_engine randomEngine(randomizerSeed());
    std::uniform_int_distribution<int> randomRange(from, to);
    return randomRange(randomEngine);
}

