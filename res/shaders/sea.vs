#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

// function prototypes
float GetSinY(float x, float z);
vec3 GetNormal(float x, float z);

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

// distance between points
uniform float time;
uniform int numWaves;
uniform float speeds[10];
uniform float amplitudes[10];
uniform float waveLengths[10];

out vec3 FragPos;
out vec3 Normal;

void main()
{
    vec3 pos = aPos;

    //calculate y
    pos.y = GetSinY(aPos.x, aPos.z);

    //calculate normal
    Normal = GetNormal(aPos.x, aPos.z);

    FragPos = vec3(model * vec4(pos, 1.0)); 
    gl_Position = projection * view * vec4(FragPos, 1.0);
}

float GetSinY(float x, float z)
{
    // Sum of multiple sine waves
    float y = 0.0;

    for(int i = 0; i < numWaves; i++)
    {
        float frequency = 2.0 / waveLengths[i];
        float phase = speeds[i] / waveLengths[i];
        y += amplitudes[i] * sin(frequency * x + time * phase);
        y += amplitudes[i] * sin(frequency * z + time * phase);
    }

    return y;
}

vec3 GetNormal(float x, float z)
{
    // Sum of multiple sine waves
    float yx = 0.0;
    float yz = 0.0;

    for(int i = 0; i < numWaves; i++)
    {
        float frequency = 2.0 / waveLengths[i];
        float phase = speeds[i] / waveLengths[i];

        yx += amplitudes[i] * phase * cos(frequency * x + time * phase);
        yz += amplitudes[i] * phase * cos(frequency * z + time * phase);
    }

    return normalize(vec3(-yx, 1.0, -yz));
}
