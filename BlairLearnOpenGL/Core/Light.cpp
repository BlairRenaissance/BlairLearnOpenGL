//
//  Light.cpp
//  BlairLearnOpenGL
//
//  Created by AI on 2025/11/21.
//

#include "Light.hpp"
#include "Shader.hpp"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

LightManager::LightManager(const glm::vec3 &pos, const glm::vec3 &color)
: position(pos), color(color), enableLighting(true), showLightBox(true) {}

void LightManager::ApplyToObjectShader(const Shader &shader, const glm::vec3 &viewPos) const {
    // 这里假定shader已经被use()
    glUniform3fv(glGetUniformLocation(shader.shaderProgramID, "lightPos"), 1, glm::value_ptr(position));
    glUniform3fv(glGetUniformLocation(shader.shaderProgramID, "viewPos"), 1, glm::value_ptr(viewPos));

    // 可选的开关，如果shader里没有这个uniform则忽略
    int enableLoc = glGetUniformLocation(shader.shaderProgramID, "enableLighting");
    if (enableLoc != -1) {
        glUniform1i(enableLoc, enableLighting ? 1 : 0);
    }
}

void LightManager::ApplyToLightBoxShader(const Shader &shader) const {
    // 这里假定shader已经被use()
    int colorLoc = glGetUniformLocation(shader.shaderProgramID, "lightColor");
    if (colorLoc != -1) {
        glUniform3fv(colorLoc, 1, glm::value_ptr(color));
    }
}

void LightManager::ProcessInput(GLFWwindow* window) {
    bool lPressed = glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS;
    if (lPressed && !lastLKeyState) {
        showLightBox = !showLightBox;
        std::cout << "[Light] Light box " << (showLightBox ? "ON" : "OFF") << std::endl;
    }
    lastLKeyState = lPressed;

    bool kPressed = glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS;
    if (kPressed && !lastKKeyState) {
        enableLighting = !enableLighting;
        std::cout << "[Light] Lighting " << (enableLighting ? "ON" : "OFF") << std::endl;
    }
    lastKKeyState = kPressed;
}

