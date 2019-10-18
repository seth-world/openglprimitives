#ifndef ZGLCONSTANTS_H
#define ZGLCONSTANTS_H

#include <glad/glad.h>

#include <glm/glm.hpp>

#define Vertice_type glm::vec3
#define Color_type glm::vec3
#define TexCoords_type glm::vec2


#define   __POSITION_ATR_NAME__ "aPosition"
#define __NORMAL_ATR_NAME__ "aNormal"
#define __TEXCOORDS_ATR_NAME__ "aTexCoord"

const GLuint cst_defaultPositionLocation = 0;
const GLuint cst_defaultNormalLocation = 1;
const GLuint cst_defaultTexCoordsLocation = 2;

const Color_type ZBlueColor(0.5f, 0.8f, 1.0f);
const Color_type ZGreyColor(0.8, 0.8, 0.8);

const Color_type ZWhiteAmbient(0.125f,0.125f,0.125f);
const Color_type ZWhiteDiffuse(0.894, 0.898, 0.898);
const Color_type ZWhiteSpecular(1.0f,1.0f,1.0f);

const Color_type ZYellowAmbient(0.025f, 0.025f, 0.0f);
const Color_type ZYellowDiffuse(0.25f, 0.25f, 0.01f);
const Color_type ZYellowSpecular(0.6f,0.6f,0.2f);

const Color_type ZYellowBright(252.0f/255.0f,252.0f/255.0f,0.2f/255.0f);
const Color_type ZGreySilver(222.0f/255.0f ,222.0f/255.0f ,222.0f/255.0f );
const Color_type cst_object_color_default=ZBlueColor;

const Vertice_type ZModelOrigin=Vertice_type(0.0,0.0,0.0);






#endif // ZGLCONSTANTS_H
