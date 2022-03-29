//------- Ignore this ----------
#include<filesystem>
namespace fs = std::filesystem;
//------------------------------

#if defined(IMGUI_IMPL_OPENGL_ES2)
#endif

// [Win32] Our example includes a copy of glfw3.lib pre-compiled with VS2010 to maximize ease of testing and compatibility with old VS compilers.
// To link with VS2010-era libraries, VS2015+ requires linking with legacy_stdio_definitions.lib, which we do using this pragma.
// Your own project should not be affected, as you are likely to link with a newer binary of GLFW that is adequate for your version of Visual Studio.
#if defined(_MSC_VER) && (_MSC_VER >= 1900) && !defined(IMGUI_DISABLE_WIN32_FUNCTIONS)
#pragma comment(lib, "legacy_stdio_definitions")
#endif

#include <iostream>
#include <spdlog/spdlog.h>
#include <GL/glew.h>
#include "imgui.h"
#include "include/imgui_impl_glfw.h"
#include "include/imgui_impl_opengl3.h"
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <unistd.h>

#define STB_IMAGE_IMPLEMENTATION
#include "Texture.h"
#include "Shader.h"
#include "VAO.h"
#include "VBO.h"
#include "EBO.h"
#include "Camera.h"

const int FPS = 60.0f;
const char* GLSL_VERSION;
unsigned int mainWindowWidth = 1300;
unsigned int mainWindowHeight = 860;

GLuint frameBufferObject;
GLuint renderBufferObject;
unsigned int textureColorBuffer;

// OpenGL window default size and position
ImVec2 glWindowSize = ImVec2(600, 400);
ImVec2 frameBufferSize = glWindowSize;
ImVec2 glWindowPosition;

static float tx1 = 0.0f;
static float tx2 = 0.0f;
static float tx3 = 0.0f;
static float tx4 = 0.0f;
static float tx5 = 0.0f;
static float tx6 = 0.25f;


// Vertices coordinates
static float planeVertices[18] =
        {
        -128.0f, 0.0f, -128.0f, -128.0f, 0.0f, 128.0f, 128.0f, 0.0f, -128.0f,
        128.0f, 0.0f, -128.0f, -128.0f, 0.0f, 128.0f, 128.0f, 0.0f, 128.0f
};
static float planeCoordinates[12] = {
        0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,
        1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f
};


static GLfloat pyramidVertices[] =
        { //     COORDINATES        /        COLORS          /    TexCoord   /        NORMALS       //
                -0.5f, 0.0f,  0.5f,     0.83f, 0.70f, 0.44f,    0.0f, 0.0f,      0.0f, -1.0f, 0.0f, // Bottom side
                -0.5f, 0.0f, -0.5f,     0.83f, 0.70f, 0.44f,    0.0f, 1.0f,      0.0f, -1.0f, 0.0f, // Bottom side
                0.5f, 0.0f, -0.5f,     0.83f, 0.70f, 0.44f,	    1.0f, 1.0f,      0.0f, -1.0f, 0.0f, // Bottom side
                0.5f, 0.0f,  0.5f,     0.83f, 0.70f, 0.44f,	    1.0f, 0.0f,      0.0f, -1.0f, 0.0f, // Bottom side

                -0.5f, 0.0f,  0.5f,     0.83f, 0.70f, 0.44f,    0.0f, 0.0f,     -0.8f, 0.5f,  0.0f, // Left Side
                -0.5f, 0.0f, -0.5f,     0.83f, 0.70f, 0.44f,    1.0f, 0.0f,     -0.8f, 0.5f,  0.0f, // Left Side
                0.0f, 0.8f,  0.0f,     0.92f, 0.86f, 0.76f,	    1.0f, 1.0f,     -0.8f, 0.5f,  0.0f, // Left Side

                -0.5f, 0.0f, -0.5f,    0.83f, 0.70f, 0.44f,	    0.0f, 0.0f,      0.0f, 0.5f, -0.8f, // Non-facing side
                0.5f, 0.0f, -0.5f,     0.83f, 0.70f, 0.44f,	    1.0f, 0.0f,      0.0f, 0.5f, -0.8f, // Non-facing side
                0.0f, 0.8f,  0.0f,     0.92f, 0.86f, 0.76f,	    0.5f, 1.0f,      0.0f, 0.5f, -0.8f, // Non-facing side

                0.5f, 0.0f, -0.5f,     0.83f, 0.70f, 0.44f,	    0.0f, 0.0f,      0.8f, 0.5f,  0.0f, // Right side
                0.5f, 0.0f,  0.5f,     0.83f, 0.70f, 0.44f,	    1.0f, 0.0f,      0.8f, 0.5f,  0.0f, // Right side
                0.0f, 0.8f,  0.0f,     0.92f, 0.86f, 0.76f,	    0.5f, 1.0f,      0.8f, 0.5f,  0.0f, // Right side

                0.5f, 0.0f,  0.5f,     0.83f, 0.70f, 0.44f,	    0.0f, 0.0f,      0.0f, 0.5f,  0.8f, // Facing side
                -0.5f, 0.0f,  0.5f,    0.83f, 0.70f, 0.44f,     1.0f, 0.0f,      0.0f, 0.5f,  0.8f, // Facing side
                0.0f, 0.8f,  0.0f,     0.92f, 0.86f, 0.76f,	    0.5f, 1.0f,      0.0f, 0.5f,  0.8f  // Facing side
        };

