
// GLSL version
#version 110
// uniforms
uniform mat4 matrixModelView;
uniform mat4 matrixNormal;
uniform mat4 matrixModelViewProjection;
// vertex attribs (input)
attribute vec3 vertexPosition;
attribute vec3 vertexNormal;
attribute vec2 vertexTexCoord;
// varyings (output)
varying vec3 esVertex, esNormal;
varying vec2 texCoord0;
void main()
{
    esVertex = vec3(matrixModelView * vec4(vertexPosition, 1.0));
    esNormal = vec3(matrixNormal * vec4(vertexNormal, 1.0));
    texCoord0 = vertexTexCoord;
    gl_Position = matrixModelViewProjection * vec4(vertexPosition, 1.0);
}
)";

const char* fsSource = R"(
// GLSL version
#version 330 core
// uniforms
uniform vec4 lightPosition;             // should be in the eye space
uniform vec4 lightAmbient;              // light ambient color
uniform vec4 lightDiffuse;              // light diffuse color
uniform vec4 lightSpecular;             // light specular color
uniform vec4 materialAmbient;           // material ambient color
uniform vec4 materialDiffuse;           // material diffuse color
uniform vec4 materialSpecular;          // material specular color
uniform float materialShininess;        // material specular shininess
uniform sampler2D map0;                 // texture map #1
uniform bool textureUsed;               // flag for texture
// varyings
varying vec3 esVertex, esNormal;
varying vec2 texCoord0;
void main()
{
    vec3 normal = normalize(esNormal);
    vec3 light;
    if(lightPosition.w == 0.0)
    {
        light = normalize(lightPosition.xyz);
    }
    else
    {
        light = normalize(lightPosition.xyz - esVertex);
    }
    vec3 view = normalize(-esVertex);
    vec3 halfv = normalize(light + view);

    vec3 color = lightAmbient.rgb * materialAmbient.rgb;        // begin with ambient
    float dotNL = max(dot(normal, light), 0.0);
    color += lightDiffuse.rgb * materialDiffuse.rgb * dotNL;    // add diffuse
    if(textureUsed)
        color *= texture2D(map0, texCoord0).rgb;                // modulate texture map
    float dotNH = max(dot(normal, halfv), 0.0);
    color += pow(dotNH, materialShininess) * lightSpecular.rgb * materialSpecular.rgb; // add specular

    // set frag color
    gl_FragColor = vec4(color, materialDiffuse.a);
}
