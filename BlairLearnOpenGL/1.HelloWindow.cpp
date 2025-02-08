//
//  HelloWindow.cpp
//  BlairLearnOpenGL
//
//  Created by yiwen ren on 2024/5/21.
//

// 请确认是在包含GLFW的头文件之前包含了GLAD的头文件。
// GLAD的头文件包含了正确的OpenGL头文件（例如GL/gl.h），所以需要在其它依赖于OpenGL的头文件之前包含GLAD。
#include "1_HelloWindow.hpp"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

#include "BaseFunction.hpp"

int hello_window() {
    
#pragma mark 初始化窗口
    GLFWwindow* window = CreateWindowContext();
    
#pragma mark 顶点输入
    
    // 定义顶点数据
    float vertices[] = {
        // 位置              // 颜色
         0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,   // 右下
        -0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,   // 左下
         0.0f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f    // 顶部
    };
    
    // OpenGL的核心模式要求我们使用VAO。
    unsigned int VAO;
    // 第一个参数指定要生成的顶点数组对象名称的数量。
    glGenVertexArrays(1, &VAO);
    // 绑定顶点数组对象(Vertex Array Object, VAO)后的顶点属性调用都会储存在这个VAO中。
    // 当配置顶点属性指针时，只需要配置一次，之后再绘制物体的时候只需要绑定相应的VAO就行了。
    glBindVertexArray(VAO);
    /**
     unsigned int VAO[3];
     glGenVertexArrays(3, VAO);
     glBindVertexArray(VAO[1]);
     */
    
    // 使用顶点缓冲对象VBO等缓冲对象的好处是我们可以一次性的发送一大批数据到显卡上，而不是每个顶点发送一次。
    // 从CPU把数据发送到显卡相对较慢，所以只要可能我们都要尝试尽量一次性发送尽可能多的数据。
    unsigned int VBO;
    glGenBuffers(1, &VBO);
    // 顶点缓冲对象的缓冲类型是GL_ARRAY_BUFFER。
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    // 把用户定义的数据复制到当前绑定的缓冲中。
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    
    // 告诉OpenGL该如何解析顶点数据。
    // 第一个参数对应 vertex shader 中的layout(location = 0)。
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    // 以顶点属性位置值作为参数，启用顶点属性。顶点属性默认是禁用的。
    glEnableVertexAttribArray(0);
    // 颜色属性
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    
    
    /** 使用索引缓冲（EBO / IBO）
    float vertices_[] = {
        0.5f, 0.5f, 0.0f,   // 右上角
        0.5f, -0.5f, 0.0f,  // 右下角
        -0.5f, -0.5f, 0.0f, // 左下角
        -0.5f, 0.5f, 0.0f   // 左上角
    };

    unsigned int indices[] = {
        // 注意索引从0开始!
        // 此例的索引(0,1,2,3)就是顶点数组vertices的下标，
        // 这样可以由下标代表顶点组合成矩形

        0, 1, 3, // 第一个三角形
        1, 2, 3  // 第二个三角形
    };
    
    unsigned int EBO;
    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    */
    
#pragma mark 着色器
    
    const char *vertexShaderSource = 
        "#version 330 core\n"
        "layout (location = 0) in vec3 aPos;\n"
        "layout (location = 1) in vec3 aColor;\n"
        "out vec3 ourColor;"
        "void main()\n"
        "{\n"
        "   gl_Position = vec4(aPos, 1.0);\n"
        "   ourColor = aColor;\n"
        "}\0";
    
    unsigned int vertexShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    // 第一个参数是要编译的着色器对象。
    // 第二个参数指定了传递的源码字符串数量,这里只有一个。
    // 第三个参数是顶点着色器真正的源码。
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);
    // 检测编译时错误可以通过以下代码来实现。
    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if(!success) {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    
    const char *fragmentShaderSource =
        "#version 330 core\n"
        "uniform vec4 setColor;\n"
        "out vec4 FragColor;\n"
        "in vec3 ourColor;\n"
        "void main()\n"
        "{\n"
        "   FragColor = vec4(ourColor, 1.0);\n"
        "}\n\0";
    
    unsigned int fragmentShader;
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);
    // 检测编译错误
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if(!success) {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    
    // 着色器程序
    unsigned int shaderProgram;
    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    // 检测编译错误
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if(!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "ERROR::PROGRAM::LINK_FAILED\n" << infoLog << std::endl;
    }
    glUseProgram(shaderProgram);
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    
    
#pragma mark 渲染
    
    // 清屏颜色
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    
    // 我们希望程序在我们主动关闭它之前不断绘制图像并能够接受用户输入。
    // 因此我们需要在程序中添加一个while循环，我们可以把它称为渲染循环(Render Loop)。
    while(!glfwWindowShouldClose(window))
    {
        // 输入
        processInput(window);
        
        glClear(GL_COLOR_BUFFER_BIT);
        
        double timeValue = glfwGetTime();
        float greenValue = static_cast<float>(sin(timeValue) / 2.0 + 0.5);
        int vertexColorLocation = glGetUniformLocation(shaderProgram, "setColor");
        glUniform4f(vertexColorLocation, 0.0f, greenValue, 0.0f, 1.0f);
        
        glDrawArrays(GL_TRIANGLES, 0, 3);
        
        /** 使用索引缓冲（EBO / IBO）
         // 最后一个参数里我们可以指定EBO中的偏移量（或者传递一个索引数组，但是这是当你不在使用索引缓冲对象的时候）。
         glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
         */
        
        // 交换颜色缓冲（我们应用双缓冲渲染窗口应用程序）。
        glfwSwapBuffers(window);
        // 检查触发事件（如键盘输入、鼠标移动、更新窗口状态等），并调用对应的回调函数（可以通过回调方法手动设置）。
        glfwPollEvents();
    }
    
    // 当渲染循环结束后我们需要正确释放/删除之前的分配的所有资源。
    // 我们可以在main函数的最后调用glfwTerminate函数来完成。
    glfwTerminate();
    return 0;
}


