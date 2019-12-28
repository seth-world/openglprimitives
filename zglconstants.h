#ifndef ZGLCONSTANTS_H
#define ZGLCONSTANTS_H

#include <glm/glm.hpp>

#define Vertice_type glm::vec3
#define Color_type glm::vec3
#define TexCoords_type glm::vec2

/* shaders standard internal names */
#define __TEXT_SHADER__             "TextShader"
#define __TEXTBOX_SHADER_SHAPE__    "BoxShaderShape"
#define __TEXTBOX_SHADER_FILL__     "BoxShaderFill"

/* Shaders standard attribute names for coordinates */
#define __SHD_POSITION_ATTR__       "aPosition"
#define __SHD_NORMAL_ATTR__         "aNormal"
#define __SHD_TEXCOORDS_ATTR__      "aTexCoords"
/* attribute for text coordinates, vec4 <x,y> text coordinates - <s,t> texture coordinates */
#define __SHD_TEXT_COORDS_ATTR__    "aTextCoords"

/* Shaders standard uniform names */
#define __SHD_TEXTURESAMPLER__  "TextureSampler"
#define __SHD_USE_TEXTURE_UN__  "useTexture"

#define __SHD_USECOLOR_UN__     "useDefaultColor"
#define __SHD_DEFAULTCOLOR_UN__ "defaultColor"

#define __SHD_USE_ALPHA_UN__    "useDefaultAlpha"
#define __SHD_ALPHA_UN__        "defaultAlpha"

#define __SHD_VIEW_POSITION_UN__    "viewPosition"
#define __SHD_LIGHT_POSITION_UN__   "lightPosition"


#define __SHD_AMB_STRGTH_UN__       "ambientStrength"
#define __SHD_USE_AMB_STRGTH_UN__   "useAmbientStrength"

#define __SHD_BLINNPHONG_UN__       "blinnPhong"

#define __SHD_MAT_AMBIENT_UN__      "material.Ambient"
#define __SHD_MAT_DIFFUSE_UN__      "material.Diffuse"
#define __SHD_MAT_SPECULAR_UN__     "material.Specular"
#define __SHD_MAT_SHININESS_UN__    "material.Shininess"

/* below uniform names are not currently used */
#define __SHD_LIGHT_AMBIENT_UN__      "light.Ambient"
#define __SHD_LIGHT_DIFFUSE_UN__      "light.Diffuse"
#define __SHD_LIGHT_SPECULAR_UN__     "light.Specular"
#define __SHD_LIGHT_SHININESS_UN__    "light.Shininess"

/* text shader */
#define __SHD_TEXTCOLOR_UN__    "textColor"
/* z position (depth) for text : set to shader under this name as uniform */
#define __SHD_TEXTPOSZ_UN__ "textPosZ"
#define __SHD_BOXPOSZ_UN__  "boxPosZ"

