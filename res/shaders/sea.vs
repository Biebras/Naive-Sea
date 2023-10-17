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
uniform float initialAmplitude;
uniform float initialFrequency;
uniform float initialSpeed;
uniform float amplitudeIncrease;
uniform float frequencyIncrease;
uniform float speedIncrease;

out vec3 FragPos;
out vec3 Normal;

void main()
{
    vec3 pos = aPos;

    // constants
    float e = 2.71828;

    // wave parameters
    float amplitude = initialAmplitude;
    float frequency = initialFrequency;
    float speed = initialSpeed;

    //calculate y and derivatives for normals
    float y = 0.0;
    float dx = 0.0;
    float dz = 0.0;

    float lastDx = 0.0;
    float lastDz = 0.0;


    for(int i = 0; i < numWaves; i++)
    {
        vec2 dir = GetRandomDirection(i);

        // domain warping. Makes crests more pointy
        pos.x += lastDx * 0.02;
        pos.z += lastDz * 0.02;

        // calculate input for wave equation
        float x = dot(dir, pos.xz) * frequency + time * speed;

        // wave equation
        y += pow(e, amplitude * sin(x));
        
        // derivative respect to x and z
        dx += amplitude * frequency * pow(e, sin(x)) * dir.x * cos(x);
        dz += amplitude * frequency * pow(e, sin(x)) * dir.y * cos(x);

        lastDx = dx;
        lastDz = dz;
        amplitude *= amplitudeIncrease;
        frequency *= frequencyIncrease;
        speed *= speedIncrease;
    }

    // we minus numWaves to make the ocean stay at the same y through the change of numWaves. I don't know why this works...
    pos.y = y - numWaves;

    //calculate normal
    Normal = normalize(vec3(-dx, 1.0, -dz));

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