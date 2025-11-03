#version 330 core
in vec2 TexCoord;
in vec3 vDirOS; // 物体空间单位视线方向

out vec4 FragColor;

uniform sampler2D texture1;
uniform float roomDepth;   // 房间深度参数 (0.001~0.999)

void main()
{
    // 房间UV（平面局部坐标）
    vec2 roomUV = fract(TexCoord);

    // 将 [0,1] 深度映射到 [0,+inf)
    float farFrac = roomDepth;
    float depthScale = 1.0 / (1.0 - farFrac) - 1.0;

    // 以房间为原点：规范化盒空间的起点在当前片元所在的平面 z = -1
    vec3 pos = vec3(roomUV * 2.0 - 1.0, -1.0);

    // 射线方向：物体空间单位方向，调整Z以匹配底面追踪
    vec3 dir = vDirOS;
    dir.z *= -depthScale;

    // 避免除零导致的NaN/Inf
    const vec3 eps = vec3(1e-6);
    vec3 id = 1.0 / (dir + eps);
    vec3 k = abs(id) - pos * id;
    float kMin = min(min(k.x, k.y), k.z);
    pos += kMin * dir;

    // z 从 [-1,1] 映射到 [0,1]
    float interp = pos.z * 0.5 + 0.5;

    // 透视校正（将非线性深度恢复为线性视觉效果）
    float realZ = clamp(interp, 0.0, 1.0) / depthScale + 1.0;
    interp = 1.0 - (1.0 / realZ);
    interp *= depthScale + 1.0;

    // 从远墙插值回近墙，得到室内的UV
    vec2 interiorUV = pos.xy * mix(1.0, farFrac, interp);
    interiorUV = interiorUV * 0.5 + 0.5;

    // 采样预投影2D室内图
    vec4 room = texture(texture1, interiorUV);
    FragColor = vec4(room.rgb, 1.0);
}
