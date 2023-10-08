#version 330 core
out vec4 FragColor;

in vec3 Normal;
in vec3 FragPos;

void main()
{    
    // ambient
    vec3 ambient = 0.25 * vec3(0.3f);

    // diffuse 
    vec3 lightPos = vec3(0, 10, 0);
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * vec3(0.4);

    // // specular
    // vec3 viewDir = normalize(-FragPos);
    // vec3 reflectDir = reflect(-lightDir, norm);  
    // float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    // vec3 specular = 1 * spec * vec3(texture(texture_specular1, TexCoords));
        
    vec3 result = ambient + diffuse;
    FragColor = vec4(result, 1.0);
}