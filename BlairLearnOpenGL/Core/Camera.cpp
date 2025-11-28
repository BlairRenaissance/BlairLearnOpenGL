//
//  Camera.cpp
//  BlairLearnOpenGL
//
//  Created by yiwen ren on 2025/2/10.
//

#include "Camera.hpp"

Camera::Camera(glm::vec3 position, glm::vec3 up, float yawAngle, float pitchAngle) 
:frontDir(glm::vec3(0.0f, 0.0f, -1.0f)),
 movementSpeed(SPEED)
{
    yaw = yawAngle;
    pitch = pitchAngle;
    worldPosition = position;
    worldUp = up;
    UpdateCameraVectors();
}

glm::mat4 Camera::GetViewMatrix(){
    return glm::lookAt(worldPosition, worldPosition + frontDir, upDir);
}

void Camera::UpdateCameraVectors()
{
    // calculate the new Front vector
    glm::vec3 front;
    front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    front.y = sin(glm::radians(pitch));
    front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    frontDir = glm::normalize(front);
    // normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
    rightDir = glm::normalize(glm::cross(frontDir, worldUp));
    upDir    = glm::normalize(glm::cross(rightDir, frontDir));
}

void Camera::ProcessMouseScroll(float yoffset)
{
    fov -= (float)yoffset;
    if (fov < 1.0f)
        fov = 1.0f;
    if (fov > 45.0f)
        fov = 45.0f;
}

void Camera::ProcessMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch)
{
    xoffset *= mouseSensitivity;
    yoffset *= mouseSensitivity;
    
    yaw   += xoffset;
    pitch += yoffset;
    
    // make sure that when pitch is out of bounds, screen doesn't get flipped
    if (constrainPitch)
    {
        if (pitch > 89.0f)
            pitch = 89.0f;
        if (pitch < -89.0f)
            pitch = -89.0f;
    }
    
    // update Front, Right and Up Vectors using the updated Euler angles
    UpdateCameraVectors();
}

void Camera::ProcessInput(GLFWwindow *window, float deltaTime)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        this->ProcessKeyboard(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        this->ProcessKeyboard(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        this->ProcessKeyboard(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        this->ProcessKeyboard(RIGHT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
        this->ProcessKeyboard(UP, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
        this->ProcessKeyboard(DOWN, deltaTime);
}

// processes input received from any keyboard-like input system. Accepts input parameter in the form of camera defined ENUM (to abstract it from windowing systems)
void Camera::ProcessKeyboard(CameraMovement direction, float deltaTime)
{
    float velocity = movementSpeed * deltaTime;
    if (direction == FORWARD)
        worldPosition += frontDir * velocity;
    if (direction == BACKWARD)
        worldPosition -= frontDir * velocity;
    if (direction == LEFT)
        worldPosition -= rightDir * velocity;
    if (direction == RIGHT)
        worldPosition += rightDir * velocity;
    if (direction == UP)
        worldPosition += upDir * velocity;
    if (direction == DOWN)
        worldPosition -= upDir * velocity;
}
