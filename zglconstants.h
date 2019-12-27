#ifndef ZGLCONSTANTS_H
#define ZGLCONSTANTS_H

#include <glm/glm.hpp>

#define Vertice_type glm::vec3
#define Color_type glm::vec3
#define TexCoords_type glm::vec2


#define __TEXT_SHADER__             "TextShader"
#define __TEXTBOX_SHADER_SHAPE__    "BoxShaderShape"
#define __TEXTBOX_SHADER_FILL__     "BoxShaderFill"

#define __SHD_POSITION_ATTR__   "aPosition"
#define __SHD_NORMAL_ATTR__     "aNormal"
#define __SHD_TEXCOORDS_ATTR__  "aTexCoords"

#define __SHD_TEXTURESAMPLER__  "TextureSampler"
#define __SHD_USE_TEXTURE_UN__  "useTexture"
#define __SHD_TEXTCOLOR_UN__    "TextColor"
#define __SHD_DEFAULTCOLOR_UN__ "DefaultColor"

#define __SHD_ALPHA_UN__        "Alpha"

/* z position (depth) for text : set to shader under this name as uniform */
#define __SHD_TEXTPOSZ_UN__ "TextPosZ"
#define __SHD_BOXPOSZ_UN__  "BoxPosZ"

#define __SHD_MODEL__       "mModel"
#define __SHD_VIEW__        "mView"
#define __SHD_PROJECTION__  "mProjection"
#define __SHD_NORMAL__      "mNormal"

/*static constexpr int Draw           = 0;
static constexpr int Shape          = 1;
static constexpr int NormVisu       = 2;
static constexpr int UserDefined    = 3;
static constexpr int MaxShaderContext=4;
*/
enum DrawContext_type : int
{
    Draw  = 0,
    Fill  = 0,
    Shape = Draw+1,
    NormVisu = Shape+1,
    UserDefined=NormVisu+1,
    MaxShaderContext=UserDefined+1
};

/* setupGL input : setup and computation rules for vertexes and indices */

enum CoordsSetupOpt : uint16_t
{
    CSO_setupVertices       =   0,/* set up vertices (default) and possibly indexes to GL Buffer : done each time setupGL is invoked */
    CSO_setupNormals        =  0x01,/* set up normal coords to GL Buffer  : this does not trigger normal computation :  */
    CSO_setupTextures       =  0x02,/* set up texture coords to GL Buffer : this does not trigger texture coordinates computation : texture coords are given */

    CSO_ComputeNormals      =  0x10, /* force Normal coordinates to be calculated and filled in Vertex Data */
    CSO_ComputeTextures     =  0x20, /* force Texture coordinates to be calculated and filled in Vertex Data */

    setupShape              =   4,
    setupNormVisu           =   8,

    CSO_DeleteDataAfter     =   0x1000,     /* if set, force all vertex data to be deleted after GL buffers are set up */

    CSO_setupAll            =   CSO_setupVertices+CSO_setupNormals+CSO_setupTextures,/* set up vertices, normals and texture coordinates to GL Buffer */

    CSO_setAndComputeNormals    = CSO_setupNormals | CSO_ComputeNormals,   /* compute normals fill them within vertex data and set them up  to GL Buffer */
    CSO_setAndComputeTextures   = CSO_setupTextures | CSO_ComputeTextures, /* compute normals fill them within vertex data and set them up  to GL Buffer */
    CSO__setAndComputeAll       = CSO_setupTextures | CSO_setupNormals | CSO_ComputeTextures | CSO_ComputeNormals

};

enum MatGen : uint8_t
{
    MAT_Model       =      1,
    MAT_View        =   0x02,
    MAT_Projection  =   0x04,
    MAT_Normal      =   0x08,
    MAT_All         =   MAT_Model | MAT_View | MAT_Projection | MAT_Normal
};



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
