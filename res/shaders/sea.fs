#version 330 core
out vec4 FragColor;

in vec2 TexCoords;
in vec3 FragPos;

uniform sampler2D texture_diffuse1;
uniform sampler2D texture_specular1;


void main()
{    
    // // ambient
    // vec3 ambient = 0.25 * vec3(texture(texture_diffuse1, TexCoords));

    // // diffuse 
    // vec3 norm = normalize(Normal);
    // vec3 lightDir = normalize(vec3(0, 100, 100) - FragPos);
    // float diff = max(dot(norm, lightDir), 0.0);
    // vec3 diffuse = 1 * diff * vec3(texture(texture_diffuse1, TexCoords));  

    // // specular
    // vec3 viewDir = normalize(-FragPos);
    // vec3 reflectDir = reflect(-lightDir, norm);  
    // float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    // vec3 specular = 1 * spec * vec3(texture(texture_specular1, TexCoords));
        
    // vec3 result = ambient + diffuse + specular;
    FragColor = vec4(1.0);
}