//
//  Camera.hpp
//  BlairLearnOpenGL
//
//  Created by yiwen ren on 2025/2/10.
//

#ifndef Camera_hpp
#define Camera_hpp

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <stdio.h>

enum CameraMovement {
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT
};

// Default camera values
const float YAW         = -90.0f;
const float PITCH       =  0.0f;
const float SPEED       =  1.0f;
const float SENSITIVITY =  0.1f;
const float ZOOM        =  45.0f;
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

class Camera {
public:
    // yaw is initialized to -90.0 degrees since a yaw of 0.0 results in a direction vector pointing to the right so we initially rotate a bit to the left.
    float yaw   = -90.0f;
    float pitch =  0.0f;
    
    // camera Attributes
    glm::vec3 worldUp;
    glm::vec3 worldPosition;
    glm::vec3 frontDir;
    glm::vec3 rightDir;
    glm::vec3 upDir;
    
    bool firstMouse = true;
    float fov = 45.0f;
    float mouseSensitivity = 0.1f;
    float movementSpeed = 1.0f;
    float lastX = SCR_WIDTH / 2.0f;
    float lastY = SCR_HEIGHT / 2.0f;
    
    // timing
    float deltaTime = 0.0f;
    float lastFrame = 0.0f;
    
    // constructor with vectors
    Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float yawAngle = YAW, float pitchAngle = PITCH);
    
    glm::mat4 GetViewMatrix();
    void ProcessMouseScroll(float yoffset);
    void ProcessMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch = true);
    void ProcessInput(GLFWwindow *window, float deltaTime);

private:
    // calculates the front vector from the Camera's (updated) Euler Angles
    void UpdateCameraVectors();
    void ProcessKeyboard(CameraMovement direction, float deltaTime);
};

#endif /* Camera_hpp */
