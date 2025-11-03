#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec2 TexCoord;
out vec3 vViewDirVS; // 视空间下：从相机指向顶点

void main()
{
    // 顶点到视空间
    vec4 posVS = view * model * vec4(aPos, 1.0);
    // 与Unity一致：viewDir = vertex - camera(0) -> 从相机指向顶点
    vViewDirVS = posVS.xyz;

    TexCoord = aTexCoord;

    gl_Position = projection * posVS;
}
