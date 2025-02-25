#version 330 core
in vec3 normal;
in vec3 fragPos;
in vec2 texCoord;

out vec4 FragColor;

uniform sampler2D texture1;
uniform sampler2D texture2;

uniform vec3 lightColor;
uniform vec3 lightPos;
uniform vec3 viewPos;

void main() {
    vec4 texColor = mix(texture(texture1, texCoord), texture(texture2, texCoord), 0.2);
    
    // ambient
    float ambientStrength = 0.1;
    vec3 ambientLight = ambientStrength * lightColor;
    
    // diffuse
    float diffuseStrength = 0.3;
    vec3 norm = normalize(normal);
    vec3 lightDir = normalize(lightPos - fragPos);
    float diffuse = max(dot(lightDir, norm), 0.0);
    vec3 diffuseLight = diffuse * diffuseStrength * lightColor;
    
    // specular
    float specularStrength = 0.5;
    vec3 viewDir = normalize(viewPos - fragPos);
    vec3 lightRefDir = reflect(-lightDir, norm);
    float specular = pow(max(dot(lightRefDir, viewDir), 0.0), 32);
    vec3 specularLight = specular * specularStrength * lightColor;
    
    vec3 result = texColor.rgb * (ambientLight + diffuseLight + specularLight);
    FragColor = vec4(result, 1.0);
}