// Indices for pyramidVertices order
static GLuint indices[] =
        {
                0, 1, 2, // Bottom side
                0, 2, 3, // Bottom side
                4, 6, 5, // Left side
                7, 9, 8, // Non-facing side
                10, 12, 11, // Right side
                13, 15, 14 // Facing side
        };

static GLfloat lightVertices[] =
        { //     COORDINATES     //
                -0.1f, -0.1f,  0.1f,
                -0.1f, -0.1f, -0.1f,
                0.1f, -0.1f, -0.1f,
                0.1f, -0.1f,  0.1f,
                -0.1f,  0.1f,  0.1f,
                -0.1f,  0.1f, -0.1f,
                0.1f,  0.1f, -0.1f,
                0.1f,  0.1f,  0.1f
        };

static GLuint lightIndices[] =
        {
                0, 1, 2,
                0, 2, 3,
                0, 4, 7,
                0, 7, 3,
                3, 7, 6,
                3, 6, 2,
                2, 6, 5,
                2, 5, 1,
                1, 5, 4,
                1, 4, 0,
                4, 5, 6,
                4, 6, 7
        };

static void glfwErrorCallback(int error, const char* description){
    spdlog::error("Glfw Error {}: {}\n", error, description);
}

static void windowResizeCallback(GLFWwindow* window, int newWidth, int newHeight){
//    glfwGetFramebufferSize(window, &newWidth, &newHeight);
//    float newAspect = (float)newWidth / (float)newHeight;
//    perspectiveMatrix = glm::perspective(1.0472f, newAspect, 0.1f, 1000.0f);
//    glViewport(0, 0, newWidth, newHeight);
    spdlog::info("window resized");
}

static void framebufferSizeCallback(GLFWwindow* window, int width, int height){
    glViewport(0, 0, width, height);
}

static void windowMoveCallback(GLFWwindow* window, int newWidth, int newHeight){
    spdlog::info("window moved");
}

void glslSetup(){
    // Decide GL+GLSL versions
#if defined(IMGUI_IMPL_OPENGL_ES2)
    // GL ES 2.0 + GLSL 100
        const char* glsl_version = "#version 100";
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
        glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API);
#elif defined(__APPLE__)
    // GL 3.2 + GLSL 150
    GLSL_VERSION = "#version 410";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+ only
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            // Required on Mac
#else
    // GL 3.0 + GLSL 130
        const char* glsl_version = "#version 130";
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
        //glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+ only
        //glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            // 3.0+ only
#endif
}

