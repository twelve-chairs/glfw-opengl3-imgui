// Dear ImGui: standalone example application for GLFW + OpenGL 3, using programmable pipeline
// (GLFW is a cross-platform general purpose library for handling windows, inputs, OpenGL/Vulkan/Metal graphics context creation, etc.)
// If you are new to Dear ImGui, read documentation from the docs/ folder + read the top of imgui.cpp.
// Read online: https://github.com/ocornut/imgui/tree/master/docs

#include "main.h"


static void glfwErrorCallback(int error, const char* description)
{
    spdlog::error("Glfw Error {}: {}\n", error, description);
}

static void windowResizeCallback(GLFWwindow* window, int newWidth, int newHeight){
    glfwGetFramebufferSize(window, &newWidth, &newHeight);
    float newAspect = (float)newWidth / (float)newHeight;
    perspectiveMatrix = glm::perspective(1.0472f, newAspect, 0.1f, 1000.0f);
}

void setupVertices(){
    const float cubePositions[108] = {
            -1.0f, 1.0f, -1.0f, -1.0f, -1.0f, -1.0f, 1.0f, -1.0f, -1.0f,
            1.0f, -1.0f, -1.0f, 1.0f, 1.0f, -1.0f, -1.0f, 1.0f, -1.0f,
            1.0f, -1.0f, -1.0f, 1.0f, -1.0f, 1.0f, 1.0f, 1.0f, -1.0f,
            1.0f, -1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, -1.0f,
            1.0f, -1.0f, 1.0f, -1.0f, -1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
            -1.0f, -1.0f, 1.0f, -1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
            -1.0f, -1.0f, 1.0f, -1.0f, -1.0f, -1.0f, -1.0f, 1.0f, 1.0f,
            -1.0f, -1.0f, -1.0f, -1.0f, 1.0f, -1.0f, -1.0f, 1.0f, 1.0f,
            -1.0f, -1.0f, 1.0f, 1.0f, -1.0f, 1.0f, 1.0f, -1.0f, -1.0f,
            1.0f, -1.0f, -1.0f, -1.0f, -1.0f, -1.0f, -1.0f, -1.0f, 1.0f,
            -1.0f, 1.0f, -1.0f, 1.0f, 1.0f, -1.0f, 1.0f, 1.0f, 1.0f,
            1.0f, 1.0f, 1.0f, -1.0f, 1.0f, 1.0f, -1.0f, 1.0f, -1.0f
    };
    // pyramid with 18 vertices, comprising 6 triangles (four sides, and two on the bottom)
    const float pyramidPositions[54] = {
            -1.0f, -1.0f, 1.0f, 1.0f, -1.0f, 1.0f, 0.0f, 1.0f, 0.0f, // front face
            1.0f, -1.0f, 1.0f, 1.0f, -1.0f, -1.0f, 0.0f, 1.0f, 0.0f, // right face
            1.0f, -1.0f, -1.0f, -1.0f, -1.0f, -1.0f, 0.0f, 1.0f, 0.0f, // back face
            -1.0f, -1.0f, -1.0f, -1.0f, -1.0f, 1.0f, 0.0f, 1.0f, 0.0f, // left face
            -1.0f, -1.0f, -1.0f, 1.0f, -1.0f, 1.0f, -1.0f, -1.0f, 1.0f, // base – left front
            1.0f, -1.0f, 1.0f, -1.0f, -1.0f, -1.0f, 1.0f, -1.0f, -1.0f // base – right back
    };
    float PLANE_POSITIONS[18] = {
            -128.0f, 0.0f, -128.0f, -128.0f, 0.0f, 128.0f, 128.0f, 0.0f, -128.0f,
            128.0f, 0.0f, -128.0f, -128.0f, 0.0f, 128.0f, 128.0f, 0.0f, 128.0f
    };
    float PLANE_TEXCOORDS[12] = {
            0.0f, 0.0f, 0.0f,
            1.0f, 1.0f, 0.0f,
            1.0f, 0.0f, 0.0f,
            1.0f, 1.0f, 1.0f
    };
    glGenVertexArrays(1, vertexArrayObjects);
    glBindVertexArray(vertexArrayObjects[0]);
    glGenBuffers(numVBOs, vertexBuffersObject);

    // Cube
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffersObject[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cubePositions), cubePositions, GL_STATIC_DRAW);

    // Pyramid
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffersObject[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(pyramidPositions), pyramidPositions, GL_STATIC_DRAW);

    spdlog::info("setupVertices");
}

