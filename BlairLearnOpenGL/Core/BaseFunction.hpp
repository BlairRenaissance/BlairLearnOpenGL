//
//  BaseFunction.hpp
//  BlairLearnOpenGL
//
//  Created by yiwen ren on 2024/5/28.
//

#ifndef BaseFunction_hpp
#define BaseFunction_hpp

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stdio.h>

// 窗口回调函数，每次窗口大小被调整的时候被调用。
void framebuffer_size_callback(GLFWwindow* window, int width, int height);

// 处理键盘输入。
void processInput(GLFWwindow *window);

// 初始化窗口。
GLFWwindow* CreateWindowContext();

GLFWwindow* CreateWindowContextWithParam(const unsigned int SCR_WIDTH = 800, 
                                const unsigned int SCR_HEIGHT = 600,
                                const char* windowName = "Hello window");

#endif /* BaseFunction_hpp */
