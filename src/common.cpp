#include "common.h"

void selectGLVersion(){
#ifdef __APPLE__
    glsl_version = "#version 330 \n";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint (GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint (GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#else
    glsl_version = "#version 420 \n";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    //glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    //glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
}

std::string readSourceFile(const std::string& filePath){
    try {
        spdlog::debug("Path: {} Exists: {}", filePath, std::filesystem::exists(filePath));

        if (std::filesystem::exists(filePath)) {
            std::ifstream sourceFile(filePath, std::ios::in);

            std::string sourceCode = glsl_version;
            std::string line;

            while (getline(sourceFile, line)) {
                sourceCode.append(line + "\n");
            }

            sourceFile.close();
            return sourceCode;
        }
        else {
            spdlog::error("File not found. {}", filePath);
            return glsl_version;
        }
    }
    catch (std::exception &e){
        spdlog::error(e.what());
        return glsl_version;
    }
}


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

