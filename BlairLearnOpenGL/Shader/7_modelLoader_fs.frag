#version 330 core

out vec4 FragColor;

struct Material {
    sampler2D texture_diffuse1;
    sampler2D texture_specular1;
    sampler2D texture_height1;
};

in vec2 texCoord;
in vec3 fragPos;
in vec3 normal;

uniform Material material;
uniform vec3 lightPos;
uniform vec3 viewPos;
uniform bool enableLighting;

void main() {
    // 获取纹理颜色
    vec3 diffuseColor = texture(material.texture_diffuse1, texCoord).rgb;
    vec3 specularColor = texture(material.texture_specular1, texCoord).rgb;
    float heightStrength = texture(material.texture_height1, texCoord).r;

    // 如果关闭光照，则只显示漫反射纹理
    if (!enableLighting) {
        FragColor = vec4(diffuseColor, 1.0);
        return;
    }

    // 基础光照计算
    vec3 n = normalize(normal);
    vec3 lightDir = normalize(lightPos - fragPos);
    vec3 viewDir = normalize(viewPos - fragPos);
    vec3 reflectDir = reflect(-lightDir, n);

    // 漫反射
    float diff = max(dot(n, lightDir), 0.0);
    vec3 diffuse = diff * diffuseColor;

    // 镜面反射
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32.0);
    vec3 specular = spec * specularColor;

    // 环境光
    vec3 ambient = vec3(0.1, 0.1, 0.1);

    // 最终颜色
    vec3 result = ambient + diffuse + specular;
//    result *= (0.8 + heightStrength * 0.4); // 使用高度贴图调整亮度

    FragColor = vec4(result, 1.0);
}