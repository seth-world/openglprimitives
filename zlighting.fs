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

uniform sampler2D   TextureSampler;

//uniform vec3        lightPosition;
uniform  light_struct       light;
uniform  material_struct    material;
//uniform        vec3     materialAmbient;
//uniform        vec3     materialDiffuse;
//uniform        vec3     materialSpecular;
//uniform        float    materialShininess;

uniform vec3                viewPosition;
uniform vec3                InColor;
uniform float               InAlpha;

//----------Blinn-Phong flag (true : Blinn-Phong - false : Phong )
uniform bool        BlinnPhong;
// -------Usage flags----------------
uniform bool useTexture;                // flag for texture
uniform bool useInColor;
uniform bool useInAlpha;

void main()
{
    vec3 wColor = vec3(0.5f, 0.8f, 1.0f);

    if(useTexture)
        {
        wColor =    texture(TextureSampler, fs_in.TexCoords).rgb;
        if (useInColor)
            wColor *= InColor;
//            wColor *= material.Ambient;

        }
        else
        {
        if (useInColor)
                wColor = InColor;
                else
                wColor = vec3(0.5f, 0.8f, 1.0f); // default color if nothing more available
        }
    // ambient
    vec3 wAmbient = 0.05 * wColor; // old
//    vec3 wAmbient =  wColor * light.Color; // new1
//    vec3 wAmbient = light.Ambient * wColor; // new

// diffuse
    vec3 wLightDir = normalize(light.Position - fs_in.Position);
    vec3 wNormal = normalize(fs_in.Normal);
    float wDiff = max(dot(wLightDir, wNormal), 0.0);
    vec3 wDiffuse = wDiff * wColor ;        // old
//    vec3 wDiffuse = wDiff * light.Diffuse ; // new
//    wDiffuse *= wColor; // added
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
    vec3 wSpecular = vec3(0.3) * wSpec; // Old: assuming bright white light color
//    vec3 wSpecular = light.Color * wSpec; // new
//    vec3 wSpecular = light.Specular * wSpec ; // new


    float wAlpha=1.0;
    if (useInAlpha)
            wAlpha=InAlpha;
//    if (useInColor)
//    {
//    vec3 wFragColor= (wAmbient + wDiffuse + wSpecular) * InColor;
//    gl_FragColor = vec4(wFragColor, wAlpha);
//    }
//    else
    gl_FragColor = vec4(wAmbient + wDiffuse + wSpecular, wAlpha);
//    FragColor = vec4(wAmbient + wDiffuse + wSpecular, wAlpha);

}
