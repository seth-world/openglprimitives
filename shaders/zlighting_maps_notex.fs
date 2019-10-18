#version 330 core

// from 4.1.light_maps.fs

out vec4 FragColor;

uniform vec3 InColor ;
uniform float InAlpha ;

struct Material {
    vec3 ambiant;
    vec3 diffuse; // diffuse is defined according material characteristics see zmaterial.h (http://devernay.free.fr/cours/opengl/materials.html)
//    sampler2D diffuse;
    vec3 specular;    
    float shininess;
}; 

struct Light {
    vec3 position;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

in vec3 FragPos;  
in vec3 Normal;  
in vec2 TexCoords;
  
uniform vec3 viewPos;
uniform Material material;
uniform Light light;


void main()
{
    // ambient
//    vec3 ambient = light.ambient * texture(material.diffuse, TexCoords).rgb;

    vec3 color = vec3(0.5f, 0.8f, 1.0f);
//    vec3 ambient = light.ambient ;
    vec3 ambient =  Material.ambiant * light.ambiant;
    
    
    
    // diffuse 
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(light.position - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
//    vec3 diffuse = light.diffuse * diff * texture(material.diffuse, TexCoords).rgb;  
    vec3 diffuse = light.diffuse * diff * material.diffuse ; 
    // specular
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = light.specular * (spec * material.specular);  
    vec3 result = (ambient + diffuse + specular) * color;       
 //   vec3 result = (ambient + diffuse + specular) + color ;
//    FragColor = vec4(result, InAlpha);
    FragColor = vec4(result, InAlpha);
} 
