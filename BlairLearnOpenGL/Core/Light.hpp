//
//  Light.hpp
//  BlairLearnOpenGL
//
//  Created by AI on 2025/11/21.
//

#ifndef Light_hpp
#define Light_hpp

#include <glm/glm.hpp>

struct GLFWwindow; // 前向声明，避免在头文件里引入GLFW

class Shader;

class LightManager {
public:
    glm::vec3 position;
    glm::vec3 color;
    bool enableLighting;
    bool showLightBox;

    // 记录上一次按键状态（也可以放到 cpp 里的静态变量）
    bool lastLKeyState = false;
    bool lastKKeyState = false;

    LightManager(const glm::vec3 &pos = glm::vec3(1.2f, 1.0f, 2.0f),
                 const glm::vec3 &color = glm::vec3(1.0f, 1.0f, 1.0f));

    // 将当前灯光参数应用到用于渲染物体的shader上
    void ApplyToObjectShader(const Shader &shader, const glm::vec3 &viewPos) const;

    // 将当前灯光参数应用到用于渲染灯光立方体的shader上
    void ApplyToLightBoxShader(const Shader &shader) const;

    // 灯光相关的键盘输入处理
    void ProcessInput(GLFWwindow* window);
};

#endif /* Light_hpp */
