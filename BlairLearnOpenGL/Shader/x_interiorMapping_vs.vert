#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec2 TexCoord;
out vec3 vDirOS; // 物体空间中的单位视线方向（相机->顶点）

/**
不再采用View空间的原因：
转动镜头时室内图像存在扭曲拉伸，而我们希望仅在相机移动时看到景深效果；
畸变的核心原因是我们把射线方向用到了“视空间下的顶点向量”，它含有相机位置的尺度信息；
当相机转动时，这个尺度会随投影变化，导致体积追踪的数值不稳定和视觉拉伸。

解决办法：
在着色器里把射线方向改为“房间（物体空间）中的单位方向”，并用“房间为原点”的标准化盒子做求交。
这会让相机转动只改变方向，不再因为相机转动造成拉伸。
*/

void main()
{
    // 顶点位置：物体空间、世界空间、裁剪空间
    vec4 posWS = model * vec4(aPos, 1.0);
    vec4 posVS = view * posWS;
    gl_Position = projection * posVS;

    TexCoord = aTexCoord;

    // 计算相机世界位置：view的逆矩阵把(0,0,0,1)从视空间原点变换到世界空间
    vec3 camPosWS = vec3(inverse(view) * vec4(0.0, 0.0, 0.0, 1.0));
    // 转到物体空间
    vec3 camPosOS = vec3(inverse(model) * vec4(camPosWS, 1.0));

    // 物体空间下的方向：从相机到顶点，并归一化（消除相机位置尺度影响）
    vDirOS = normalize(aPos - camPosOS);
}
