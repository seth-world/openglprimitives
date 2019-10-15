#version 330 core
out vec4 FragColor;

in vec3 FragPos;  


uniform vec3 DefaultColor;
uniform float DefaultAlpha;


void main()
{
        FragColor = vec4(DefaultColor,DefaultAlpha);
} 
