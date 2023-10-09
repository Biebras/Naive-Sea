#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

float Random (vec2 seed);
vec2 GetRandomDirection(int waveIndex);

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

// distance between points
uniform float time;
uniform int numWaves;
uniform float directionSeed;

out vec3 FragPos;
out vec3 Normal;

void main()
{
    vec3 pos = aPos;

    // constants
    float e = 2.71828;

    // wave parameters
    float amplitude = 0.9;
    float frequency = 0.5;
    float speed = 1.25;
    float amplitudeSum = 0.0;

    //calculate y and derivatives for normals
    float y = 0.0;
    float yx = 0.0;
    float yz = 0.0;

    for(int i = 0; i < numWaves; i++)
    {
        vec2 dir = GetRandomDirection(i);

        // calculate input for wave equation
        float x = dot(dir, pos.xz) * frequency + time * speed;

        // wave equation
        y += pow(e, amplitude * sin(x) - 1);
        
        // derivative respect to x and z
        yx += amplitude * dir.x * frequency * cos(x) * pow(e, amplitude * sin(x) - 1);
        yz += amplitude * dir.y * frequency * cos(x) * pow(e, amplitude * sin(x) - 1);

        amplitude *= 0.82;
        frequency *= 1.1;
        speed *= 1.01;
    }

    // we divide by the sum of the amplitudes to normalize the wave. I don't know why this works
    pos.y = y - 20;

    //calculate normal
    Normal = normalize(vec3(-yx, 1.0, -yz));
    //Normal = normalize(aNormal);

    FragPos = vec3(model * vec4(pos, 1.0)); 
    gl_Position = projection * view * vec4(FragPos, 1.0);
}

// from https://www.shadertoy.com/view/4djSRW
float Random (vec2 seed) 
{
    return fract(sin(dot(seed.xy,vec2(12.9898,78.233))) * 43758.5453123);
}

vec2 GetRandomDirection(int waveIndex)
{
    vec2 dir = vec2(1);

    dir.x = 2.0 * Random(vec2(waveIndex, directionSeed)) - 1.0;
    dir.y = 2.0 * Random(vec2(waveIndex, directionSeed + 1)) - 1.0;

    return normalize(dir);
}