//
//  Coordinate.cpp
//  BlairLearnOpenGL
//
//  Created by yiwen ren on 2024/5/27.
//

#include "3_Coordinate.hpp"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include "stb_image.h"

#include "BaseFunction.hpp"
#include "Shader.hpp"

int coordinate()
{
    GLFWwindow* window = CreateWindowContext();
    
    Shader transformShader("Shader/transform_vs.vs", "Shader/transform_fs.fs");
    
    float vertices[] = {
        // positions          // texture coords
         0.5f,  0.5f, 0.0f,   1.0f, 1.0f, // top right
         0.5f, -0.5f, 0.0f,   1.0f, 0.0f, // bottom right
        -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, // bottom left
        -0.5f,  0.5f, 0.0f,   0.0f, 1.0f  // top left
    };
    unsigned int indices[] = {
        0, 1, 3, // first triangle
        1, 2, 3  // second triangle
    };
    
#pragma mark 设置缓冲区
    unsigned int VBO, VAO, EBO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    
    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    
    
    unsigned int texture1, texture2;
    int width, height, nrChannels;
    
#pragma mark 纹理1
    glGenTextures(1, &texture1);
    glBindTexture(GL_TEXTURE_2D, texture1);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    unsigned char* data = stbi_load("../Resource/container.jpg", &width, &height, &nrChannels, 0);
    if(data){
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    }else{
        std::cout << "Failed to load texture1" << std::endl;
    }
    stbi_image_free(data);
    
#pragma mark 纹理2
    glGenTextures(1, &texture2);
    glBindTexture(GL_TEXTURE_2D, texture2);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    
    // 图片原点在左上角，但OpenGL的原点在左下角
    stbi_set_flip_vertically_on_load(true);
    data = stbi_load("../Resource/awesomeface.png", &width, &height, &nrChannels, 0);
    if(data){
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    }else{
        std::cout << "Failed to load texture2" << std::endl;
    }
    stbi_image_free(data);
    
    transformShader.use();
    
    // 通过glUniform1i设置每个着色器采样器属于哪个纹理单元(GL_TEXTURE0/1/../15)
    glUniform1i(glGetUniformLocation(transformShader.shaderProgramID, "texture1"), 0);
    glUniform1i(glGetUniformLocation(transformShader.shaderProgramID, "texture2"), 1);
    
#pragma mark 渲染loop
    while (!glfwWindowShouldClose(window)) {
        processInput(window);
        glClearColor(0.0, 0.0, 0.0, 1.0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture1);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture2);
        
        glm::mat4 model = glm::mat4(1.0f);
        glm::mat4 view = glm::mat4(1.0f);
        glm::mat4 projection = glm::mat4(1.0f);
        
        model = glm::rotate(model, glm::radians(-55.f), glm::vec3(1.0f, 0.0f, 0.0f));
        view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));
        projection = glm::perspective(glm::radians(45.0f), 800.0f/600.0f, 0.1f, 100.0f); // 第二个参数是宽高比
        
        /*
         * 第一个参数是Uniform Location
         * 第二个参数是Count，指定要修改的矩阵数量。如果目标统一变量不是矩阵数组，则该值应为 1；如果它是矩阵数组，则该值应为 1 或更多。
         * 第三个参数是布尔值transpose，指定在将值加载到统一变量中时是否转置矩阵。
         * 第四个参数是const GLfloat * value，是一个指向数据起始位置的指针。下面展示了两种获取方法。
         * 注意：第四个参数这里不能使用&model。（矩阵数据的指针vs.对象的地址）
           在 C++ 中，glm::mat4 是一个对象，它包含了矩阵的数据。对象的地址 &model 是 glm::mat4 对象在内存中的起始地址。这个地址指向的是整个对象，而不仅仅是矩阵数据。
           glm::value_ptr(model) 返回一个指向矩阵数据的指针，这个指针可以直接传递给 OpenGL 函数。
           glUniformMatrix4fv 函数期望接收一个指向矩阵数据的指针，而不是一个指向 glm::mat4 对象的指针。glm::mat4 对象可能包含额外的元数据或方法，而不仅仅是矩阵数据本身。因此，直接使用 &model 会导致错误。
         * 正确的用法需要使用 glm::value_ptr 函数来获取矩阵数据的指针，或者使用 &model[0][0] 获取矩阵数据的首地址，这个地址可以直接传递给 OpenGL 函数。
         */
        
        glUniformMatrix4fv(glGetUniformLocation(transformShader.shaderProgramID, "model"), 1, 0, glm::value_ptr(model));
        glUniformMatrix4fv(glGetUniformLocation(transformShader.shaderProgramID, "view"), 1, 0, &view[0][0]);
        glUniformMatrix4fv(glGetUniformLocation(transformShader.shaderProgramID, "projection"), 1, 0, &projection[0][0]);
        
        /*
         * 第三个参数指定索引值的类型。
         * 第四个参数指定相对于第一个索引的偏移量。
         */
        glDrawElements(GL_TRIANGLES, sizeof(indices), GL_UNSIGNED_INT, 0);
        
        // 交换颜色缓冲（我们应用双缓冲渲染窗口应用程序）。
        glfwSwapBuffers(window);
        // 检查触发事件（如键盘输入、鼠标移动、更新窗口状态等），并调用对应的回调函数（可以通过回调方法手动设置）。
        glfwPollEvents();
    }
    
    // 释放缓冲区空间
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    
    glfwTerminate();
    return 0;
}
