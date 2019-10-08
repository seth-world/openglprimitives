// GLSL version
#version 330 core


struct light_struct {
    vec3 Position;             // should be in the eye space
    vec3 Ambient;              // light ambient color
    vec3 Diffuse;              // light diffuse color
    vec3 Specular;             // light specular color

    vec3    Color;
};

struct material_struct {
    vec3 Ambient;           // material ambient color
    vec3 Diffuse;           // material diffuse color
    vec3 Specular;          // material specular color
    float Shininess;        // material specular shininess
    float DiffuseAlpha;
};
// uniforms

uniform light_struct        light;
uniform material_struct     material;

uniform sampler2D map0;                 // texture map #1

uniform vec3 InColor;
uniform float InAlpha;

// -------Usage flags----------------
uniform bool useTexture;                // flag for texture
uniform bool useInColor;
uniform bool useInAlpha;

// varyings (input from vertex shader)
varying vec3 esVertex, esNormal;
varying vec2 texCoord0;
void main()
{
    vec3 wNormal = normalize(esNormal);
    vec3 wLight;
 //   if(light.Position.w == 0.0)
 //   {
 //       wLight = normalize(light.Position.xyz);
 //   }
 //   else
 //   {
        wLight = normalize(light.Position.xyz - esVertex);
 //   }
    vec3 wView = normalize(-esVertex);
    vec3 wHalfv = normalize(wLight + wView);

    vec3 wColor = light.Ambient.rgb * material.Ambient.rgb ;        // begin with ambient
    float wDotNL = max(dot(wNormal, wLight), 0.0);
    wColor += light.Diffuse.rgb * material.Diffuse.rgb * wDotNL;    // add diffuse
    if(useTexture)
        {
        wColor *= texture2D(map0, texCoord0).rgb; // modulate texture map
        if (useInColor)
            wColor *= InColor;
        }
        else
        {
        if (useInColor)
                wColor = InColor;
                else
                wColor = vec3(0.5f, 0.8f, 1.0f);
        }
    float dotNH = max(dot(wNormal, wHalfv), 0.0);
    wColor += pow(dotNH, material.Shininess) * light.Specular.rgb * material.Specular.rgb; // add specular

    float wAlpha=material.DiffuseAlpha;
 //   float wAlpha=1.0f;
    if (useInAlpha)
            wAlpha*=InAlpha;

    // set frag color
//    gl_FragColor = vec4(wColor, material.Diffuse.a);
    gl_FragColor = vec4(wColor, wAlpha);
}
