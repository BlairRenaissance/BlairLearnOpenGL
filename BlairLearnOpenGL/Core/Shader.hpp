//
//  Shader.hpp
//  BlairLearnOpenGL
//
//  Created by yiwen ren on 2024/5/22.
//

#ifndef Shader_hpp
#define Shader_hpp

#include <string>

class Shader{

public:
    unsigned int shaderProgramID;
    
    Shader(const char* vertexPath, const char* fragmentPath);
    void use();

    void SetInt(const std::string &name, int value) const;
    
};

#endif /* Shader_hpp */
