#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform float time;
uniform int numWaves;
uniform float speeds[10];
uniform float amplitudes[10];
uniform float waveLengths[10];

out vec2 TexCoords;
out vec3 FragPos;

void main()
{
    vec3 pos = aPos;

    // Sum of multiple sine waves
    float y = 0.0;
    for(int i = 0; i < numWaves; i++)
    {
        float frequency = 2.0 / waveLengths[i];
        float phase = speeds[i] / waveLengths[i];
        y += amplitudes[i] * sin(frequency * aPos.x + time * phase);
        y += amplitudes[i] * sin(frequency * aPos.z + time * phase);
    }

    pos.y = y;

    gl_Position = projection * view * model * vec4(pos, 1.0);
    FragPos = vec3(model * vec4(pos, 1.0));
    TexCoords = aTexCoords;
}