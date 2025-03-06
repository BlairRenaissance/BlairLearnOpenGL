//
//  BaseFunction.cpp
//  BlairLearnOpenGL
//
//  Created by yiwen ren on 2024/5/28.
//

#include "BaseFunction.hpp"

#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}


void processInput(GLFWwindow *window){
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}


GLFWwindow* CreateWindowContext()
{
    // 初始化GLFW。
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    
    // 实例化GLFW窗口。
    GLFWwindow* window = glfwCreateWindow(800, 600, "hello window", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return nullptr;
    }
    glfwMakeContextCurrent(window);
    
    // 在 OpenGL 中，GLAD 是一个用于加载 OpenGL 函数指针的库。由于 OpenGL 函数在运行时才被加载，因此在调用任何 OpenGL 函数之前，必须先初始化 GLAD。
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return nullptr;
    }
    
    return window;
}

GLFWwindow* CreateWindowContextWithParam(const unsigned int SCR_WIDTH, const unsigned int SCR_HEIGHT, const char* windowName)
{
    // 初始化GLFW。
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    
    // 实例化GLFW窗口。
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, windowName, NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return nullptr;
    }
    glfwMakeContextCurrent(window);
    
    // 在 OpenGL 中，GLAD 是一个用于加载 OpenGL 函数指针的库。由于 OpenGL 函数在运行时才被加载，因此在调用任何 OpenGL 函数之前，必须先初始化 GLAD。
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return nullptr;
    }
    
    return window;
}

void BaseFunction::HandleMouseCallback(GLFWwindow* window, double xposIn, double yposIn) {
    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);
    
    if (cameraEntity.firstMouse) {
        cameraEntity.lastX = xpos;
        cameraEntity.lastY = ypos;
        cameraEntity.firstMouse = false;
    }
    
    float xoffset = xpos - cameraEntity.lastX;
    float yoffset = cameraEntity.lastY - ypos; // reversed since y-coordinates go from bottom to top
    
    cameraEntity.lastX = xpos;
    cameraEntity.lastY = ypos;
    
    cameraEntity.ProcessMouseMovement(xoffset, yoffset);
}

void BaseFunction::HandleScrollCallback(GLFWwindow* window, double xoffset, double yoffset){
    cameraEntity.ProcessMouseScroll(static_cast<float>(yoffset));
}


