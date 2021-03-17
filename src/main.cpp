#include <spdlog/spdlog.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>

#define numVAOs 1

GLuint renderingProgram;
GLuint vao[numVAOs];
static std::string glsl_version;

void selectGLVersion(){
    // Decide GL+GLSL versions
#ifdef __APPLE__
    // GL 3.2 + GLSL 150
    glsl_version = "#version 330 \n";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint (GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint (GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);         // Required on Mac
#else
    // GL 3.0 + GLSL 130
        glsl_version = "#version 420 \n";
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
        //glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+ only
        //glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            // 3.0+ only
#endif
}

GLuint createShaderProgram(){
    std::string vShaderString =
            "void main(void) \n"
            "{gl_Position = vec4(0.0, 0.0, 0.0, 1.0);}";
    vShaderString.insert(0, glsl_version);
    const char* vShaderSource = vShaderString.c_str();

    std::string fShaderString =
            "out vec4 color; \n"
            "void main(void) \n"
            "{color = vec4(0.0, 0.0, 1.0, 1.0);}";
    fShaderString.insert(0, glsl_version);
    const char* fShaderSource = fShaderString.c_str();

    GLuint vfProgram = glCreateProgram();

    GLuint vShader = glCreateShader(GL_VERTEX_SHADER);
    GLuint fShader = glCreateShader(GL_FRAGMENT_SHADER);

    glShaderSource(vShader, 1, &vShaderSource, nullptr);
    glCompileShader(vShader);

    glShaderSource(fShader, 1, &fShaderSource, nullptr);\
    glCompileShader(fShader);

    glAttachShader(vfProgram, vShader);
    glAttachShader(vfProgram, fShader);

    glLinkProgram(vfProgram);

    return vfProgram;
}

void init(GLFWwindow* window) {
    renderingProgram = createShaderProgram();
    glGenVertexArrays(numVAOs, vao);
    glBindVertexArray(vao[0]);
}

void display(GLFWwindow* window, double currentTime){
//    glClearColor(1.0, 0.0, 0.0, 1.0);
//    glClear(GL_COLOR_BUFFER_BIT);
    glUseProgram(renderingProgram);
    glDrawArrays(GL_POINTS, 0, 1);
}

int main(int, char**){
    if (!glfwInit()) {
        spdlog::error("Error initializing GLFW!");
        return 1;
    }

    selectGLVersion();

    GLFWwindow* window = glfwCreateWindow(600, 600, "GLFW+ImGui+OpenGL", nullptr, nullptr);
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

    glfwSwapInterval(1); // Enable vsync

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