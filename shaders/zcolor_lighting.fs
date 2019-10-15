#version 330 core
out vec4 FragColor;

in Vertex_iblock {
    vec3 Position;
    vec3 Normal;
    vec2 TexCoords;
} fs_in ;



uniform sampler2D   TextureSampler; // not used

//uniform vec3 lightPosition;
//uniform vec3 lightColor;
uniform struct light_struct {
    vec3 Position;      // mandatory
    vec3 Ambient;       // not used
    vec3 Diffuse;       // not used
    vec3 Specular;      // not used
    vec3 Color;         // mandatory
    } light;

uniform vec3    viewPosition;
uniform vec3    DefaultColor;
uniform float   DefaultAlpha;


struct material_struct {
    vec3 Ambient;           // material ambient color
    vec3 Diffuse;           // material diffuse color
    vec3 Specular;          // material specular color
    float Shininess;        // material specular shininess
    float DiffuseAlpha;
};

uniform material_struct material;

// -------Usage flags----------------
uniform bool useTexture;                // flag for texture
uniform bool useDefaultColor;
uniform bool useDefaultAlpha;

void main()
{
    // ambient
    float ambientStrength = 0.4;
    vec3 wAmbient = ambientStrength * DefaultColor;
//    vec3 wAmbient = material.Ambient * light.Color;
  	
    // diffuse 
    vec3 norm = normalize(fs_in.Normal);
    vec3 lightDir = normalize(light.Position - fs_in.Position);
    float diff = max(dot(norm, lightDir), 0.0);
 //   vec3 wDiffuse = diff * light.Color;
    vec3 wDiffuse = diff * vec3(0.6);
    // specular
    float specularStrength = 0.5;
    vec3 viewDir = normalize(viewPosition - fs_in.Position);
    vec3 reflectDir = reflect(-lightDir, norm);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 wSpecular = specularStrength * spec * DefaultColor;
//    vec3 wspecular = material.Specular * spec * light.Color;
        
    vec3 result = (wAmbient + wDiffuse + wSpecular) * DefaultColor;
    if (useDefaultAlpha)
        FragColor = vec4(result, DefaultAlpha);
        else
        FragColor = vec4(result,1.0f);
} 
