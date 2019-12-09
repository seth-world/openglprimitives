#version 330 core
in vec2 TextureCoords;

uniform sampler2D TextureSampler;
uniform float Alpha;
uniform vec3 DefaultColor;
uniform bool UseTexture;
void main(void)
{
    if (UseTexture)
        gl_FragColor = vec4(texture(TextureSampler, TextureCoords).rgb,Alpha);
    else
        gl_FragColor = vec4(DefaultColor,Alpha);
}
