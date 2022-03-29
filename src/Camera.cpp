#include"Camera.h"

glm::vec2 lastMousePosition = glm::vec2(0.0f, 0.0f);
float lastMouseUpdate = 0.0f;
float maxAngle = 90.0f;

Camera::Camera(int width, int height, glm::vec3 position){
    Camera::width = width;
    Camera::height = height;
    Position = position;
}

void Camera::updateMatrix(float FOVdeg, float nearPlane, float farPlane){
    // Initializes matrices since otherwise they will be the null matrix
    glm::mat4 view = glm::mat4(1.0f);
    glm::mat4 projection = glm::mat4(1.0f);

    // Makes camera look in the right direction from the right position
    view = glm::lookAt(Position, Position + Orientation, Up);
    // Adds perspective to the scene
    projection = glm::perspective(glm::radians(FOVdeg), (float)width / height, nearPlane, farPlane);

    // Sets new camera matrix
    cameraMatrix = projection * view;
}


void Camera::Matrix(Shader& shader, const char* uniform){
    // Exports camera matrix
    glUniformMatrix4fv(glGetUniformLocation(shader.ID, uniform), 1, GL_FALSE, glm::value_ptr(cameraMatrix));
}

void Camera::scrollCallback(){}

void Camera::relevelView(){
    if (Position.y < 0.2 || Position.y > 0.2){
        Position.y = 0.2;
    }
    if (Orientation.y < 0.05){
        Orientation.y = 0.05;
    }
}

void Camera::resetView(){
    Orientation = glm::vec3(0.532, 0.194f, -0.947f);
    Position = glm::vec3(-0.931f, 0.2f, 4.174f);
}


void Camera::Inputs(GLFWwindow* window, ImVec2 glWindowPosition, ImVec2 glWindowSize){
        ImVec4 glWindowCoordinates = ImVec4(glWindowPosition.x, glWindowPosition.y, glWindowPosition.x + glWindowSize.x,
                                            glWindowPosition.y + glWindowSize.y);
        // Handle key inputs

        // Forward
        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
            Position += speed * Orientation;
        }
        // Left
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
            Position += speed * -glm::normalize(glm::cross(Orientation, Up));
        }
        // Back
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
            Position += speed * -Orientation;
        }
        // Right
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
            Position += speed * glm::normalize(glm::cross(Orientation, Up));
        }

        // Handles mouse inputs
    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS) {

    }
        double mouseX;
        double mouseY;

        float mouseAreaPadding = 400.0f;
        int mouseDelay = 20;
        glWindowCoordinates.x += mouseAreaPadding;
        glWindowCoordinates.y += mouseAreaPadding / 2;
        glWindowCoordinates.z -= mouseAreaPadding;
        glWindowCoordinates.w -= mouseAreaPadding / 2;

        glfwGetCursorPos(window, &mouseX, &mouseY);

        // TODO: this is very messy
        if (((glWindowCoordinates.x < mouseX) && (mouseX < glWindowCoordinates.z)) &&
            ((glWindowCoordinates.y < mouseY) && (mouseY < glWindowCoordinates.w))) {
            int currentTime = duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
            // Only update view if mouse position has changed, to prevent unnecessary float
            if (lastMousePosition.x != mouseX || lastMousePosition.y != mouseY) {
                if ((currentTime + mouseDelay) > (lastMouseUpdate)) {
                    lastMousePosition = glm::vec2(mouseX, mouseY);
                    lastMouseUpdate = currentTime;

                    // Hides mouse cursor
                    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

                    // Prevents camera from jumping on the first click
                    float z = (glWindowCoordinates.x + glWindowCoordinates.z) / 2;
                    float w = (glWindowCoordinates.y + glWindowCoordinates.w) / 2;
                    if (firstClick) {
//                glfwSetCursorPos(window, z, w);
                        firstClick = false;
                    }

                    // Normalizes and shifts the coordinates of the cursor such that they begin in the middle of the screen
                    // and then "transforms" them into degrees
                    float rotX = sensitivity * (float) (mouseY - (glWindowCoordinates.w / 2.5)) / glWindowCoordinates.z;
                    float rotY = sensitivity * (float) (mouseX - (glWindowCoordinates.z / 1.5)) / glWindowCoordinates.w;

                    // Calculates upcoming vertical change in the Orientation
                    glm::vec3 newOrientation = glm::rotate(Orientation, glm::radians(-rotX),
                                                           glm::normalize(glm::cross(Orientation, Up)));

                    // Decides whether the next vertical Orientation is legal or not
                    if (abs(glm::angle(newOrientation, Up) - glm::radians(90.0f)) <= glm::radians(maxAngle)) {
                        Orientation = newOrientation;
                    }

                    // Rotates the Orientation left and right
                    Orientation = glm::rotate(Orientation, glm::radians(-rotY), Up);
//                    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

                }
            }
        }
        relevelView();


//    }
//    else if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_RELEASE){
//        // Unhides cursor since camera is not looking around anymore
//        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
//        // Makes sure the next time the camera looks around it doesn't jump
//        firstClick = true;
//    }
}