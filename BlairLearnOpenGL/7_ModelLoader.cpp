//
// Created by Blair Ren on 2025/11/18.
//

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

#include "stb_image.h"
#include "BaseFunction.hpp"
#include "Shader.hpp"
#include "Model.h"
#include "Light.hpp"

// 立方体顶点数据
float cubeVertices[] = {
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

int modelLoader() {
    GLFWwindow* window = CreateWindowContextWithParam(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL");
    BaseFunction& baseFunction = BaseFunction::getInstance();

    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, BaseFunction::mouse_callback); // 鼠标移动回调函数
    glfwSetScrollCallback(window, BaseFunction::scroll_callback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    Shader modelShader("Shader/7_modelLoader_vs.vert", "Shader/7_modelLoader_fs.frag");
    Shader lightShader("Shader/5_phongLight_vs.vert", "Shader/5_phongLight_fs.frag");

    // 使用LightManager统一管理灯光
    LightManager lightManager(glm::vec3(1.2f, 1.0f, 2.0f), glm::vec3(1.0f, 1.0f, 1.0f));

    glEnable(GL_DEPTH_TEST);

    // 设置立方体缓冲区（用于灯光盒子）
    unsigned int VBO, lightVAO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), cubeVertices, GL_STATIC_DRAW);

    // 灯光立方体VAO（只使用位置数据）
    glGenVertexArrays(1, &lightVAO);
    glBindVertexArray(lightVAO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    Model model("Resource/backpack/backpack.obj");

    while (!glfwWindowShouldClose(window)) {
        float currentFrame = static_cast<float>(glfwGetTime());
        baseFunction.cameraEntity.deltaTime = currentFrame - baseFunction.cameraEntity.lastFrame;
        baseFunction.cameraEntity.lastFrame = currentFrame;
        baseFunction.cameraEntity.ProcessInput(window, baseFunction.cameraEntity.deltaTime);

        // 处理灯光相关按键
        lightManager.ProcessInput(window);

        glClearColor(0.05f, 0.05f, 0.05f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glm::mat4 modelMat = glm::mat4(1.0f);
        glm::mat4 viewMat = glm::mat4(1.0f);
        glm::mat4 projectionMat = glm::mat4(1.0f);

        modelMat = glm::translate(modelMat, glm::vec3(0.0f, 0.0f, 0.0f));
        modelMat = glm::scale(modelMat, glm::vec3(1.0f, 1.0f, 1.0f));
        viewMat = baseFunction.cameraEntity.GetViewMatrix();
        projectionMat = glm::perspective(glm::radians(baseFunction.cameraEntity.fov), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f); // 第二个参数是宽高比

        // 绘制模型
        modelShader.use();

        glUniformMatrix4fv(glGetUniformLocation(modelShader.shaderProgramID, "model"), 1, 0, glm::value_ptr(modelMat));
        glUniformMatrix4fv(glGetUniformLocation(modelShader.shaderProgramID, "view"), 1, 0, glm::value_ptr(viewMat));
        glUniformMatrix4fv(glGetUniformLocation(modelShader.shaderProgramID, "projection"), 1, 0, glm::value_ptr(projectionMat));

        model.Draw(modelShader);

        // 使用LightManager设置光照相关uniform
        lightManager.ApplyToObjectShader(modelShader, baseFunction.cameraEntity.worldPosition);
        // 绘制灯光立方体
        if (lightManager.showLightBox) {
            lightShader.use();

            glm::mat4 lightModel = glm::mat4(1.0f);
            lightModel = glm::translate(lightModel, lightManager.position);
            lightModel = glm::scale(lightModel, glm::vec3(0.1f)); // 小立方体

            glUniformMatrix4fv(glGetUniformLocation(lightShader.shaderProgramID, "model"), 1, 0, glm::value_ptr(lightModel));
            glUniformMatrix4fv(glGetUniformLocation(lightShader.shaderProgramID, "view"), 1, 0, glm::value_ptr(viewMat));
            glUniformMatrix4fv(glGetUniformLocation(lightShader.shaderProgramID, "projection"), 1, 0, glm::value_ptr(projectionMat));

            // 使用LightManager设置灯光颜色
            lightManager.ApplyToLightBoxShader(lightShader);

            glBindVertexArray(lightVAO);
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }

        // 交换颜色缓冲（我们应用双缓冲渲染窗口应用程序）。
        glfwSwapBuffers(window);
        // 检查触发事件（如键盘输入、鼠标移动、更新窗口状态等），并调用对应的回调函数（可以通过回调方法手动设置）。
        glfwPollEvents();
    }

    // 清理资源
    glDeleteVertexArrays(1, &lightVAO);
    glDeleteBuffers(1, &VBO);

    glfwTerminate();
    return 0;
}

int main() {
    modelLoader();

    int uniform_count = 0;
    glGetIntegerv(GL_MAX_VERTEX_UNIFORM_COMPONENTS, &uniform_count);
    std::cout << uniform_count << std::endl;

    return 0;
}