#version 330 core
out vec4 FragColor;

in vec3 TexCoords;

uniform samplerCube skybox;
uniform vec3 sunDirection;

float GetSunMask(float sunViewDot, float sunRadius);

void main()
{    
    vec3 color = texture(skybox, TexCoords).rgb;

    // add sun
    float sunViewDot = dot(normalize(-sunDirection), normalize(TexCoords));
    float sunMask = GetSunMask(sunViewDot, 0.01);
    vec3 sun = vec3(1.0, 0.9, 0.6) * sunMask;

    FragColor = vec4(color + sun, 1.0);
}

float GetSunMask(float sunViewDot, float sunRadius)
{
    float stepRadius = 1 - sunRadius * sunRadius;
    return step(stepRadius, sunViewDot);
}