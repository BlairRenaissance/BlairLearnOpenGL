#version 330 core

struct Material {
    sampler2D diffuse;
    sampler2D specular;
    float shininess;
};

struct Spot {
    vec3  spotLightPos;
    vec3  spotLightDir;
    float cutOff;
    float outerCutOff;
};

in vec3 normal;
in vec3 fragPos;
in vec2 texCoord;

out vec4 FragColor;

uniform vec3 lightPos;
uniform vec3 viewPos;
uniform Material material;
uniform Spot spotLight;

vec3 PointLight();
vec3 SpotLight();

void main() {
    vec3 result = SpotLight();
    FragColor = vec4(result, 1.0);
}


vec3 PointLight(){
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
    return result;
}

vec3 SpotLight(){
    vec3 lightDir = normalize(spotLight.spotLightPos - fragPos);
    
    // check if lighting is inside the spotlight cone
    float theta = dot(lightDir, normalize(-spotLight.spotLightDir));
    float epsilon   = spotLight.cutOff - spotLight.outerCutOff;
    float intensity = clamp((theta - spotLight.outerCutOff) / epsilon, 0.0, 1.0);
    
    // ambient
    float ambientStrength = 0.3;
    vec3 ambientLight = ambientStrength * texture(material.diffuse, texCoord).rgb;
    
    // diffuse
    float diffuseStrength = 0.5;
    vec3 norm = normalize(normal);
    float diffuse = max(dot(lightDir, norm), 0.0);
    vec3 diffuseLight = diffuse * diffuseStrength * texture(material.diffuse, texCoord).rgb;
    
    // specular
    float specularStrength = 0.5;
    vec3 viewDir = normalize(viewPos - fragPos);
    vec3 lightRefDir = reflect(-lightDir, norm);
    float specular = pow(max(dot(lightRefDir, viewDir), 0.0), material.shininess);
    vec3 specularLight = specular * specularStrength * texture(material.specular, texCoord).rgb;
    
    vec3 result = ambientLight + diffuseLight * intensity + specularLight * intensity;
    return result;
    
}

