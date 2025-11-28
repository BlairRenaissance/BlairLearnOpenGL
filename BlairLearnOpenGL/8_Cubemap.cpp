//
// Created by Blair Ren on 2025/11/27.
//

#include<iostream>
#include <stb_image.h>

#include "BaseFunction.hpp"
#include "Shader.hpp"
#include "Camera.hpp"
#include "Model.h"

float skyboxVertices[] = {
	// positions
	-1.0f, 1.0f, -1.0f,
	-1.0f, -1.0f, -1.0f,
	1.0f, -1.0f, -1.0f,
	1.0f, -1.0f, -1.0f,
	1.0f, 1.0f, -1.0f,
	-1.0f, 1.0f, -1.0f,
	-1.0f, -1.0f, 1.0f,
	-1.0f, -1.0f, -1.0f,
	-1.0f, 1.0f, -1.0f,
	-1.0f, 1.0f, -1.0f,
	-1.0f, 1.0f, 1.0f,
	-1.0f, -1.0f, 1.0f,

	1.0f, -1.0f, -1.0f,
	1.0f, -1.0f, 1.0f,
	1.0f, 1.0f, 1.0f,
	1.0f, 1.0f, 1.0f,
	1.0f, 1.0f, -1.0f,
	1.0f, -1.0f, -1.0f,

	-1.0f, -1.0f, 1.0f,
	-1.0f, 1.0f, 1.0f,
	1.0f, 1.0f, 1.0f,
	1.0f, 1.0f, 1.0f,
	1.0f, -1.0f, 1.0f,
	-1.0f, -1.0f, 1.0f,

	-1.0f, 1.0f, -1.0f,
	1.0f, 1.0f, -1.0f,
	1.0f, 1.0f, 1.0f,
	1.0f, 1.0f, 1.0f,
	-1.0f, 1.0f, 1.0f,
	-1.0f, 1.0f, -1.0f,

	-1.0f, -1.0f, -1.0f,
	-1.0f, -1.0f, 1.0f,
	1.0f, -1.0f, -1.0f,
	1.0f, -1.0f, -1.0f,
	-1.0f, -1.0f, 1.0f,
	1.0f, -1.0f, 1.0f
};

unsigned int loadCubemap(const std::vector<std::string>& texturePaths) {
	unsigned int textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

	int width, height, nrChannels;
	unsigned char* data;
	for(unsigned int i = 0; i < texturePaths.size(); i++){
		data = stbi_load(texturePaths[i].c_str(), &width, &height, &nrChannels, 0);
		if(data){
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		}else{
			std::cout << "Failed to load texture2" << std::endl;
		}
		stbi_image_free(data);
	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	return textureID;
}

int cubemap() {
	GLFWwindow *window = CreateWindowContextWithParam(SCR_WIDTH, SCR_HEIGHT, "Cubemap");
	BaseFunction &baseFunction = BaseFunction::getInstance();

	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, BaseFunction::mouse_callback);
	glfwSetScrollCallback(window, BaseFunction::scroll_callback);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	Shader skyShader("Shader/8_cubemap_vs.vert", "Shader/8_cubemap_fs.frag");
	Shader modelShader("Shader/7_modelLoader_vs.vert", "Shader/7_modelLoader_fs.frag");

	unsigned int VAO, VBO;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), skyboxVertices, GL_STATIC_DRAW);

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *) 0);

	std::vector<std::string> texturePaths = {
		"Resource/skybox/right.jpg",
		"Resource/skybox/left.jpg",
		"Resource/skybox/top.jpg",
		"Resource/skybox/bottom.jpg",
		"Resource/skybox/front.jpg",
		"Resource/skybox/back.jpg"
	};
	unsigned int textureID = loadCubemap(texturePaths);

	skyShader.use();
	glUniform1f(glGetUniformLocation(skyShader.shaderProgramID, "skybox"), 0);
	glEnable(GL_DEPTH_TEST);
	// 开启背面剔除
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glDepthFunc(GL_LEQUAL);

	while (!glfwWindowShouldClose(window)) {
		float currentFrame = static_cast<float>(glfwGetTime());
		baseFunction.cameraEntity.deltaTime = currentFrame - baseFunction.cameraEntity.lastFrame;
		baseFunction.cameraEntity.lastFrame = currentFrame;
		baseFunction.cameraEntity.ProcessInput(window, baseFunction.cameraEntity.deltaTime);

		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		skyShader.use();
		glm::mat4 view = glm::mat4(1.0f);
		glm::mat4 projection = glm::mat4(1.0f);
		// 绘制天空盒Cubemap，只取旋转部分，不取平移部分
		view = glm::mat4(glm::mat3(baseFunction.cameraEntity.GetViewMatrix()));
		projection = glm::perspective(glm::radians(baseFunction.cameraEntity.fov), (float) SCR_WIDTH / (float) SCR_HEIGHT, 0.1f, 100.0f); // 第二个参数是宽高比
		glUniformMatrix4fv(glGetUniformLocation(skyShader.shaderProgramID, "view"), 1, 0, glm::value_ptr(view));
		glUniformMatrix4fv(glGetUniformLocation(skyShader.shaderProgramID, "projection"), 1, 0, glm::value_ptr(projection));

		glBindVertexArray(VAO);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);

		// modelShader.use();
		// glUniformMatrix4fv(glGetUniformLocation(modelShader.shaderProgramID, "model"), 1, 0, glm::value_ptr(model))
		// glUniformMatrix4fv(glGetUniformLocation(modelShader.shaderProgramID, "view"), 1, 0, glm::value_ptr(view));
		// glUniformMatrix4fv(glGetUniformLocation(modelShader.shaderProgramID, "projection"), 1, 0, glm::value_ptr(projection));
		//
		// modelRes.Draw(modelShader);
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	return 0;
}

int main() {
	cubemap();
	return 0;
}
