#version 330 core
out vec4 FragColor;

in vec3 Normal;  
in vec3 FragPos;  

uniform vec3 lightPosition;
uniform vec3 viewPosition;
uniform vec3 lightColor;
//uniform vec3 objectColor;
uniform vec3 DefaultColor;
uniform float DefaultAlpha;


struct material_struct {
    vec3 Ambient;           // material ambient color
    vec3 Diffuse;           // material diffuse color
    vec3 Specular;          // material specular color
    float Shininess;        // material specular shininess
    float DiffuseAlpha;
};
// -------Usage flags----------------
uniform bool useTexture;                // flag for texture
uniform bool useDefaultColor;
uniform bool useDefaultAlpha;

void main()
{
    // ambient
//    float ambientStrength = 0.4;
//    vec3 ambient = ambientStrength * lightColor;
    vec3 wAmbient = materialAmbient * lightColor;
  	
    // diffuse 
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPosition - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 wdiffuse = diff * lightColor;
    
    // specular
    float specularStrength = 0.5;
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
//    vec3 specular = specularStrength * spec * lightColor;  
    vec3 wspecular = specular * spec * lightColor;  
        
    vec3 result = (wAmbient + wdiffuse + wspecular) * DefaultColor;
    if (useDefaultAlpha)
        FragColor = vec4(result, DefaultAlpha);
        else
        FragColor = vec4(result,1.0f);
} 
