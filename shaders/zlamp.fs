#version 330 core
uniform vec3  defaultColor;
uniform bool  useAlpha;
uniform float defaultAlpha;
void main()
{
    float wAlpha=1.0f;
    if (useAlpha)
            wAlpha=defaultAlpha;

    gl_FragColor = vec4(defaultColor,wAlpha);
}
