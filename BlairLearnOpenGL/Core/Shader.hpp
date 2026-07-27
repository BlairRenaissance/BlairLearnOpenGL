//
//  Shader.hpp
//  BlairLearnOpenGL
//
//  Created by yiwen ren on 2024/5/22.
//

#ifndef Shader_hpp
#define Shader_hpp

#include <string>
#include <glm/glm.hpp>

class Shader{

public:
    
    Shader(const char* vertexPath, const char* fragmentPath);
    void use() const;

    void setInt(const std::string &name, int value) const;
    void SetInt(const std::string &name, int value) const;
    void setMat4(const std::string &name, const glm::mat4 &mat) const;
    void setVec3(const std::string &name, const glm::vec3 &value) const;
    void setVec3(const std::string &name, float x, float y, float z) const;

    unsigned int shaderProgramID = 0;
};

#endif /* Shader_hpp */