GLuint createShaderProgram(){
    std::string vertexShaderString = readShaderSource("../src/include/vertexShader.glsl");
    std::string fragmentShaderString = readShaderSource("../src/include/fragmentShader.glsl");
    const char* vertexShaderSource = vertexShaderString.c_str();
    const char* fragmentShaderSource = fragmentShaderString.c_str();

    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);

    GLint vertexCompiled;
    GLint fragmentCompiled;
    GLint linked;

    glCompileShader(vertexShader);
    checkOpenGLError();
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &vertexCompiled);
    if (vertexCompiled != 1){
        spdlog::error("vertexCompiled: Error");
        printShaderLog(vertexShader);
    }

    glCompileShader(fragmentShader);
    checkOpenGLError();
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &fragmentCompiled);
    if (fragmentCompiled != 1){
        spdlog::error("fragmentCompiled: Error");
        printShaderLog(fragmentShader);
    }

    GLuint vfProgram = glCreateProgram();

    glAttachShader(vfProgram, vertexShader);
    glAttachShader(vfProgram, fragmentShader);

    glLinkProgram(vfProgram);
    checkOpenGLError();
    glGetProgramiv(vfProgram, GL_LINK_STATUS, &linked);
    if (linked != 1){
        spdlog::error("linked: error");
        printProgramLog(vfProgram);
    }

    if (checkOpenGLError()){
        printShaderLog(vertexShader);
        printProgramLog(vfProgram);
    }

    return vfProgram;
}

void initFrameBuffer(){
    glGenFramebuffers(1, &frameBufferObject);
    glBindFramebuffer(GL_FRAMEBUFFER, frameBufferObject);
    // generate texture
    unsigned int textureColorbuffer;
    glGenTextures(1, &textureColorbuffer);
    glBindTexture(GL_TEXTURE_2D, textureColorbuffer);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 800, 600, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, 0);

    // attach it to currently bound framebuffer object
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureColorbuffer, 0);

    glGenRenderbuffers(1, &renderBufferObject);
    glBindRenderbuffer(GL_RENDERBUFFER, renderBufferObject);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, 800, 600);
    glBindRenderbuffer(GL_RENDERBUFFER, 0);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, renderBufferObject);

    if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        spdlog::error("ERROR::FRAMEBUFFER:: Framebuffer is not complete!");
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void init(GLFWwindow* window){
    renderingProgram = createShaderProgram();
    cameraPositionX = 4.4f;
    cameraPositionY = 2.7f;
    cameraPositionZ = 10.0f;
    cubeLocationX = 0.667f;
    cubeLocationY = 2.0f;
    cubeLocationZ = 0.0f;
    pyramidLocationX = 1.473f;
    pyramidLocationY = 0.63f;
    pyramidLocationZ = 1.56f;
    setupVertices();
    initFrameBuffer();
    glfwGetFramebufferSize(window, &width, &height);
    aspect = (float)width / (float)height;
    perspectiveMatrix = glm::perspective(1.0472f, aspect, 0.1f, 1000.0f);
    spdlog::info("init");
}

