#version 330 core
out vec4 FragColor;

in vec3 TexCoords;

uniform samplerCube skybox;
uniform vec3 sunDirection;
uniform vec3 sunColor;
uniform float sunRadius;
uniform float time;

float GetSunMask(float sunViewDot, float sunRadius);

void main()
{    
    vec3 animatedTexCoords = TexCoords;
    animatedTexCoords.z += time * 0.007;

    vec3 color = texture(skybox, animatedTexCoords).rgb;

    // add sun
    float sunViewDot = dot(normalize(-sunDirection), normalize(TexCoords));
    float sunMask = GetSunMask(sunViewDot, sunRadius);
    vec3 sun = sunColor * sunMask;

    FragColor = vec4(color + sun, 1.0);
}

float GetSunMask(float sunViewDot, float sunRadius)
{
    float edge0 = 1 - (sunRadius * sunRadius);
    float edge1 = edge0 + 0.01; // decrease this value to slow down the fade out
    return smoothstep(edge0, edge1, sunViewDot);
}