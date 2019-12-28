#version 330 core
//out vec4 FragColor;

in Vertex_iblock {
    vec3 Position;
    vec3 Normal;
    vec2 TexCoords;
} fs_in ;

uniform sampler2D   TextureSampler; // not used

//uniform vec3 lightPosition;
//uniform vec3 lightColor;
uniform struct light_struct {
    vec3 Ambient;       // not used
    vec3 Diffuse;       // not used
    vec3 Specular;      // not used
    vec3 Color;         // mandatory
    } light;

uniform struct material_struct {
    vec3 Ambient;
    vec3 Diffuse;
    vec3 Specular;
    float Shininess;
} material;

uniform vec3    viewPosition;
uniform vec3    lightPosition;
uniform vec3    defaultColor;
uniform float   defaultAlpha;
// -------Usage flags----------------
uniform bool useTexture;                // flag for texture
uniform bool useDefaultColor;
uniform bool useDefaultAlpha;

void main()
{
    // ambient
    float wAmbientStrength = 0.4;
    vec3 wAmbient = wAmbientStrength * defaultColor;
//    vec3 wAmbient = material.Ambient * light.Color;
  	
    // diffuse 
    vec3 norm = normalize(fs_in.Normal);
    vec3 lightDir = normalize(lightPosition - fs_in.Position);
    float diff = max(dot(norm, lightDir), 0.0);
 //   vec3 wDiffuse = diff * light.Color;
    vec3 wDiffuse = diff * vec3(0.6);
    // specular
    float wSpecularStrength = 0.5;
    vec3 viewDir = normalize(viewPosition - fs_in.Position);
    vec3 wReflectDir = reflect(-lightDir, norm);


    float spec = pow(max(dot(viewDir, wReflectDir), 0.0), 32);

    vec3 wSpecular = wSpecularStrength * spec * defaultColor;
//    vec3 wspecular = material.Specular * spec * light.Color;
        
    vec3 result = (wAmbient + wDiffuse + wSpecular) * defaultColor;
    if (useDefaultAlpha)
        gl_FragColor = vec4(result, defaultAlpha);
        else
        gl_FragColor = vec4(result,1.0f);
} 
