#version 330 core

//struct Material {
//    sampler2D texture_diffuse1;
//    sampler2D texture_specular1;
//    sampler2D texture_height1;
//};

in vec2 texCoord;
out vec4 FragColor;

//uniform Material material;

uniform sampler2D texture_diffuse1;

void main() {
//    FragColor = vec4(texture(material.texture_diffuse1, texCoord).rgb, 1.0);
//    FragColor = vec4(texCoord, 1.0, 1.0);
    FragColor = texture(texture_diffuse1, texCoord);
}