GLuint initFrameBuffer(){
    glGenFramebuffers(1, &frameBufferObject);
    glBindFramebuffer(GL_FRAMEBUFFER, frameBufferObject);
    // generate texture
    glGenTextures(1, &textureColorBuffer);
    glBindTexture(GL_TEXTURE_2D, textureColorBuffer);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, frameBufferSize.x, frameBufferSize.y, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, 0);

    // attach it to currently bound framebuffer object
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureColorBuffer, 0);

    glGenRenderbuffers(1, &renderBufferObject);
    glBindRenderbuffer(GL_RENDERBUFFER, renderBufferObject);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, frameBufferSize.x, frameBufferSize.y);
    glBindRenderbuffer(GL_RENDERBUFFER, 0);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, renderBufferObject);

    if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        spdlog::error("ERROR::FRAMEBUFFER:: Framebuffer is not complete!");
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    return frameBufferObject;
}

void display(auto &window, auto &camera, auto &shaderProgram, auto &texture, auto &VAO1, auto &lightShader, auto &lightVAO, bool isWindowFocused){
    // Specify the color of the background
    glClearColor(0.05f, 0.05f, 0.05f, 0.3f);
    // Clean the back buffer and depth buffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Handles camera inputs
    if (isWindowFocused) {
        camera.Inputs(window, glWindowPosition, glWindowSize);
    }
    // Updates and exports the camera matrix to the Vertex Shader
    camera.updateMatrix(45.0f, 0.2f, 100.0f);

    // Tells OpenGL which Shader Program we want to use
    shaderProgram.Activate();
    // Exports the camera Position to the Fragment Shader for specular lighting
    glUniform3f(glGetUniformLocation(shaderProgram.ID, "camPos"), camera.Position.x, camera.Position.y, camera.Position.z);
    // Export the camMatrix to the Vertex Shader of the pyramid
    camera.Matrix(shaderProgram, "camMatrix");
    // Binds texture so that is appears in rendering
    texture.Bind();
    // Bind the VAO so OpenGL knows to use it
    VAO1.Bind();
    // Draw primitives, number of indices, datatype of indices, index of indices
    glDrawElements(GL_TRIANGLES, sizeof(indices) / sizeof(int), GL_UNSIGNED_INT, 0);

    // Tells OpenGL which Shader Program we want to use
    lightShader.Activate();
    // Export the camMatrix to the Vertex Shader of the light cube
    camera.Matrix(lightShader, "camMatrix");
    // Bind the VAO so OpenGL knows to use it
    lightVAO.Bind();
    // Draw primitives, number of indices, datatype of indices, index of indices
    glDrawElements(GL_TRIANGLES, sizeof(lightIndices) / sizeof(int), GL_UNSIGNED_INT, 0);
}

