#ifndef ZGLCONSTANTS_H
#define ZGLCONSTANTS_H

#include <glm/glm.hpp>
#include <string> // for std::string

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
typedef uint16_t CSO_type ;
enum CoordsSetupOpt : CSO_type
{
    /* note : normal and texture coordinates are always computed and setup within Draw context */
    CSO_setupVertices           =   0,/* set up vertices (default) and possibly indexes to GL Buffer : done each time setupGL is invoked */
    CSO_setupNormals            =  0x0001,/* set up normal coords to GL Buffer  : this does not trigger normal computation :  */
    CSO_setupTextures           =  0x0002,/* set up texture coords to GL Buffer : this does not trigger texture coordinates computation : texture coords are given */

    CSO_ComputeNormals          =  0x0100, /* force Normal coordinates to be calculated and filled in Vertex Data */
    CSO_ComputeTextures         =  0x0200, /* force Texture coordinates to be calculated and filled in Vertex Data */

    /* contexts to setup (Draw,Shape, NormVisu, UserDefined */
    /* note : Draw context is always set up */
    CSO_setupShape              =  0x0004,
    CSO_setupNormVisu           =  0x0008,
    CSO_setupUser               =  0x0010,

    CSO_DeleteDataAfter         =  0x1000,  /* if set, force all vertex data & index to be deleted after GL buffers have been set up */

    CSO_setupAll                =   CSO_setupVertices+CSO_setupNormals+CSO_setupTextures,/* set up vertices, normals and texture coordinates to GL Buffer */

    CSO_setAndComputeNormals    = CSO_setupNormals | CSO_ComputeNormals,   /* compute normals fill them within vertex data and set them up  to GL Buffer */
    CSO_setAndComputeTextures   = CSO_setupTextures | CSO_ComputeTextures, /* compute normals fill them within vertex data and set them up  to GL Buffer */
    CSO__setAndComputeAll       = CSO_setupTextures | CSO_setupNormals | CSO_ComputeTextures | CSO_ComputeNormals

};

/* see CSO flag type */
typedef uint8_t GDLC_type;
enum GLDescCreation:GDLC_type
{
    GLDC_Draw       = 0,
    GLDC_Shape      = 1,    /* CSO_setupShape */
    GLDC_NormVisu   = 2,    /* CSO_setupNormVisu */
    GLDC_User       = 4     /* CSO_setupUser */
};

typedef uint8_t     MatGen_Type;
enum MatGen : MatGen_Type
{
    MAT_Model       =      1,
    MAT_View        =   0x02,
    MAT_Projection  =   0x04,
    MAT_Normal      =   0x08,
    MAT_All         =   MAT_Model | MAT_View | MAT_Projection | MAT_Normal
};

typedef uint32_t SHU_type;
enum ShaderUniform_type:SHU_type {
    SHU_Nothing         =   0,
    SHU_Bool            =   1,
    SHU_Float           =   2,
    SHU_Vec2            =   0x1004,
    SHU_Vec3            =   0x1008,
    SHU_Vec4            =   0x1010,
    SHU_Mat2            =   0x1020,
    SHU_Mat3            =   0x1040,
    SHU_Mat4            =   0x1080,
    SHU_Mat4Transpose   =   0x1100,

    SHU_Model           = 0x021000,
    SHU_View            = 0x041000,
    SHU_Projection      = 0x081000,
    SHU_Normal          = 0x101000,

    SHU_Texture         = 0x3000,
    SHU_Material        = 0x5000,
    SHU_Light           = 0x9000,

    SHU_LineWidth        = 0x1400, /* special case for glLineSize() */

    SHU_IsPointer       = 0x1000
};


/* Text and text box attributes */

typedef uint32_t RBP_type;
enum RBoxPos : RBP_type
{
    RBP_Nothing             = 0,
    /* for horizontal display only */
    RBP_Center              = 0x01, /* centered either horizontally or vertically according diplay mode (vertical or horizontal)*/
    RBP_LeftJust            = 0x04, /* text is horizontally left justified (default) */
    RBP_RightJust           = 0x08, /* text is horizontally right justified */
    /* for vertical text display only */
//    RBP_VertCenter      = 0x02, /* vertically centered */
    RBP_TopJust             = 0x10, /* text is vertically display starting at top of box (default) */
    RBP_BotJust             = 0x20, /* text is vertically displayed to box bottom */

    RBP_LineWrap            = 0x40, /* Text is cut where line/column ends without taking care of words*/
    RBP_WordWrap            = 0x80, /* Text is wrapped by word if it does not fit into box boundary (default)*/
    RBP_TruncChar           = 0x0100, /* Displays a truncate sign at the end of the truncated line */
//    RBP_FitVertical     = 0x80,          /* Text should fit into vertical box boundary  */

    RBP_AdjustFSize         = 0x0200,/* Text should fit as it is adjusting font size if necessary :
                                    This option must be set only if RBP_Wrap is not set
                                    to make text with fit into box maximum width */

    RBP_TextMask           =    0x00FFFF,

/* text box drawing flag */

    RBP_BoxVisible         =    0x010000,
    RBP_BoxShape           =    0x020000,
    RBP_BoxFill            =    0x040000,
    RBP_BoxTexture         =    0x080000,

    RBP_BoxMask            =  0xFFFF0000,

//    RBP_AdjustBestTry   = 0x0200,       /* Text size is being adjusted if it does not fit vertically after being cut (default) */

    RBP_Default         = RBP_LeftJust | RBP_TopJust | RBP_LineWrap | RBP_BoxVisible | RBP_BoxShape,

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


#include <glm/glm.hpp>

inline glm::vec3 vAdd(glm::vec3 pV1,glm::vec3 pV2)
{
    return glm::vec3(pV1.x+pV2.x,pV1.y+pV2.y,pV1.z+pV2.z);
}

inline glm::vec3 vSubst(glm::vec3 pV1,glm::vec3 pV2)
{
    return glm::vec3(pV1.x-pV2.x,pV1.y-pV2.y,pV1.z-pV2.z);
}

inline glm::vec3 vMult(glm::vec3 pV1,glm::vec3 pV2)
{
    return glm::vec3(pV1.x*pV2.x,pV1.y*pV2.y,pV1.z*pV2.z);
}

inline glm::vec3 vDiv(glm::vec3 pV1,glm::vec3 pV2)
{
    return glm::vec3(pV1.x/pV2.x,pV1.y/pV2.y,pV1.z/pV2.z);
}


const char* decodeSHU(const ShaderUniform_type pSHU );
ShaderUniform_type encodeSHU(const char* pSHUCtx);

std::string decodeCSO(const CSO_type pFlag);
CSO_type encodeCSO(const char* pIn);

std::string decodeRBP(const RBP_type pFlag);
RBP_type encodeRBP(const char* pIn);

std::string decodeMatGen(const MatGen_Type pFlag);
MatGen_Type encodeMatGen(const char* pIn);



#endif // ZGLCONSTANTS_H
