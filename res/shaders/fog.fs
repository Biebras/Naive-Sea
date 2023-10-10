#version 330 core

out vec4 fragColor;
in vec2 texCoords;

uniform sampler2D screenTexture;

uniform vec2 screenResolution;

void main()
{
    fragColor = texture(screenTexture, texCoords);
}