int main(){
    try {
        // Initialize GLFW
        // Setup window
        glfwSetErrorCallback(glfwErrorCallback);
        if (!glfwInit()){
            spdlog::error("glfwInit");
            return 1;
        }

        glslSetup();

        // Create window with graphics context
        GLFWmonitor* monitor =  glfwGetPrimaryMonitor(); // The primary monitor.. Later Occulus?..
        const GLFWvidmode* mode = glfwGetVideoMode(monitor);

        mainWindowWidth = mode->width;
        mainWindowHeight = mode->height;

        GLFWwindow* window = glfwCreateWindow(mainWindowWidth,mainWindowHeight, "glfw-opengl-imgui", monitor, nullptr);
        if (window == nullptr) {
            spdlog::error("glfwCreateWindow");
            return 1;
        }

        glfwMakeContextCurrent(window);
//        glfwSetScrollCallback(window, scroll_callback);
        glfwSetWindowSizeCallback(window, windowResizeCallback);
        glfwSetWindowPosCallback(window, windowMoveCallback);
        glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);

        glfwSwapInterval(1); // Enable vsync

        // Introduce the window into the current context
        glfwMakeContextCurrent(window);
        //Load GLEW so it configures OpenGL
        if (glewInit() != GLEW_OK) {
            spdlog::error("glewInit");
            return 0;
        }

        // Generates Shader object using shaders default.vert and default.frag
        Shader shaderProgram("../src/include/default.vert", "../src/include/default.frag");
        // Generates Vertex Array Object and binds it
        VAO VAO1;
        VAO1.Bind();
        // Generates Vertex Buffer Object and links it to pyramidVertices
        VBO VBO1(pyramidVertices, sizeof(pyramidVertices));
        // Generates Element Buffer Object and links it to indices
        EBO EBO1(indices, sizeof(indices));
        // Links VBO attributes such as coordinates and colors to VAO
        VAO1.LinkAttrib(VBO1, 0, 3, GL_FLOAT, 11 * sizeof(float), (void *) 0);
        VAO1.LinkAttrib(VBO1, 1, 3, GL_FLOAT, 11 * sizeof(float), (void *) (3 * sizeof(float)));
        VAO1.LinkAttrib(VBO1, 2, 2, GL_FLOAT, 11 * sizeof(float), (void *) (6 * sizeof(float)));
        VAO1.LinkAttrib(VBO1, 3, 3, GL_FLOAT, 11 * sizeof(float), (void *) (8 * sizeof(float)));
        // Unbind all to prevent accidentally modifying them
        VAO1.Unbind();
        VBO1.Unbind();
        EBO1.Unbind();

        // Shader for light cube
        Shader lightShader("../src/include/light.vert", "../src/include/light.frag");
        // Generates Vertex Array Object and binds it
        VAO lightVAO;
        lightVAO.Bind();
        // Generates Vertex Buffer Object and links it to lightVertices
        VBO lightVBO(lightVertices, sizeof(lightVertices));
        // Generates Element Buffer Object and links it to indices
        EBO lightEBO(lightIndices, sizeof(lightIndices));
        // Links VBO attributes such as coordinates and colors to VAO
        lightVAO.LinkAttrib(lightVBO, 0, 3, GL_FLOAT, 3 * sizeof(float), (void *) 0);
        // Unbind all to prevent accidentally modifying them
        lightVAO.Unbind();
        lightVBO.Unbind();
        lightEBO.Unbind();

        glm::vec4 lightColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
        glm::vec3 lightPos = glm::vec3(0.5f, 0.5f, 0.5f);
        glm::mat4 lightModel = glm::mat4(1.0f);
        lightModel = glm::translate(lightModel, lightPos);

        glm::vec3 pyramidPos = glm::vec3(0.0f, 0.0f, 0.0f);
        glm::mat4 pyramidModel = glm::mat4(1.0f);
        pyramidModel = glm::translate(pyramidModel, pyramidPos);

        glm::vec3 pyramidPos2 = glm::vec3(2.0f, 1.0f, 0.0f);
        glm::mat4 pyramidModel2 = glm::mat4(1.0f);
        pyramidModel2 = glm::translate(pyramidModel2, pyramidPos2);

        lightShader.Activate();
        glUniformMatrix4fv(glGetUniformLocation(lightShader.ID, "model"), 1, GL_FALSE, glm::value_ptr(lightModel));
        glUniform4f(glGetUniformLocation(lightShader.ID, "lightColor"), lightColor.x, lightColor.y, lightColor.z, lightColor.w);
        shaderProgram.Activate();
        glUniformMatrix4fv(glGetUniformLocation(shaderProgram.ID, "model"), 1, GL_FALSE, glm::value_ptr(pyramidModel));
//        glUniformMatrix4fv(glGetUniformLocation(shaderProgram.ID, "model"), 1, GL_FALSE, glm::value_ptr(pyramidModel2));
        glUniform4f(glGetUniformLocation(shaderProgram.ID, "lightColor"), lightColor.x, lightColor.y, lightColor.z, lightColor.w);
        glUniform3f(glGetUniformLocation(shaderProgram.ID, "lightPos"), lightPos.x, lightPos.y, lightPos.z);

        // Texture
        Texture firstTexture("../src/include/sample_texture.png", GL_TEXTURE_2D, GL_TEXTURE0, GL_RGBA, GL_UNSIGNED_BYTE);
        firstTexture.texUnit(shaderProgram, "tex0", 0);

        // Enables the Depth Buffer
        glEnable(GL_DEPTH_TEST);
        // Creates camera object
        Camera camera(glWindowSize.x, glWindowSize.y, glm::vec3(-2.140f, 0.6, 5.649f));
        camera.resetView();

        // Setup Dear ImGui context
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO(); (void)io;
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
        io.ConfigWindowsMoveFromTitleBarOnly = true;

        // Setup Dear ImGui style
        ImGui::StyleColorsDark();

        // Setup Platform/Renderer backends
        ImGui_ImplGlfw_InitForOpenGL(window, true);
        ImGui_ImplOpenGL3_Init(GLSL_VERSION);

        // Our state
        bool show_demo_window = false;
        auto mainBackgroundColor = ImVec4(0.01f, 0.01f, 0.01f, 1.00f);

        initFrameBuffer();
        double lastTime = glfwGetTime();

        // Main while loop
        while (!glfwWindowShouldClose(window)) {
            glfwPollEvents();

            // Start the Dear ImGui frame
            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplGlfw_NewFrame();
            ImGui::NewFrame();

            if (ImGui::BeginMainMenuBar()){
                if (ImGui::BeginMenu("File")){
                    if (ImGui::MenuItem("Open")){
                        //Do something
                    }
                    if (ImGui::MenuItem("Exit")){
                        // Delete all the objects we've created
                        VAO1.Delete();
                        VBO1.Delete();
                        EBO1.Delete();
                        firstTexture.Delete();
                        shaderProgram.Delete();
                        lightVAO.Delete();
                        lightVBO.Delete();
                        lightEBO.Delete();
                        lightShader.Delete();

                        ImGui_ImplOpenGL3_Shutdown();
                        ImGui_ImplGlfw_Shutdown();
                        ImGui::DestroyContext();

                        // Delete window before ending the program
                        glfwDestroyWindow(window);
                        // Terminate GLFW before ending the program
                        glfwTerminate();
                        return 0;

                    }
                    ImGui::EndMenu();
                }
                if (ImGui::BeginMenu("Graph")){
                    if (ImGui::MenuItem("Something")){
                        //Do something
                    }
                    ImGui::EndMenu();
                }
                ImGui::EndMainMenuBar();
            }

            // Stats window
            ImGui::SetNextWindowPos( ImVec2(0,0), ImGuiCond_Once);
            ImGui::SetNextWindowSize(ImVec2(250, 70), ImGuiCond_Always);
            ImGui::Begin("Stats");
            ImGui::Text(" %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
            // Stores the coordinates of the cursor
            double mouseX;
            double mouseY;
            // Fetches the coordinates of the cursor
            glfwGetCursorPos(window, &mouseX, &mouseY);
            ImGui::Text(" %.1f x %.1f", mouseX, mouseY);
            ImGui::End();

            // 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).
            if (show_demo_window) {
                ImGui::ShowDemoWindow(&show_demo_window);
            }

            // Camera window
            ImGui::SetNextWindowPos( ImVec2(0,90), ImGuiCond_Once);
            ImGui::SetNextWindowSize(ImVec2(250, 380), ImGuiCond_Always);
            ImGui::Begin("Camera");
            ImGui::SliderInt("Width", &camera.width, 0, mainWindowHeight);
            ImGui::SliderInt("Height", &camera.height, 0, mainWindowHeight);
            ImGui::SliderFloat("Orient-X", &camera.Orientation.x, -10, 10);
            ImGui::SliderFloat("Orient-Y", &camera.Orientation.y, -10, 10);
            ImGui::SliderFloat("Orient-Z", &camera.Orientation.z, -10, 10);
            ImGui::SliderFloat("Pos-X", &camera.Position.x, -10, 10);
            ImGui::SliderFloat("Pos-Y", &camera.Position.y, 0, 10);
            ImGui::SliderFloat("Pos-Z", &camera.Position.z, -10, 10);
            ImGui::SliderFloat("Up-X", &camera.Up.x, -10, 10);
            ImGui::SliderFloat("Up-Y", &camera.Up.y, -10, 10);
            ImGui::SliderFloat("Up-Z", &camera.Up.z, -10, 10);
            ImGui::SliderFloat("Speed", &camera.speed, 0, 1);
            ImGui::SliderFloat("Sense", &camera.sensitivity, 0, 2);
            if (ImGui::Button("User / World")){
                //... my_code
            }
            if (ImGui::Button("Reset view")){
                camera.resetView();
            }
            ImGui::End();

            // Light window
            ImGui::SetNextWindowPos( ImVec2(0,470), ImGuiCond_Once);
            ImGui::SetNextWindowSize(ImVec2(250, 250), ImGuiCond_Always);
            ImGui::Begin("Light");
            ImGui::SliderFloat("tx1", &tx1, -1, 1);
            ImGui::SliderFloat("tx2", &tx2, -1, 1);
            ImGui::SliderFloat("tx3", &tx3, -1, 1);
            ImGui::SliderFloat("tx4", &tx4, -1, 1);
            ImGui::SliderFloat("tx5", &tx5, -1, 1);
            ImGui::SliderFloat("tx6", &tx6, -1, 1);
            ImGui::End();

            // Map window
            ImGui::SetNextWindowPos( ImVec2(0,720), ImGuiCond_Once);
            ImGui::SetNextWindowSize(ImVec2(250, mainWindowHeight - 720), ImGuiCond_Always);
            ImGui::Begin("Map");
            ImGui::End();

            // OpenGL window
            ImGui::SetNextWindowPos( ImVec2(250,0), ImGuiCond_Once);
            ImGui::SetNextWindowSize(ImVec2(mainWindowWidth - 250, mainWindowHeight - 19), ImGuiCond_Once);
            ImGui::Begin("OpenGL");   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
            ImGui::BeginChild("Render");
            glWindowSize = ImGui::GetContentRegionAvail();
            glWindowPosition = ImGui::GetWindowPos();
            frameBufferSize.x = glWindowSize.x;
            frameBufferSize.y = glWindowSize.y;
            // Specify the viewport of OpenGL in the Window
            // In this case the viewport goes from x = 0, y = 0, to x = 800, y = 800
//             glViewport(0, 0, glWindowSize.x, glWindowSize.y);
            try {
                glBindTexture(GL_TEXTURE_2D, textureColorBuffer);
                glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, frameBufferSize.x, frameBufferSize.y, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
                glBindTexture(GL_TEXTURE_2D, 0);

                glBindRenderbuffer(GL_RENDERBUFFER, renderBufferObject);
                glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, frameBufferSize.x, frameBufferSize.y);
                glBindRenderbuffer(GL_RENDERBUFFER, 0);
                glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, renderBufferObject);

                glClearColor( 0.0f , 0.0f , 0.0f , 1.0f );
                glClear(GL_FRAMEBUFFER);
                glBindFramebuffer(GL_FRAMEBUFFER, frameBufferObject);

                display(window, camera, shaderProgram, firstTexture, VAO1, lightShader, lightVAO, ImGui::IsWindowFocused());

                glBindFramebuffer(GL_FRAMEBUFFER, 0);
                // TODO: Hard-coded texture ID
                ImGui::Image(reinterpret_cast<ImTextureID>(2), frameBufferSize, ImVec2(0, 1), ImVec2(1, 0));
            }
            catch (std::exception &e){
                spdlog::error("ImGui::Image: {}", e.what());
            }

            ImGui::EndChild();
            ImGui::End();

            // Rendering
            ImGui::Render();
            glClearColor(mainBackgroundColor.x, mainBackgroundColor.y, mainBackgroundColor.z, 1);
            glClear(GL_COLOR_BUFFER_BIT);
            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

            glfwSwapBuffers(window);

            // Limit to n FPS
            while (glfwGetTime() < lastTime + 1.0 / FPS) {
//                pthread_yield_np();
            }
            lastTime += 1.0 / FPS;
        }

        // Delete all the objects we've created
        VAO1.Delete();
        VBO1.Delete();
        EBO1.Delete();
        firstTexture.Delete();
        shaderProgram.Delete();
        lightVAO.Delete();
        lightVBO.Delete();
        lightEBO.Delete();
        lightShader.Delete();

        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();

        // Delete window before ending the program
        glfwDestroyWindow(window);
        // Terminate GLFW before ending the program
        glfwTerminate();
        return 0;
    }
    catch (std::exception &e){
        spdlog::error(e.what());
    }
}
