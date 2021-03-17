//#include <imgui.h>
//#include "../include/imgui_impl_glfw.h"
//#include "../include/imgui_impl_opengl3.h"

//static void showHelperOverlay(bool* p_open){
//    const float PADDING = 10.0f;
//    static int corner = 3;
//    ImGuiIO& io = ImGui::GetIO();
//    ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoDecoration |
//            ImGuiWindowFlags_AlwaysAutoResize |
//            ImGuiWindowFlags_NoSavedSettings |
//            ImGuiWindowFlags_NoFocusOnAppearing |
//            ImGuiWindowFlags_NoNav;
//    if (corner != -1){
//        const ImGuiViewport* viewport = ImGui::GetMainViewport();
//        ImVec2 work_pos = viewport->WorkPos; // Use work area to avoid menu-bar/task-bar, if any!
//        ImVec2 work_size = viewport->WorkSize;
//        ImVec2 window_pos, window_pos_pivot;
//        window_pos.x = (corner & 1) ? (work_pos.x + work_size.x - PADDING) : (work_pos.x + PADDING);
//        window_pos.y = (corner & 2) ? (work_pos.y + work_size.y - PADDING) : (work_pos.y + PADDING);
//        window_pos_pivot.x = (corner & 1) ? 1.0f : 0.0f;
//        window_pos_pivot.y = (corner & 2) ? 1.0f : 0.0f;
//        ImGui::SetNextWindowPos(window_pos, ImGuiCond_Always, window_pos_pivot);
//        window_flags |= ImGuiWindowFlags_NoMove;
//    }
//
//    ImGui::SetNextWindowBgAlpha(0.85f);
//
//    if (ImGui::Begin("Debug", nullptr, window_flags)){
//        if (ImGui::IsMousePosValid()) {
//            ImGui::TextColored(ImColor(255, 255, 255),
//                               "Mouse Position: %.1f x %.1f",
//                               io.MousePos.x,
//                               io.MousePos.y);
//        }
//        else {
//            ImGui::TextColored(ImColor(255, 255, 255), "Mouse Position: <invalid>");
//        }
//
//        ImGui::Separator();
//
//        ImGui::TextColored(ImColor(255, 255, 255),
//                           "Application average %.3f ms/frame (%.1f FPS)",
//                           1000.0f / ImGui::GetIO().Framerate,
//                           ImGui::GetIO().Framerate);
//
//        if (ImGui::BeginPopupContextWindow()){
//            if (ImGui::MenuItem("Custom", nullptr, corner == -1)) corner = -1;
//            if (ImGui::MenuItem("Top-left",     nullptr, corner == 0)) corner = 0;
//            if (ImGui::MenuItem("Top-right",    nullptr, corner == 1)) corner = 1;
//            if (ImGui::MenuItem("Bottom-left",  nullptr, corner == 2)) corner = 2;
//            if (ImGui::MenuItem("Bottom-right", nullptr, corner == 3)) corner = 3;
//            if (p_open && ImGui::MenuItem("Close")) *p_open = false;
//            ImGui::EndPopup();
//        }
//    }
//    ImGui::End();
//}

//    // Setup Dear ImGui context
//    IMGUI_CHECKVERSION();
//    ImGui::CreateContext();
//    ImGuiIO& io = ImGui::GetIO();
//    (void)io;
//
//    // Setup Dear ImGui style
//    ImGui::StyleColorsClassic();
//
//    // Setup Platform/Renderer backends
//    ImGui_ImplGlfw_InitForOpenGL(window, true);
//    ImGui_ImplOpenGL3_Init();
//
//    // Our state
//    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
//
//    // Main loop
//    while (!glfwWindowShouldClose(window)){
//        glfwPollEvents();
//
//        // Start the Dear ImGui frame
//        ImGui_ImplOpenGL3_NewFrame();
//        ImGui_ImplGlfw_NewFrame();
//
//        ImGui::NewFrame();
//
//        static bool use_work_area = true;
//        static ImGuiWindowFlags flags = ImGuiWindowFlags_NoBringToFrontOnFocus |
//                ImGuiWindowFlags_NoMove |
//                ImGuiWindowFlags_NoResize |
//                ImGuiWindowFlags_NoSavedSettings |
//                ImGuiWindowFlags_NoNav |
//                ImGuiWindowFlags_NoCollapse;
//
//        const ImGuiViewport* viewport = ImGui::GetMainViewport();
//        ImGui::SetNextWindowPos(use_work_area ? viewport->WorkPos : viewport->Pos);
//        ImGui::SetNextWindowSize(use_work_area ? viewport->WorkSize : viewport->Size);
//        ImGui::Begin("OpenGL Context", &use_work_area, flags);
//
//        ImGui::End();
//
//        showHelperOverlay(&use_work_area);
//
//        // Rendering
//        ImGui::Render();
//        int display_w, display_h;
//        glfwGetFramebufferSize(window, &display_w, &display_h);
//        glViewport(0, 0, display_w, display_h);
//        glClearColor(clear_color.x * clear_color.w,
//                     clear_color.y * clear_color.w,
//                     clear_color.z * clear_color.w,
//                     clear_color.w);
//        glClear(GL_COLOR_BUFFER_BIT);
//        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
//
//        glfwSwapBuffers(window);
//    }
//
//    // Cleanup
//    ImGui_ImplOpenGL3_Shutdown();
//    ImGui_ImplGlfw_Shutdown();
//    ImGui::DestroyContext();