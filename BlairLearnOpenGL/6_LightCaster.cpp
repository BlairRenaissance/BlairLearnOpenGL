//
//  6_LightCaster.cpp
//  BlairLearnOpenGL
//
//  Created by yiwen ren on 2025/3/6.
//

#include "6_LightCaster.hpp"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

#include "stb_image.h"
#include "BaseFunction.hpp"
#include "Shader.hpp"


// lighting
glm::vec3 lightPos(0.4f, 0.4f, 1.0f);

int lightCaster(){
    GLFWwindow* window = CreateWindowContextWithParam(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL");
    BaseFunction& baseFunction = BaseFunction::getInstance();
    
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, BaseFunction::mouse_callback);
    glfwSetScrollCallback(window, BaseFunction::scroll_callback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    
    Shader cubeShader("Shader/6_lightCasterCube_vs.vs", "Shader/6_lightCasterCube_fs.fs");
    Shader lightShader("Shader/6_lightCaster_vs.vs", "Shader/6_lightCaster_fs.fs");
    
    glEnable(GL_DEPTH_TEST);
    
    float vertices[] = {
        // positions          // normals           // texture coords
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,
        0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  0.0f,
        0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
        0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,
        
        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,
        0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  0.0f,
        0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
        0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,
        
        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
        -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
        
        0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
        0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
        0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
        0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
        0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
        0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
        
        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,
        0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  1.0f,
        0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
        0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,
        
        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f,
        0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  1.0f,
        0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
        0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f
    };
    // positions all containers
    glm::vec3 cubePositions[] = {
        glm::vec3( 0.0f,  0.0f,  0.0f),
        glm::vec3( 2.0f,  5.0f, -15.0f),
        glm::vec3(-1.5f, -2.2f, -2.5f),
        glm::vec3(-3.8f, -2.0f, -12.3f),
        glm::vec3( 2.4f, -0.4f, -3.5f),
        glm::vec3(-1.7f,  3.0f, -7.5f),
        glm::vec3( 1.3f, -2.0f, -2.5f),
        glm::vec3( 1.5f,  2.0f, -2.5f),
        glm::vec3( 1.5f,  0.2f, -1.5f),
        glm::vec3(-1.3f,  1.0f, -1.5f)
    };
    
    unsigned int VBO, cubeVAO, lightVAO;
    
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    
    glGenVertexArrays(1, &cubeVAO);
    glBindVertexArray(cubeVAO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);
    
    glGenVertexArrays(1, &lightVAO);
    glBindVertexArray(lightVAO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    
    
#pragma mark 纹理1
    
    int width, height, nrChannels;
    
    unsigned int diffuseTexture;
    glGenTextures(1, &diffuseTexture);
    glBindTexture(GL_TEXTURE_2D, diffuseTexture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    unsigned char* data = stbi_load("../Resource/container2.png", &width, &height, &nrChannels, 0);
    if(data){
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    }else{
        std::cout << "Failed to load texture1" << std::endl;
    }
    stbi_image_free(data);
    
#pragma mark 纹理2
    
    unsigned int specularTexture;
    glGenTextures(1, &specularTexture);
    glBindTexture(GL_TEXTURE_2D, specularTexture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    
    // 图片原点在左上角，但OpenGL的原点在左下角
    stbi_set_flip_vertically_on_load(true);
    data = stbi_load("../Resource/container2_specular.png", &width, &height, &nrChannels, 0);
    if(data){
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    }else{
        std::cout << "Failed to load texture2" << std::endl;
    }
    stbi_image_free(data);
    
    cubeShader.use();
    
    glUniform1i(glGetUniformLocation(cubeShader.shaderProgramID, "material.diffuse"), 0);
    glUniform1i(glGetUniformLocation(cubeShader.shaderProgramID, "material.specular"), 1);
    
    
#pragma mark 渲染loop
    while(!glfwWindowShouldClose(window)){
        float currentFrame = static_cast<float>(glfwGetTime());
        baseFunction.cameraEntity.deltaTime = currentFrame - baseFunction.cameraEntity.lastFrame;
        baseFunction.cameraEntity.lastFrame = currentFrame;
        baseFunction.cameraEntity.ProcessInput(window, baseFunction.cameraEntity.deltaTime);
        
        glClearColor(0.0, 0.0, 0.0, 1.0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        glm::mat4 model = glm::mat4(1.0f);
        glm::mat4 view = glm::mat4(1.0f);
        glm::mat4 projection = glm::mat4(1.0f);
        
        view = baseFunction.cameraEntity.GetViewMatrix();
        projection = glm::perspective(glm::radians(baseFunction.cameraEntity.fov), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f); // 第二个参数是宽高比
        
        // 绘制Cube
        cubeShader.use();
        
        glUniformMatrix4fv(glGetUniformLocation(cubeShader.shaderProgramID, "view"), 1, 0, glm::value_ptr(view));
        glUniformMatrix4fv(glGetUniformLocation(cubeShader.shaderProgramID, "projection"), 1, 0, glm::value_ptr(projection));
        
        glUniform1f(glGetUniformLocation(cubeShader.shaderProgramID, "material.shininess"), 64.0f);
        glUniform3f(glGetUniformLocation(cubeShader.shaderProgramID, "lightPos"), lightPos.x, lightPos.y, lightPos.z);
        glUniform3f(glGetUniformLocation(cubeShader.shaderProgramID, "viewPos"), baseFunction.cameraEntity.worldPosition.x, baseFunction.cameraEntity.worldPosition.y, baseFunction.cameraEntity.worldPosition.z);
        
        glUniform3f(glGetUniformLocation(cubeShader.shaderProgramID, "spotLight.spotLightPos"), baseFunction.cameraEntity.worldPosition.x, baseFunction.cameraEntity.worldPosition.y, baseFunction.cameraEntity.worldPosition.z);
        glUniform3f(glGetUniformLocation(cubeShader.shaderProgramID, "spotLight.spotLightDir"), baseFunction.cameraEntity.frontDir.x, baseFunction.cameraEntity.frontDir.y, baseFunction.cameraEntity.frontDir.z);
        glUniform1f(glGetUniformLocation(cubeShader.shaderProgramID, "spotLight.cutOff"), glm::cos(glm::radians(12.5f)));
        glUniform1f(glGetUniformLocation(cubeShader.shaderProgramID, "spotLight.outerCutOff"), glm::cos(glm::radians(14.0f)));
        
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, diffuseTexture);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, specularTexture);
        
        glBindVertexArray(cubeVAO);
        for (unsigned int i = 0; i < 10; i++)
        {
            // calculate the model matrix for each object and pass it to shader before drawing
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, cubePositions[i]);
            float angle = 20.0f * i;
            model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
            glUniformMatrix4fv(glGetUniformLocation(cubeShader.shaderProgramID, "model"), 1, 0, glm::value_ptr(model));
            
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }
        
        // 绘制灯光Box
        lightShader.use();
        
        // 正常的变换顺序应该是先rotate再scale再translate
        // 但这里的目的只是移动到一个位置再让盒子小一点，所以没必要了
        model = glm::translate(model, lightPos);
        model = glm::scale(model, glm::vec3(0.1f)); // a smaller cube
        
        glUniformMatrix4fv(glGetUniformLocation(lightShader.shaderProgramID, "model"), 1, 0, glm::value_ptr(model));
        glUniformMatrix4fv(glGetUniformLocation(lightShader.shaderProgramID, "view"), 1, 0, glm::value_ptr(view));
        glUniformMatrix4fv(glGetUniformLocation(lightShader.shaderProgramID, "projection"), 1, 0, glm::value_ptr(projection));
        
        glUniform3f(glGetUniformLocation(lightShader.shaderProgramID, "lightColor"), 1.0f, 1.0f, 1.0f);
        
        glBindVertexArray(lightVAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        
        
        // 交换颜色缓冲（我们应用双缓冲渲染窗口应用程序）。
        glfwSwapBuffers(window);
        // 检查触发事件（如键盘输入、鼠标移动、更新窗口状态等），并调用对应的回调函数（可以通过回调方法手动设置）。
        glfwPollEvents();
        
    }
    
    // 释放缓冲区空间
    glDeleteVertexArrays(1, &cubeVAO);
    glDeleteVertexArrays(1, &lightVAO);
    glDeleteBuffers(1, &VBO);
    
    glfwTerminate();
    return 0;
}
