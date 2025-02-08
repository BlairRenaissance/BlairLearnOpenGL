//
//  Shader.hpp
//  BlairLearnOpenGL
//
//  Created by yiwen ren on 2024/5/22.
//

#ifndef Shader_hpp
#define Shader_hpp

#include <glad/glad.h>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <stdio.h>
class Shader{

public:
    unsigned int shaderProgramID;
    
    Shader(const char* vertexPath, const char* fragmentPath);
    void use();
    
};

#endif /* Shader_hpp */
