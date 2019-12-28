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

uniform float   ambientStrength;

//----------Blinn-Phong flag (true : blinn-Phong - false : Phong )
uniform bool        blinnPhong;
// -------Usage flags----------------
uniform bool useDefaultColor;           // not used : always use defaultColor as input
uniform bool useDefaultAlpha;
uniform bool useAmbientStrength;

void main()
{
    // ambient
    float wAmbientStrength = 0.4;
    if (useAmbientStrength)
            wAmbientStrength=ambientStrength;

    vec3 wAmbient = wAmbientStrength * defaultColor;
//    vec3 wAmbient = material.Ambient * light.Color;
  	
    // diffuse 
    vec3 wNormal = normalize(fs_in.Normal);

    vec3 wLightDir = normalize(lightPosition - fs_in.Position);
    float wDiff = max(dot(wLightDir,wNormal), 0.0);
 //   vec3 wDiffuse = diff * light.Color;
    vec3 wDiffuse = wDiff * vec3(0.6);
    // specular
    float wSpecularStrength = 0.5;

    vec3 wViewDir = normalize(viewPosition - fs_in.Position);

    vec3 wReflectDir = reflect(-wLightDir, wNormal);
    float wSpec = 0.0;
    if(blinnPhong)
    {
        vec3 wHalfwayDir = normalize(wLightDir + wViewDir);
        wSpec = pow(max(dot(wNormal, wHalfwayDir), 0.0), 32.0);
    }
    else
    {
        vec3 wReflectDir = reflect(-wLightDir, wNormal);
        wSpec = pow(max(dot(wViewDir, wReflectDir), 0.0), 8.0);
    }

//    float spec = pow(max(dot(viewDir, wReflectDir), 0.0), 32);

    vec3 wSpecular = wSpecularStrength * wSpec * defaultColor;
//    vec3 wspecular = material.Specular * spec * light.Color;
        
    vec3 result = (wAmbient + wDiffuse + wSpecular) * defaultColor;

    float wAlpha=1.0;
    if (useDefaultAlpha)
       wAlpha=defaultAlpha;

    gl_FragColor = vec4(result,wAlpha);
} 
