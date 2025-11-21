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

int modelLoader() {
    GLFWwindow* window = CreateWindowContextWithParam(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL");
    BaseFunction& baseFunction = BaseFunction::getInstance();

    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, BaseFunction::mouse_callback); // 鼠标移动回调函数
    glfwSetScrollCallback(window, BaseFunction::scroll_callback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    Shader modelShader("Shader/7_modelLoader_vs.vert", "Shader/7_modelLoader_fs.frag");

    glEnable(GL_DEPTH_TEST);

    Model model("Resource/backpack/backpack.obj");

    while (!glfwWindowShouldClose(window)) {
        float currentFrame = static_cast<float>(glfwGetTime());
        baseFunction.cameraEntity.deltaTime = currentFrame - baseFunction.cameraEntity.lastFrame;
        baseFunction.cameraEntity.lastFrame = currentFrame;
        baseFunction.cameraEntity.ProcessInput(window, baseFunction.cameraEntity.deltaTime);

        glClearColor(0.05f, 0.05f, 0.05f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glm::mat4 modelMat = glm::mat4(1.0f);
        glm::mat4 viewMat = glm::mat4(1.0f);
        glm::mat4 projectionMat = glm::mat4(1.0f);

        modelMat = glm::translate(modelMat, glm::vec3(0.0f, 0.0f, 0.0f));
        modelMat = glm::scale(modelMat, glm::vec3(1.0f, 1.0f, 1.0f));
        viewMat = baseFunction.cameraEntity.GetViewMatrix();
        projectionMat = glm::perspective(glm::radians(baseFunction.cameraEntity.fov), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f); // 第二个参数是宽高比

        // 绘制Cube
        modelShader.use();

        glUniformMatrix4fv(glGetUniformLocation(modelShader.shaderProgramID, "model"), 1, 0, glm::value_ptr(modelMat));
        glUniformMatrix4fv(glGetUniformLocation(modelShader.shaderProgramID, "view"), 1, 0, glm::value_ptr(viewMat));
        glUniformMatrix4fv(glGetUniformLocation(modelShader.shaderProgramID, "projection"), 1, 0, glm::value_ptr(projectionMat));

        model.Draw(modelShader);

        // 交换颜色缓冲（我们应用双缓冲渲染窗口应用程序）。
        glfwSwapBuffers(window);
        // 检查触发事件（如键盘输入、鼠标移动、更新窗口状态等），并调用对应的回调函数（可以通过回调方法手动设置）。
        glfwPollEvents();
    }

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


