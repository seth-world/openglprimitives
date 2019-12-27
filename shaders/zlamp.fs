#version 330 core
out vec4 FragColor;
uniform vec3 DefaultColor;
uniform float DefaultAlpha;
void main()
{
    FragColor = vec4(DefaultColor,DefaultAlpha); // set all 4 vector values to 1.0
}
