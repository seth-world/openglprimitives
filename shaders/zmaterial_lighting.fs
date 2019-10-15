#version 330 core
//out vec4 FragColor;

in Vertex_iblock {
    vec3 Position;
    vec3 Normal;
    vec2 TexCoords;
} fs_in ;

struct material_struct {
        vec3 Ambient;
        vec3 Diffuse;
        vec3 Specular;
        float Shininess;
        };

struct light_struct {
    vec3 Position;
    vec3 Ambient;
    vec3 Diffuse;
    vec3 Specular;
//        vec3 Color;
        };

uniform sampler2D   TextureSampler; // not used

//uniform vec3        lightPosition;
uniform  light_struct       light;
uniform  material_struct    material;
//uniform        vec3     materialAmbient;
//uniform        vec3     materialDiffuse;
//uniform        vec3     materialSpecular;
//uniform        float    materialShininess;

uniform vec3                viewPosition;
uniform vec3                DefaultColor;
uniform float               DefaultAlpha;

//----------Blinn-Phong flag (true : Blinn-Phong - false : Phong )
uniform bool        BlinnPhong;
// -------Usage flags----------------
uniform bool useTexture;                // flag for texture
uniform bool useDefaultColor;
uniform bool useDefaultAlpha;

void main()
{
    vec3 wColor = material.Ambient;


        if (useDefaultColor)
            wColor *= DefaultColor;

// ambient
    vec3 wAmbient = 0.05 * wColor; // old
//    vec3 wAmbient =  wColor * light.Color; // new1
//    vec3 wAmbient = light.Ambient * wColor; // new

// diffuse
    vec3 wLightDir = normalize(light.Position - fs_in.Position);
//    normalize(vec3(projection * vec4(normalMatrix * aNormal, 0.0)));
    vec3 wNormal = normalize(fs_in.Normal);


    float wDiff = max(dot(wLightDir, wNormal), 0.0);
//    vec3 wDiffuse = wDiff * wColor ;        // old
//    vec3 wDiffuse = wDiff * material.Diffuse * light.Diffuse ; // new
    vec3 wDiffuse = wDiff * material.Diffuse  ; // new
    // specular
    vec3 wViewDir = normalize(viewPosition - fs_in.Position);
    vec3 wReflectDir = reflect(-wLightDir, wNormal);
    float wSpec = 0.0;
    if(BlinnPhong)
    {
        vec3 wHalfwayDir = normalize(wLightDir + wViewDir);
        wSpec = pow(max(dot(wNormal, wHalfwayDir), 0.0), 32.0);
    }
    else
    {
        vec3 wReflectDir = reflect(-wLightDir, wNormal);
        wSpec = pow(max(dot(wViewDir, wReflectDir), 0.0), 8.0);
    }
//    vec3 wSpecular = vec3(0.3) * wSpec; // Old: assuming bright white light color
//    vec3 wSpecular = light.Color * wSpec; // new
//    vec3 wSpecular = light.Specular * wSpec * material.Specular; // new
    vec3 wSpecular =  wSpec * material.Specular;

    float wAlpha=1.0;
    if (useDefaultAlpha)
            wAlpha=DefaultAlpha;
//    if (useDefaultColor)
//    {
//    vec3 wFragColor= (wAmbient + wDiffuse + wSpecular) * DefaultColor;
//    gl_FragColor = vec4(wFragColor, wAlpha);
//    }
//    else
    gl_FragColor = vec4(wAmbient + wDiffuse + wSpecular, wAlpha);
//    FragColor = vec4(wAmbient + wDiffuse + wSpecular, wAlpha);

}
