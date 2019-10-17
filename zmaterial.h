#ifndef ZMATERIAL_H
#define ZMATERIAL_H
#include <glm/glm.hpp>

class ZMaterial{
private:
    inline void _cloneFrom(const ZMaterial& pIn)
    {
        Ambient=pIn.Ambient;
        Diffuse=pIn.Diffuse;
        Specular=pIn.Specular;
        Shininess=pIn.Shininess;
    }
public:
    ZMaterial(const glm::vec3 pAmbient, const glm::vec3 pDiffuse,const glm::vec3 pSpecular,float pShininess)
    {
        Ambient=pAmbient;
        Diffuse=pDiffuse;
        Specular=pSpecular;
        Shininess=pShininess;
    }
    ZMaterial(const ZMaterial &pIn)
    {
        _cloneFrom(pIn);
    }
    ZMaterial(const ZMaterial &&pIn)
    {
         _cloneFrom(pIn);
    }

    ZMaterial& operator=(const ZMaterial& pIn)
    {
         _cloneFrom(pIn);
         return *this;
    }



    glm::vec3 Ambient;
    glm::vec3 Diffuse;
    glm::vec3 Specular;
    float Shininess;
};
class ZLight{
public:
    ZLight(const glm::vec3 pAmbient, const glm::vec3 pDiffuse,const glm::vec3 pSpecular)
    {
        Ambient=pAmbient;
        Diffuse=pDiffuse;
        Specular=pSpecular;
    }
    ZLight(ZLight &pIn)
    {
        Ambient=pIn.Ambient;
        Diffuse=pIn.Diffuse;
        Specular=pIn.Specular;
    }
    ZLight(ZLight &&pIn)
    {
        Ambient=pIn.Ambient;
        Diffuse=pIn.Diffuse;
        Specular=pIn.Specular;
    }

    glm::vec3 Ambient;
    glm::vec3 Diffuse;
    glm::vec3 Specular;
};
/* http://devernay.free.fr/cours/opengl/materials.html
 *
Name        Ambient                 Diffuse                 Specular                                    Shininess
emerald 	0.0215 	0.1745 	0.0215 	0.07568 	0.61424 	0.07568 	0.633       0.727811 	0.633       0.6
jade        0.135 	0.2225 	0.1575 	0.54        0.89        0.63        0.316228 	0.316228 	0.316228 	0.1
obsidian 	0.05375 0.05 	0.06625 0.18275 	0.17        0.22525 	0.332741 	0.328634 	0.346435 	0.3
pearl       0.25 	0.20725 0.20725 1           0.829       0.829       0.296648 	0.296648 	0.296648 	0.088
ruby        0.1745 	0.01175 	0.01175 	0.61424 	0.04136 	0.04136 	0.727811 	0.626959 	0.626959 	0.6
turquoise 	0.1 	0.18725 	0.1745 	0.396 	0.74151 	0.69102 	0.297254 	0.30829 	0.306678 	0.1
brass       0.329412 	0.223529 	0.027451 	0.780392 	0.568627 	0.113725 	0.992157 	0.941176 	0.807843 	0.21794872
bronze      0.2125 	0.1275 	0.054 	0.714 	0.4284 	0.18144 	0.393548 	0.271906 	0.166721 	0.2
chrome      0.25 	0.25 	0.25 	0.4 	0.4 	0.4 	0.774597 	0.774597 	0.774597 	0.6
copper      0.19125 	0.0735 	0.0225 	0.7038 	0.27048 	0.0828 	0.256777 	0.137622 	0.086014 	0.1
gold        0.24725 	0.1995 	0.0745 	0.75164 	0.60648 	0.22648 	0.628281 	0.555802 	0.366065 	0.4
silver      0.19225 	0.19225 	0.19225 	0.50754 	0.50754 	0.50754 	0.508273 	0.508273 	0.508273 	0.4
black plastic 	0.0 	0.0 	0.0 	0.01 	0.01 	0.01 	0.50 	0.50 	0.50 	.25
cyan plastic 	0.0 	0.1 	0.06 	0.0 	0.50980392 	0.50980392 	0.50196078 	0.50196078 	0.50196078 	.25
green plastic 	0.0 	0.0 	0.0 	0.1 	0.35 	0.1 	0.45 	0.55 	0.45 	.25
red plastic 	0.0 	0.0 	0.0 	0.5 	0.0 	0.0 	0.7 	0.6 	0.6 	.25
white plastic 	0.0 	0.0 	0.0 	0.55 	0.55 	0.55 	0.70 	0.70 	0.70 	.25
yellow plastic 	0.0 	0.0 	0.0 	0.5 	0.5 	0.0 	0.60 	0.60 	0.50 	.25
black rubber 	0.02 	0.02 	0.02 	0.01 	0.01 	0.01 	0.4 	0.4 	0.4 	.078125
cyan rubber 	0.0 	0.05 	0.05 	0.4 	0.5 	0.5 	0.04 	0.7 	0.7 	.078125
green rubber 	0.0 	0.05 	0.0 	0.4 	0.5 	0.4 	0.04 	0.7 	0.04 	.078125
red rubber      0.05 	0.0 	0.0 	0.5 	0.4 	0.4 	0.7 	0.04 	0.04 	.078125
white rubber 	0.05 	0.05 	0.05 	0.5 	0.5 	0.5 	0.7 	0.7 	0.7 	.078125
yellow rubber 	0.05 	0.05 	0.0 	0.5 	0.5 	0.4 	0.7 	0.7 	0.04 	.078125
*/

const ZMaterial ZEmerald = {glm::vec3(0.0215f,0.1745f,0.0215f),
                           glm::vec3(0.07568f,0.61424f,0.07568f),
                           glm::vec3(0.07568f,0.61424f,0.07568f),
                           0.6f
                          };

const ZMaterial ZBronze = {glm::vec3(0.0215f,0.1745f,0.0215f),
                           glm::vec3(0.07568f,0.61424f,0.07568f),
                           glm::vec3(0.07568f,0.61424f,0.07568f),
                           0.6f
                          };
const ZMaterial ZChrome = {glm::vec3(0.25f,0.25f,0.25f),
                           glm::vec3(0.4f,0.4f,0.4f),
                           glm::vec3(0.774597f,0.774597f,0.774597f),
                           0.6f
                          };
const ZMaterial ZSphereMaterial (glm::vec3(0.5f, 0.5f, 0.5f),
                                   glm::vec3(0.7f, 0.7f, 0.7f),
                                   glm::vec3(0.4f, 0.4f, 0.4f),
                                   16.0f
                                  );

#endif // ZMATERIAL_H
