#version 330 core
/* Do not modify attribute or uniform names
 * Respect naming standards :
 *   attribute name starts with 'a'
 *   uniform name starts with a lowercase
 *   internal variables names start with 'w'
 */
in vec2 TextureCoords;

uniform sampler2D   TextureSampler;
uniform vec3        textColor;

void main()
{    
//    vec4 wSampled = vec4(1.0, 1.0, 1.0, texture(TextureSampler, TextureCoords).r);
//texture(    gl_FragColor = vec4(textColor, 1.0) * wSampled;
//    gl_FragColor = vec4(textColor,texture(TextureSampler, TextureCoords).r);
    float wT=texture(TextureSampler, TextureCoords).r;

    if (wT >0.6)
            gl_FragColor =vec4(textColor,1.0);
    else
            gl_FragColor=vec4(textColor,0.0);

}  
