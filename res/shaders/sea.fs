#version 330 core
out vec4 FragColor;

// in vec2 TexCoords;
in vec3 Normal;
in vec3 FragPos;

void main()
{    
    // ambient
    vec3 ambient = 0.25 * vec3(0.5f);

    // diffuse 
    vec3 lightPos = vec3(20, 10, 20);
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * vec3(1);

    // // specular
    // vec3 viewDir = normalize(-FragPos);
    // vec3 reflectDir = reflect(-lightDir, norm);  
    // float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    // vec3 specular = 1 * spec * vec3(texture(texture_specular1, TexCoords));
        
    vec3 result = ambient + diffuse;
    FragColor = vec4(result, 1.0);
}