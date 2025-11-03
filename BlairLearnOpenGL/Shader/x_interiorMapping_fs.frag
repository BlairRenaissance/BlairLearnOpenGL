#version 330 core
in vec2 TexCoord;
in vec3 vViewDirVS;

out vec4 FragColor;

uniform sampler2D texture1;
uniform float roomDepth;   // 房间深度参数 (0.001~0.999)

void main()
{
    vec2 roomUV = fract(TexCoord);
    float farFrac = roomDepth;

    // 将 [0,1] 深度映射到 [0,+inf)
    float depthScale = 1.0 / (1.0 - farFrac) - 1.0;

    // 起点为规范化盒空间平面 z = -1
    vec3 pos = vec3(roomUV * 2.0 - 1.0, -1.0);

    // 视线方向（相机->表面），与Unity一致；z 方向反转并缩放
    vec3 dir = vViewDirVS;
    dir.z *= -depthScale;

    // 避免除零
    vec3 id = 1.0 / (dir + vec3(1e-6));
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

    // 采样（当前没有图集，只采一次）
    vec4 room = texture(texture1, interiorUV);
    FragColor = vec4(room.rgb, 1.0);
}
