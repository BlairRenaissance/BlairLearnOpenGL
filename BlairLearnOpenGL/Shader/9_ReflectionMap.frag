#version 330 core

out vec4 FragColor;

in vec3 fragPos;
in vec3 normal;

uniform vec3 cameraPos;
uniform samplerCube skybox;

void main() {
    vec3 viewDir = normalize(fragPos - cameraPos);
    vec3 reflectDir = reflect(viewDir, normalize(normal));
    FragColor = vec4(texture(skybox, reflectDir).rgb, 1.0);
}
