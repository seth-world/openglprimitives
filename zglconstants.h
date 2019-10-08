#ifndef ZGLCONSTANTS_H
#define ZGLCONSTANTS_H


#include <glm/glm.hpp>
#define Vertice_type glm::vec3
#define Color_type glm::vec3
#define TextCoords_type glm::vec2


const Color_type ZBlueColor(0.5f, 0.8f, 1.0f);
const Color_type ZGreyColor(0.8, 0.8, 0.8);

const Color_type ZWhiteAmbient(0.125f,0.125f,0.125f);
const Color_type ZWhiteDiffuse(0.894, 0.898, 0.898);
const Color_type ZWhiteSpecular(1.0f,1.0f,1.0f);

const Color_type ZYellowAmbient(0.025f, 0.025f, 0.0f);
const Color_type ZYellowDiffuse(0.25f, 0.25f, 0.01f);
const Color_type ZYellowSpecular(0.6f,0.6f,0.2f);

const Color_type ZYellowBright(0.8f,0.8f,0.2f);

const Color_type cst_object_color_default=ZBlueColor;



#endif // ZGLCONSTANTS_H
