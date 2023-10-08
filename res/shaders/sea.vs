#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

// distance between points
uniform float time;
uniform int numWaves;
uniform float speeds[10];
uniform vec2 directions[10];
uniform float amplitudes[10];
uniform float waveLengths[10];

out vec3 FragPos;
out vec3 Normal;

void main()
{
    vec3 pos = aPos;

    //calculate y and derivatives for normals
    float y = 0.0;
    float yx = 0.0;
    float yz = 0.0;

    for(int i = 0; i < numWaves; i++)
    {
        float frequency = 2.0 / waveLengths[i];
        float phase = speeds[i] / waveLengths[i];
        vec2 dir = directions[i];

        y += amplitudes[i] * sin(dot(dir, aPos.xz) * frequency + time * phase);

        yx += amplitudes[i] * dir.x * frequency * cos(dot(dir, aPos.xz) * frequency + time * phase);
        yz += amplitudes[i] * dir.y * frequency * cos(dot(dir, aPos.xz) * frequency + time * phase);
    }

    pos.y = y;

    //calculate normal
    Normal = normalize(vec3(-yx, 1.0, -yz));
    //Normal = normalize(aNormal);

    FragPos = vec3(model * vec4(pos, 1.0)); 
    gl_Position = projection * view * vec4(FragPos, 1.0);
}


        // y += amplitudes[i] * sin(frequency * pos.x + time * phase);
        // y += amplitudes[i] * sin(frequency * pos.z + time * phase);

        // yx += amplitudes[i] * phase * cos(frequency * pos.x + time * phase);
        // yz += amplitudes[i] * phase * cos(frequency * pos.z + time * phase);