#ifndef ZGLCONSTANTS_H
#define ZGLCONSTANTS_H

#include <glm/glm.hpp>

#define Vertice_type glm::vec3
#define Color_type glm::vec3
#define TexCoords_type glm::vec2


#define   __POSITION_ATR_NAME__ "aPosition"
#define __NORMAL_ATR_NAME__ "aNormal"
#define __TEXCOORDS_ATR_NAME__ "aTexCoord"

#define __TEXTURESAMPLER__ "TextureSampler"

#define __TEXTCOLOR__   "TextColor"
/* z position (depth) for text : set to shader under this name as uniform */
#define __TEXTPOSZ__ "TextPosZ"
#define __BOXPOSZ__ "BoxPosZ"

#define __MODEL__   "mModel"
#define __VIEW__   "mView"
#define __PROJECTION__   "mProjection"

const unsigned int cst_defaultPositionLocation = 0;
const unsigned int cst_defaultNormalLocation = 1;
const unsigned int cst_defaultTexCoordsLocation = 2;

const Color_type ZBlackColor(0.0f, 0.0f, 0.0f);
const Color_type ZWhiteColor(1.0f, 1.0f, 1.0f);

const Color_type ZBlueColor(0.5f, 0.8f, 1.0f);
const Color_type ZBlue1Color(0.2f, 0.0f, 1.0f);

const Color_type ZGreyColor(0.8, 0.8, 0.8);

const Color_type ZRedMedium(0.952, 0.207, 0.207);

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

/* Replacement char : used when character code in out of range */
#define __REPLACEMENT_CHAR__ 0x3F




#endif // ZGLCONSTANTS_H
