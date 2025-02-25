//
//  Camera.cpp
//  BlairLearnOpenGL
//
//  Created by yiwen ren on 2025/2/10.
//

#include "Camera.hpp"

Camera::Camera(glm::vec3 position, glm::vec3 up, float yawAngle, float pitchAngle) 
:Front(glm::vec3(0.0f, 0.0f, -1.0f))
{
    Yaw = yawAngle;
    Pitch = pitchAngle;
    Position = position;
    worldUp = up;
    updateCameraVectors();
}

glm::mat4 Camera::GetViewMatrix(){
    return glm::lookAt(Position, Position + Front, Up);
}

void Camera::updateCameraVectors()
{
    // calculate the new Front vector
    glm::vec3 front;
    front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
    front.y = sin(glm::radians(Pitch));
    front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
    Front = glm::normalize(front);
    // normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
    Right = glm::normalize(glm::cross(Front, worldUp));
    Up    = glm::normalize(glm::cross(Right, Front));
}

void Camera::ProcessMouseScroll(float yoffset)
{
    Fov -= (float)yoffset;
    if (Fov < 1.0f)
        Fov = 1.0f;
    if (Fov > 45.0f)
        Fov = 45.0f;
}

void Camera::ProcessMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch)
{
    xoffset *= mouseSensitivity;
    yoffset *= mouseSensitivity;
    
    Yaw   += xoffset;
    Pitch += yoffset;
    
    // make sure that when pitch is out of bounds, screen doesn't get flipped
    if (constrainPitch)
    {
        if (Pitch > 89.0f)
            Pitch = 89.0f;
        if (Pitch < -89.0f)
            Pitch = -89.0f;
    }
    
    // update Front, Right and Up Vectors using the updated Euler angles
    updateCameraVectors();
}

