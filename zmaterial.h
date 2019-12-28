#ifndef ZMATERIAL_H
#define ZMATERIAL_H

#ifdef __USE_GLAD__
#include <glad/glad.h>
#else
#include <GL/glew.h>
#endif

#include <glm/glm.hpp>

#include <stdio.h>  // for sprintf()

static char wDB[200];

class ZMaterial{
private:
    inline void _cloneFrom(const ZMaterial& pIn)
    {
        Ambient=pIn.Ambient;
        Diffuse=pIn.Diffuse;
        Specular=pIn.Specular;
        Shininess=pIn.Shininess;
        Name=pIn.Name;
    }
public:
    ZMaterial(const glm::vec3 pAmbient, const glm::vec3 pDiffuse,const glm::vec3 pSpecular,float pShininess,const char*pName=nullptr)
    {
        Name=pName;
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
    const char* Name=nullptr;

    char* display() const
    {
        sprintf (wDB,"<%10s>[Ambient<r:%4f g:%4f b:%4f> Diffuse<r:%4f g:%4f b:%4f> Specular<r:%4f g:%4f b:%4f> Shininess:%4f]",
                 Name,
                 Ambient.r,Ambient.g,Ambient.b,
                 Diffuse.r,Diffuse.g,Diffuse.b,
                 Specular.r,Specular.g,Specular.b,
                 Shininess);
        return wDB;
    }

};
class ZLight{
public:
    ZLight(const glm::vec3 pAmbient, const glm::vec3 pDiffuse,const glm::vec3 pSpecular,const char* pName)
    {
        Name=pName;
        Ambient=pAmbient;
        Diffuse=pDiffuse;
        Specular=pSpecular;
    }
    ZLight(ZLight &pIn)
    {
        Ambient=pIn.Ambient;
        Diffuse=pIn.Diffuse;
        Specular=pIn.Specular;
        Name=pIn.Name;
    }
    ZLight(ZLight &&pIn)
    {
        Ambient=pIn.Ambient;
        Diffuse=pIn.Diffuse;
        Specular=pIn.Specular;
        Name=pIn.Name;
    }
    ZLight& operator=(const ZLight& pIn)
    {
        Ambient=pIn.Ambient;
        Diffuse=pIn.Diffuse;
        Specular=pIn.Specular;
        Name=pIn.Name;
         return *this;
    }
    glm::vec3 Ambient;
    glm::vec3 Diffuse;
    glm::vec3 Specular;
    const char* Name;

    char* display() const
        {
        sprintf (wDB,"<%10s>[Ambient<r:%4f g:%4f b:%4f> Diffuse<r:%4f g:%4f b:%4f> Specular<r:%4f g:%4f b:%4f>]",
                 Name,
                 Ambient.r,Ambient.g,Ambient.b,
                 Diffuse.r,Diffuse.g,Diffuse.b,
                 Specular.r,Specular.g,Specular.b);
        return wDB;
        }

};
/*
 * http://devernay.free.fr/cours/opengl/materials.html
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
                           0.6*128.0,
                          "ZEmerald"};
const ZMaterial ZJade= {glm::vec3(0.135 ,	0.2225, 	0.1575),
                           glm::vec3(0.54    ,    0.89     ,   0.63  ),
                           glm::vec3(0.316228 ,	0.316228 ,	0.316228 ),
                           0.1*128.0,
                           "ZJade"
                          };
const ZMaterial ZPearl= {glm::vec3(0.25 ,	0.20725, 0.20725 ),
                           glm::vec3(1     ,      0.829    ,   0.829   ),
                           glm::vec3(0.296648 ,	0.296648 	,0.296648 ),
                           0.088*128.0,
                         "ZPearl"
                          };
const ZMaterial ZRuby= {glm::vec3( 0.1745 ,	0.01175 ,	0.01175 ),
                           glm::vec3(0.61424 , 0.04136 ,	0.04136  ),
                           glm::vec3(0.727811 ,	0.626959 ,	0.626959 ),
                           0.6*128.0,
                        "ZRuby"
                          };
const ZMaterial ZTurquoise ={glm::vec3( 0.1 ,	0.18725 ,	0.1745 ),
                           glm::vec3(0.396 ,	0.74151 ,	0.69102 ),
                           glm::vec3(0.297254 ,	0.30829 ,	0.306678  ),
                           0.1*128.0,
                             "ZTurquoise"
                          };
const ZMaterial ZBrass = {glm::vec3( 0.329412 ,	0.223529 ,	0.027451 ),
                           glm::vec3(0.780392 ,	0.568627 ,	0.113725 ),
                           glm::vec3(0.992157 ,	0.941176 ,	0.807843  ),
                           0.21794872*128.0,
                          "ZBrass"
                          };
const ZMaterial ZBronze = {glm::vec3(0.2125f,0.1275f,0.054f),
                         glm::vec3(0.714f,0.4284f,0.18144f),
                         glm::vec3(0.393548f,0.271906f,0.166721f),
                         0.2*128.0,
                           "ZBronze"
                        };
const ZMaterial ZChrome = {glm::vec3(0.25f,0.25f,0.25f),
                           glm::vec3(0.4f,0.4f,0.4f),
                           glm::vec3(0.774597f,0.774597f,0.774597f),
                           0.6f,
                           "ZChrome"
                          };
const ZMaterial ZCopper = {glm::vec3(0.19125f,0.0735f,0.0225f),
                           glm::vec3(0.7038f,0.27048f,0.0828f),
                           glm::vec3(0.256777f,0.137622f,0.086014f),
                           0.1*128.0,
                           "ZCopper"
                          };

const ZMaterial ZGold = {glm::vec3(0.24725,	0.1995 ,	0.0745 ),
                           glm::vec3(0.75164 ,	0.60648 ,	0.22648 ),
                           glm::vec3(0.628281 ,	0.555802 ,	0.366065 ),
                           0.4*128.0,
                         "ZGold"
                          };
const ZMaterial ZSilver = {glm::vec3(0.19225 ,	0.19225 ,	0.19225 ),
                           glm::vec3(0.50754 ,	0.50754 ,	0.50754  ),
                           glm::vec3(0.508273 ,	0.508273 ,	0.508273  ),
                           0.4*128.0,
                           "ZSilver"
                          };

const ZMaterial ZSphereMaterial (glm::vec3(0.5f, 0.5f, 0.5f),
                                   glm::vec3(0.7f, 0.7f, 0.7f),
                                   glm::vec3(0.4f, 0.4f, 0.4f),
                                   16.0,
                                 "ZSphereMat"
                                  );

#endif // ZMATERIAL_H