void display(GLFWwindow* window, double currentTime){

    glClearColor( 0.0f , 0.0f , 0.0f , 1.0f );
    glClear(GL_FRAMEBUFFER);
    glBindFramebuffer(GL_FRAMEBUFFER, frameBufferObject);

    glClear(GL_DEPTH_BUFFER_BIT);
    glClear(GL_COLOR_BUFFER_BIT);
    glUseProgram(renderingProgram);

    viewLocation = glGetUniformLocation(renderingProgram, "v_matrix");
    modelViewMatrixLocation = glGetUniformLocation(renderingProgram, "mv_matrix");
    projectionLocation = glGetUniformLocation(renderingProgram, "proj_matrix");
    timeFrameLocation = glGetUniformLocation(renderingProgram, "tf");

//    (cameraPositionX < 1000) ? cameraPositionX += 0.1f : cameraPositionX = 0;
//    (cameraPositionY < 1000) ? cameraPositionY += 0.2f : cameraPositionY = 0;
//    (cameraPositionZ < 1300) ? cameraPositionZ += 0.3f : cameraPositionZ = 0;

    // the view matrix is computed once and used for both objects
    viewMatrix = glm::translate(
            glm::mat4(1.0f), glm::vec3(0.0f, -cameraHeight, 0.0f))
            *
            glm::rotate(glm::mat4(1.0f), glm::radians(cameraPitch), glm::vec3(1.0f, 0.0f, 0.0f));

    // draw the cube (use buffer #0)
    modelMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(0, planeHeight, 0));
    modelViewMatrix = viewMatrix * modelMatrix;

    glUniformMatrix4fv(modelViewMatrixLocation, 1, GL_FALSE, glm::value_ptr(modelViewMatrix));
    glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, glm::value_ptr(perspectiveMatrix));

    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffersObject[0]);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
    glEnableVertexAttribArray(0);

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glDrawArrays(GL_TRIANGLES, 0, 6);

    // draw the pyramid (use buffer #1)
    modelMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(pyramidLocationX, pyramidLocationY, pyramidLocationZ));
    modelViewMatrix = viewMatrix * modelMatrix;

    glUniformMatrix4fv(modelViewMatrixLocation, 1, GL_FALSE, glm::value_ptr(modelViewMatrix));
    glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, glm::value_ptr(perspectiveMatrix));

    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffersObject[1]);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
    glEnableVertexAttribArray(0);

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glDrawArrays(GL_TRIANGLES, 0, 18);

    // Animated pyramids
//    modelMatrix = translationMatrix * rotationMatrix;
//    glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, glm::value_ptr(perspectiveMatrix));
//
//    glUniformMatrix4fv(viewLocation, 1, GL_FALSE, glm::value_ptr(viewMatrix));
//    auto timeFactor = ((float)currentTime);
//    glUniform1f(timeFrameLocation, (float)timeFactor);
//
////    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffersObject[randomInteger(1)]);
//    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
//    glEnableVertexAttribArray(0);
//
//    glEnable(GL_DEPTH_TEST);
//    glDepthFunc(GL_LEQUAL);
//    glDrawArraysInstanced(GL_TRIANGLES, 0, 36, 100000);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

