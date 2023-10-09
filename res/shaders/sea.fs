#version 330 core

struct Light
{
    vec3 direction;
    vec3 ambient;
    vec3 diffuse;
};

struct Material
{
    vec3 diffuse;
    vec3 specular;
    float shininess;
};

in vec3 Normal;
in vec3 FragPos;

uniform vec3 cameraPos;
uniform samplerCube skybox;
uniform Light light;
uniform Material material;

out vec4 FragColor;

void main()
{    
    // ambient
    vec3 ambient = light.ambient * material.diffuse;
  	
    // diffuse 
    vec3 normal = normalize(Normal);
    vec3 lightDir = normalize(-light.direction);
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 diffuse = diff * light.diffuse * material.diffuse;
    
    // specular
    vec3 viewDir = normalize(cameraPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, normal);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = spec * material.specular;  
        
    float ratio = 1.00 / 1.33;
    vec3 I = normalize(FragPos - cameraPos);
    vec3 R = refract(I, normalize(Normal), ratio);
    vec3 refraction = texture(skybox, R).rgb;

    vec3 result = ambient + diffuse + specular + refraction;

    FragColor = vec4(result, 1.0);
}