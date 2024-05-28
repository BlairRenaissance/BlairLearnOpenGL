//
//  main.cpp
//  BlairLearnOpenGL
//
//  Created by yiwen ren on 2024/5/21.
//

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

#include "BaseFunction.hpp"
#include "HelloWindow.hpp"
#include "Texture.hpp"


int main(int argc, const char * argv[]) {
//    hello_window();
    texture();
    
    int uniform_count = 0;
    glGetIntegerv(GL_MAX_VERTEX_UNIFORM_COMPONENTS, &uniform_count);
    std::cout << uniform_count << std::endl;

}
