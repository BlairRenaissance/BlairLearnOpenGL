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
#include "1_HelloWindow.hpp"
#include "2_Texture.hpp"
#include "3_Coordinate.hpp"
#include "4_Camera.hpp"


int main(int argc, const char * argv[]) {
//    hello_window();
//    texture();
//    coordinate();
    camera();
    
    int uniform_count = 0;
    glGetIntegerv(GL_MAX_VERTEX_UNIFORM_COMPONENTS, &uniform_count);
    std::cout << uniform_count << std::endl;

}
