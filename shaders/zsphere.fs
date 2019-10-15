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

uniform sampler2D TextureSampler;                 // texture map #1

uniform vec3                viewPosition;
uniform vec3                DefaultColor;
uniform float               DefaultAlpha;


// -------Usage flags----------------
uniform bool useTexture;                // flag for texture
uniform bool useDefaultColor;
uniform bool useDefaultAlpha;

// varyings (input from vertex shader)
varying vec3 esVertex, esNormal;
varying vec2 texCoord0;
void main()
{
    vec3 wNormal = normalize(esNormal);
    vec3 wLightDir;
 //   if(light.Position.w == 0.0)
 //   {
 //       wLightDir = normalize(light.Position.xyz);
 //   }
 //   else
 //   {
        wLightDir = normalize(light.Position.xyz - esVertex);
 //   }
    vec3 wView = normalize(-esVertex);
    vec3 wHalfv = normalize(wLightDir + wView);

    vec3 wColor = light.Ambient.rgb * material.Ambient.rgb ;        // begin with ambient
    float wDotNL = max(dot(wNormal, wLightDir), 0.0);
    wColor += light.Diffuse.rgb * material.Diffuse.rgb * wDotNL;    // add diffuse
    if(useTexture)
        {
        wColor *= texture2D(TextureSampler, texCoord0).rgb; // modulate texture map
        if (useDefaultColor)
            wColor *= DefaultColor;
        }
        else
        {
        if (useDefaultColor)
                wColor = DefaultColor;
                else
                wColor = vec3(0.5f, 0.8f, 1.0f);
        }
    float dotNH = max(dot(wNormal, wHalfv), 0.0);
    wColor += pow(dotNH, material.Shininess) * light.Specular.rgb * material.Specular.rgb; // add specular

    float wAlpha=material.DiffuseAlpha;
 //   float wAlpha=1.0f;
    if (useDefaultAlpha)
            wAlpha*=DefaultAlpha;

    // set frag color
//    gl_FragColor = vec4(wColor, material.Diffuse.a);
    gl_FragColor = vec4(wColor, wAlpha);
}
