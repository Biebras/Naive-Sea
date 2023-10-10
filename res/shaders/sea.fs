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
    float refractiveIndex;
    vec3 highlight;
    float highlightOffset;
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
    vec3 viewDir = normalize(cameraPos - FragPos);

    // ambient
    vec3 ambient = light.ambient * material.diffuse;
  	
    // diffuse 
    vec3 normal = normalize(Normal);
    vec3 lightDir = normalize(-light.direction);
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 diffuse = diff * light.diffuse * material.diffuse;

    //highlight
    float highlightMask = max((FragPos.y - material.highlightOffset), 0.0);
    float distanceToPlayer = length(cameraPos - FragPos);
    float fadeFactor = max(1.0 - (distanceToPlayer / 125.0), 0);
    vec3 highlight = material.highlight * highlightMask * fadeFactor;

    // Fresnel
    float fresnel = pow(1.0 - max(dot(viewDir, normal), 0.15), 5.0);
    
    // specular
    vec3 reflectDir = reflect(-lightDir, normal);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = spec * material.specular * fresnel;  
    
    // refraction
    float ratio = 1.00 / material.refractiveIndex;
    vec3 I = normalize(FragPos - cameraPos);
    vec3 R = refract(I, normalize(Normal), ratio);
    vec3 refraction = (1.0 - fresnel) * texture(skybox, R).rgb;
    
    vec3 result = ambient + diffuse + highlight + specular + refraction;

    FragColor = vec4(result, 1.0);
}