int main(int, char**){
    // Setup window
    glfwSetErrorCallback(glfwErrorCallback);
    if (!glfwInit()){
        spdlog::error("glfwInit");
        return 1;
    }

    {
        // Decide GL+GLSL versions
#if defined(IMGUI_IMPL_OPENGL_ES2)
        // GL ES 2.0 + GLSL 100
        const char* glsl_version = "#version 100";
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
        glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API);
#elif defined(__APPLE__)
        // GL 3.2 + GLSL 150
        glsl_version = "#version 150";
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
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

    // Create window with graphics context
    GLFWwindow* window = glfwCreateWindow(1300, 900, "glfw-opengl3-imgui", nullptr, nullptr);
    if (window == nullptr) {
        spdlog::error("glfwCreateWindow");
        return 1;
    }
    glfwMakeContextCurrent(window);

    if (glewInit() != GLEW_OK){
        spdlog::error("glewInit");
        return 1;
    }

    glfwSwapInterval(1); // Enable vsync

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

    // Setup Dear ImGui style
    ImGui::StyleColorsLight();

    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);

    // // Load Fonts
    // - If no fonts are loaded, dear imgui will use the default font. You can also load multiple fonts and use ImGui::PushFont()/PopFont() to select them.
    // - AddFontFromFileTTF() will return the ImFont* so you can store it if you need to select the font among multiple.
    // - If the file cannot be loaded, the function will return NULL. Please handle those errors in your application (e.g. use an assertion, or display an error and quit).
    // - The fonts will be rasterized at a given size (w/ oversampling) and stored into a texture when calling ImFontAtlas::Build()/GetTexDataAsXXXX(), which ImGui_ImplXXXX_NewFrame below will call.
    // - Read 'docs/FONTS.md' for more instructions and details.
    // - Remember that in C/C++ if you want to include a backslash \ in a string literal you need to write a double backslash \\ !
    //io.Fonts->AddFontDefault();
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/Roboto-Medium.ttf", 16.0f);
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/Cousine-Regular.ttf", 15.0f);
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/DroidSans.ttf", 16.0f);
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/ProggyTiny.ttf", 10.0f);
    //ImFont* font = io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\ArialUni.ttf", 18.0f, NULL, io.Fonts->GetGlyphRangesJapanese());
    //IM_ASSERT(font != NULL);

    // Our state
    bool show_demo_window = false;
    bool show_another_window = false;
    auto mainBackgroundColor = ImVec4(0.32f, 0.56f, 0.86f, 1.00f);

    glfwSetWindowSizeCallback(window, windowResizeCallback);
    init(window);

    // Main loop
    while (!glfwWindowShouldClose(window)){
        // Poll and handle events (inputs, window resize, etc.)
        // You can read the io.WantCaptureMouse, io.WantCaptureKeyboard flags to tell if dear imgui wants to use your inputs.
        // - When io.WantCaptureMouse is true, do not dispatch mouse input data to your main application.
        // - When io.WantCaptureKeyboard is true, do not dispatch keyboard input data to your main application.
        // Generally you may always pass all inputs to dear imgui, and hide them from your application based on those two flags.
        glfwPollEvents();


        // Start the Dear ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        // 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).
        if (show_demo_window) {
            ImGui::ShowDemoWindow(&show_demo_window);
        }

        // OpenGL window
        ImGui::SetNextWindowPos( ImVec2(220,0), ImGuiCond_Once);
        ImGui::SetNextWindowSize(ImVec2(1060, 860), ImGuiCond_Once);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
        ImGui::Begin("OpenGL");   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
        ImGui::PopStyleVar();
        // Using a Child allow to fill all the space of the window.
        // It also allows customization
        ImGui::BeginChild("Render");
        // Get the size of the child (i.e. the whole draw size of the windows).
        ImVec2 wsize = ImGui::GetWindowSize();
        try {
            display(window, glfwGetTime());
            ImGui::Image(reinterpret_cast<ImTextureID>(frameBufferObject), wsize, ImVec2(0, 1), ImVec2(1, 0));
        }
        catch (std::exception &e){
            spdlog::error("ImGui::Image: {}", e.what());
        }

        ImGui::EndChild();
        ImGui::End();


        // Stats window
        ImGui::SetNextWindowPos( ImVec2(0,0), ImGuiCond_Once);
        ImGui::SetNextWindowSize(ImVec2(220, 50), ImGuiCond_Always);
        ImGui::Begin("Stats");
        ImGui::Text(" %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
        ImGui::End();

        // Camera window
        ImGui::SetNextWindowPos( ImVec2(0,50), ImGuiCond_Once);
        ImGui::SetNextWindowSize(ImVec2(220, 160), ImGuiCond_Always);
        ImGui::Begin("Camera");
        ImGui::SliderFloat("X", &cameraPositionX, -10, 10);
        ImGui::SliderFloat("Y", &cameraPositionY, -10, 10);
        ImGui::SliderFloat("Z", &cameraPositionZ, -10, 10);
        ImGui::SliderFloat("Height", &cameraHeight, -2, 2);
        ImGui::SliderFloat("Pitch", &cameraPitch, -90, 90);
        ImGui::SliderFloat("Aspect", &aspect, -10, 10);
        ImGui::End();

//        // Cube window
//        ImGui::SetNextWindowPos( ImVec2(0,160), ImGuiCond_Once);
//        ImGui::SetNextWindowSize(ImVec2(220, 110), ImGuiCond_Always);
//        ImGui::Begin("Cube");
//        ImGui::SliderFloat("X", &cubeLocationX, -2, 2);
//        ImGui::SliderFloat("Y", &cubeLocationY, -2, 2);
//        ImGui::SliderFloat("Z", &cubeLocationZ, -2, 2);
//        ImGui::End();
//
//        // Pyramid window
//        ImGui::SetNextWindowPos( ImVec2(0,270), ImGuiCond_Once);
//        ImGui::SetNextWindowSize(ImVec2(220, 110), ImGuiCond_Always);
//        ImGui::Begin("Pyramid");
//        ImGui::SliderFloat("X", &pyramidLocationX, -2, 2);
//        ImGui::SliderFloat("Y", &pyramidLocationY, -2, 2);
//        ImGui::SliderFloat("Z", &pyramidLocationZ, -2, 2);
//        ImGui::End();

        // Rendering
        ImGui::Render();

        int display_w;
        int display_h;
        glfwGetFramebufferSize(window, &display_w, &display_h);
        glViewport(0, 3, display_w, display_h);
        glClearColor(mainBackgroundColor.x, mainBackgroundColor.y, mainBackgroundColor.z, 1);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window);
    }


    // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}
