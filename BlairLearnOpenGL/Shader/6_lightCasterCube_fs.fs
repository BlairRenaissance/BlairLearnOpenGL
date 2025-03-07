#version 330 core

struct Material {
    sampler2D diffuse;
    sampler2D specular;
    float shininess;
};

in vec3 normal;
in vec3 fragPos;
in vec2 texCoord;

out vec4 FragColor;

uniform vec3 lightPos;
uniform vec3 viewPos;
uniform Material material;

void main() {
    // ambient
    float ambientStrength = 0.5;
    vec3 ambientLight = ambientStrength * texture(material.diffuse, texCoord).rgb;
    
    // diffuse
    float diffuseStrength = 0.3;
    vec3 norm = normalize(normal);
    vec3 lightDir = normalize(lightPos - fragPos);
    float diffuse = max(dot(lightDir, norm), 0.0);
    vec3 diffuseLight = diffuse * diffuseStrength * texture(material.diffuse, texCoord).rgb;
    
    // specular
    float specularStrength = 0.5;
    vec3 viewDir = normalize(viewPos - fragPos);
    vec3 lightRefDir = reflect(-lightDir, norm);
    float specular = pow(max(dot(lightRefDir, viewDir), 0.0), material.shininess);
    vec3 specularLight = specular * specularStrength * texture(material.specular, texCoord).rgb;
    
    vec3 result = ambientLight + diffuseLight + specularLight;
    FragColor = vec4(result, 1.0);
}

