//
//  HelloWindow.hpp
//  BlairLearnOpenGL
//
//  Created by yiwen ren on 2024/5/21.
//

#ifndef HelloWindow_hpp
#define HelloWindow_hpp

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stdio.h>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);

int hello_window();

#endif /* HelloWindow_hpp */
