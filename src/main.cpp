#include "main.h"


GLuint createShaderProgram(){
    std::string vertexShaderSource = readSourceFile("vertexShader.glsl");
    std::string fragmentShaderSource = readSourceFile("fragmentShader.glsl");
    const char* vertexShaderSourceString = vertexShaderSource.c_str();
    const char* fragmentShaderSourceString = fragmentShaderSource.c_str();

    GLuint shaderProgram = glCreateProgram();
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

    glShaderSource(vertexShader, 1, &vertexShaderSourceString, nullptr);
    glCompileShader(vertexShader);

    glShaderSource(fragmentShader, 1, &fragmentShaderSourceString, nullptr);\
    glCompileShader(fragmentShader);

    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);

    glLinkProgram(shaderProgram);

    return shaderProgram;
}

void init(GLFWwindow* window) {
    renderingProgram = createShaderProgram();
    glGenVertexArrays(numVAOs, vertexArrayObjects);
    glBindVertexArray(vertexArrayObjects[0]);
}

void display(GLFWwindow* window, double currentTime){

    glClear(GL_DEPTH_BUFFER_BIT);
    glClearColor(0.0, 0.0, 0.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);

    glUseProgram(renderingProgram);

//    glPointSize(60);
//    glDrawArrays(GL_POINTS, 0, 1);

    x += inc;
    if (x > 1.0f) inc = -0.01f;
    if (x < -1.0f) inc = 0.01f;
    GLuint offsetLocation = glGetUniformLocation(renderingProgram, "offset");
    glProgramUniform1f(renderingProgram, offsetLocation, x);

    glDrawArrays(GL_TRIANGLES, 0, 3);
}

int main(int, char**){
    if (!glfwInit()) {
        spdlog::error("Error initializing GLFW!");
        return 1;
    }

    selectGLVersion();

    GLFWwindow* window = glfwCreateWindow(300, 300, "GLFW+ImGui+OpenGL", nullptr, nullptr);
    if (window == nullptr) {
        spdlog::error("Failed to create GLFW window!\n");
        return 1;
    }

    glfwMakeContextCurrent(window);

    // Initialize OpenGL loader
    if (glewInit() != GLEW_OK){
        spdlog::error("Failed to initialize OpenGL loader!\n");
        return 1;
    }

    // Enable vsync
    glfwSwapInterval(1);

    init(window);

    while (!glfwWindowShouldClose(window)){
        display(window, glfwGetTime());
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}