//
//  OpenGLContext.hpp
//  BlairLearnOpenGL
//
//  Created by yiwen ren on 2024/5/28.
//

#ifndef OpenGLContext_hpp
#define OpenGLContext_hpp

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stdio.h>

#include "Camera.hpp"

// 初始化窗口。
GLFWwindow* CreateWindowContext();

GLFWwindow* CreateWindowContextWithParam(const unsigned int SCR_WIDTH = 800,
                                         const unsigned int SCR_HEIGHT = 600,
                                         const char* windowName = "Hello window");

// 窗口回调函数，每次窗口大小被调整的时候被调用。
void framebuffer_size_callback(GLFWwindow* window, int width, int height);

// 处理键盘输入。
void processInput(GLFWwindow *window);



class OpenGLContext {
public:
    // 获取单例实例的静态方法
    static OpenGLContext& getInstance() {
        static OpenGLContext instance;
        return instance;
    }
    
    // 禁止拷贝构造和赋值运算符
    OpenGLContext(const OpenGLContext&) = delete;
    OpenGLContext& operator=(const OpenGLContext&) = delete;
    
    Camera cameraEntity;
    
private:
    // 构造函数
    OpenGLContext() : cameraEntity(glm::vec3(0.0f, 0.0f, 3.0f)) {}
};


#endif /* OpenGLContext_hpp */