/* matrices */
#define __SHD_MODEL__       "mModel"
#define __SHD_VIEW__        "mView"
#define __SHD_PROJECTION__  "mProjection"
#define __SHD_NORMAL__      "mNormal"
/*
--------------------------------------------
 Fragment shaders attributes and related uniform values
--------------------------------------------
   zlamp.fs :  (works with zlamp.vs)  basic shader
   ----------

attributes (vertex shader):
    __SHD_POSITION_ATTR__           vec3 <x,y,z>
matrices:
uses 3 matrices :
    __SHD_MODEL__       mModel
    __SHD_VIEW__        mView
    __SHD_PROJECTION__  mProjection

    (mNormal is not used and not defined)

uniforms (fragment shader):
    __SHD_DEFAULTCOLOR_UN__     mandatory

    __SHD_USE_ALPHA_UN__        optional use of mentionned alpha value (opengl defaults to zero = false)
    __SHD_ALPHA_UN__            optional alpha value    (opengl defaults to zero)


    zcolor.fs : (works with zlighting.vs) use of mentionned color
    -----------

attributes (vertex shader):
    __SHD_POSITION_ATTR__           vec3 <x,y,z>
    __SHD_NORMAL_ATTR__             vec3 <x,y,z>
    __SHD_TEXCOORDS_ATTR__          vec2 <s,t>

matrices :
    __SHD_MODEL__       mModel
    __SHD_VIEW__        mView
    __SHD_PROJECTION__  mProjection
    __SHD_NORMAL__      mNormal

uniforms (fragment shader):
    __SHD_DEFAULTCOLOR_UN__     mandatory

    __SHD_USE_ALPHA_UN__
    __SHD_ALPHA_UN__


    __SHD_VIEW_POSITION_UN__
    __SHD_LIGHT_POSITION_UN__
    __SHD_BLINNPHONG_UN__


    ztexture.fs : (works with zlighting.vs) use of texture
    -------------

attributes (vertex shader):
    __SHD_POSITION_ATTR__           vec3 <x,y,z>
    __SHD_NORMAL_ATTR__             vec3 <x,y,z>
    __SHD_TEXCOORDS_ATTR__          vec2 <s,t>

matrices :
    __SHD_MODEL__       mModel
    __SHD_VIEW__        mView
    __SHD_PROJECTION__  mProjection
    __SHD_NORMAL__      mNormal

uniforms (fragment shader):

    __SHD_USECOLOR_UN__         if set to true will shade given default color with texture
    __SHD_DEFAULTCOLOR_UN__     optional

    __SHD_USE_ALPHA_UN__        optional, Alpha value is set to 1.0f by default
    __SHD_ALPHA_UN__

    __SHD_VIEW_POSITION_UN__    Mandatory
    __SHD_LIGHT_POSITION_UN__   Mandatory
    __SHD_BLINNPHONG_UN__       Optional ; if set to true Blinn effect, else Phong effect

    __SHD_USE_AMB_STRGTH_UN__   Optional ; if set Ambient strength booster is applied. If not default applies.
    __SHD_AMB_STRGTH_UN__       float : Ambient strenght booster. Defaulted to 0.4


    __SHD_TEXTURESAMPLER__      Mandatory loaded with GL_TEXTUREx as int

    zmaterial.fs : (works with zlighting.vs) use of material
    --------------

attributes (vertex shader):
    __SHD_POSITION_ATTR__           vec3 <x,y,z>
    __SHD_NORMAL_ATTR__             vec3 <x,y,z>
    __SHD_TEXCOORDS_ATTR__          vec2 <s,t>
matrices :
    __SHD_MODEL__       mModel
    __SHD_VIEW__        mView
    __SHD_PROJECTION__  mProjection
    __SHD_NORMAL__      mNormal

uniforms (fragment shader):
    __SHD_USECOLOR_UN__         if set to true will shade given default color with Material Ambient value
    __SHD_DEFAULTCOLOR_UN__     optional, Mandatory if

    __SHD_USE_ALPHA_UN__        optional, Alpha value is set to 1.0f by default
    __SHD_ALPHA_UN__

    __SHD_VIEW_POSITION_UN__    Mandatory
    __SHD_LIGHT_POSITION_UN__   Mandatory
    __SHD_BLINNPHONG_UN__       Optional ; if set to true Blinn effect, else Phong effect

    __SHD_USE_AMB_STRGTH_UN__   Optional ; if set Ambient strength booster is applied. If not default applies.
    __SHD_AMB_STRGTH_UN__       float : Ambient strenght booster. Defaulted to 0.7

   material characteristics structure

   __SHD_MAT_AMBIENT_UN__       Mandatory
   __SHD_MAT_DIFFUSE_UN__       Mandatory
   __SHD_MAT_SPECULAR_UN__      Mandatory
   __SHD_MAT_SHININESS_UN__     Mandatory


   ------------Text--------------------------

   zgltext.fs : works with zgltext.vs
   ------------
attributes for vertex shader:
    __SHD_TEXT_COORDS_ATTR__    vec4 <x,y> text coordinates - <s,t> texture coordinates

uniforms:
   __SHD_TEXTCOLOR_UN__         Mandatory
   __SHD_TEXTPOSZ_UN__          Mandatory : text z position vs box (box is positionned with z=0.0)
   __SHD_TEXTURESAMPLER__       Mandatory loaded with GL_TEXTUREx as int
matrices:
uses 3 matrices :
    __SHD_MODEL__       mModel
    __SHD_VIEW__        mView
    __SHD_PROJECTION__  mProjection

    (mNormal is not used and not defined)

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
