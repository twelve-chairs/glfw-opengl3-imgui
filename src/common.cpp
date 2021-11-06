//
// Created by VokamisAir on 1/9/21.
//

#include "common.h"

int randomInteger(int to, int from){
    std::random_device randomizerSeed;
    std::default_random_engine randomEngine(randomizerSeed());
    std::uniform_int_distribution<int> randomRange(from, to);
    return randomRange(randomEngine);
}

float randomFloat(float to, float from){
    std::random_device randomizerSeed;
    std::default_random_engine randomEngine(randomizerSeed());
    std::uniform_int_distribution<float> randomRange(from, to);
    return randomRange(randomEngine);
}

void printShaderLog(GLuint shader){
    int len = 0;
    int charsWritten = 0;
    char *log;
    glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &len);
    if (len > 0 ){
        log = (char *)malloc(len);
        glGetShaderInfoLog(shader, len, &charsWritten, log);
        spdlog::info("Shader info Log: {}", log);
        free(log);
    }
}

void printProgramLog(int program){
    int len = 0;
    int charWritten = 0;
    char *log;
    glGetProgramiv(program, GL_INFO_LOG_LENGTH, &len);
    if (len > 0){
        log = (char*)malloc(len);
        glGetProgramInfoLog(program, len, &charWritten, log);
        spdlog::info("Program info log: {}", log);
        free(log);
    }
}

bool checkOpenGLError(){
    bool errorFound = false;
    int glError = glGetError();
    while (glError != GL_NO_ERROR){
        spdlog::error("glError: {}", glError);
        errorFound = true;
        glError = glGetError();
    }
    return errorFound;
}

std::string readShaderSource(const char* filePath){
    spdlog::info(filePath);
    std::ifstream fileStream(filePath, std::ios::in);
    std::string line = "";
    std::string content;
    try {
        while (!fileStream.eof()){
            std::getline(fileStream, line);
            content.append(line + "\n");
        }
        fileStream.close();
    }

    catch (std::exception &e){
        spdlog::error("readShaderSource: {}", e.what());
        return NULL;
    }
    return content;
}
