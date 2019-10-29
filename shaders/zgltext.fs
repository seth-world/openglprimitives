#version 330 core
in vec2 TextureCoords;
out vec4 Color;

uniform sampler2D TextureSampler;
uniform vec3 TextColor;

void main()
{    
    vec4 wSampled = vec4(1.0, 1.0, 1.0, texture(TextureSampler, TextureCoords).r);
    Color = vec4(TextColor, 1.0) * wSampled;
}  
