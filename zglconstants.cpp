#include <zglconstants.h>
#include <string.h> // for strcasestr


const char*
decodeSHU(const ShaderUniform_type pSHU )
{
    switch (pSHU)
    {
    case SHU_Bool:
        return "SHU_Bool";
    case SHU_Float:
        return "SHU_Float";
    case SHU_Vec2:
        return "SHU_Vec2";
    case SHU_Vec3:
        return "SHU_Vec3";
    case SHU_Vec4:
        return "SHU_Vec4";
    case SHU_Mat2:
        return "SHU_Mat2";
    case SHU_Mat3:
        return "SHU_Mat3";
    case SHU_Mat4:
        return "SHU_Mat4";
    case SHU_Mat4Transpose:
        return "SHU_Mat4Transpose";
    case SHU_Texture:
        return "SHU_Texture";
    case SHU_Light:
        return "SHU_Light";
    case SHU_LineWidth:
        return "SHU_LineWidth";
    case SHU_Material:
        return "SHU_Material";
     default:
        return "SHU_Nothing";

    }//switch
}//decodeSHU


ShaderUniform_type encodeSHU(const char* pSHUCtx)
{
    if (strcasestr(pSHUCtx,"SHU_Bool"))
            return SHU_Bool;
    if (strcasestr(pSHUCtx,"SHU_Float"))
            return SHU_Float;
    if (strcasestr(pSHUCtx,"SHU_Float"))
            return SHU_Vec2;
    if (strcasestr(pSHUCtx,"SHU_Vec2"))
            return SHU_Vec3;
    if (strcasestr(pSHUCtx,"SHU_Vec3"))
            return SHU_Vec4;
    if (strcasestr(pSHUCtx,"SHU_Vec4"))
            return SHU_Mat2;
    if (strcasestr(pSHUCtx,"SHU_Mat2"))
            return SHU_Mat3;
    if (strcasestr(pSHUCtx,"SHU_Mat3"))
            return SHU_Mat4Transpose;
    if (strcasestr(pSHUCtx,"SHU_Mat4Transpose"))
            return SHU_Mat4;
    if (strcasestr(pSHUCtx,"SHU_Mat4"))
            return SHU_Texture;
    if (strcasestr(pSHUCtx,"SHU_Texture"))
            return SHU_Texture;
    if (strcasestr(pSHUCtx,"SHU_Light"))
            return SHU_Light;
    if (strcasestr(pSHUCtx,"SHU_LineWidth"))
            return SHU_LineWidth;
    if (strcasestr(pSHUCtx,"SHU_Material"))
            return SHU_Material;
    /* nothing found corresponding */
    return SHU_Nothing;
}//encodeSHU




std::string decodeMatGen(const MatGen_Type pFlag)
{
    std::string wRet="";

    if (pFlag & MAT_Model)
            wRet += "MAT_Model ";
    if (pFlag & MAT_View)
            {
            if (wRet!="")
                    wRet+="| ";
            wRet += "MAT_View ";
            }
    if (pFlag & MAT_Projection)
            {
            if (wRet!="")
                    wRet+="| ";
            wRet += "MAT_Projection ";
            }
    if (pFlag & MAT_Normal)
            {
            if (wRet!="")
                    wRet+="| ";
            wRet += "MAT_Normal ";
            }
    return wRet;
}


MatGen_Type encodeMatGen(const char* pIn)
{
    MatGen_Type wRet=0;

    if (strcasestr(pIn , "MAT_Model"))
            wRet |= MAT_Model;
    if (strcasestr(pIn , "MAT_View"))
            wRet |= MAT_View;
    if (strcasestr(pIn , "MAT_Projection"))
            wRet |= MAT_Projection;
    if (strcasestr(pIn , "MAT_Normal"))
            wRet |= MAT_Normal;

    return wRet;
}


std::string decodeCSO(const CSO_type pFlag)
{
    std::string wRet="";

    if (pFlag & CSO_setupNormals)
            {
            if (wRet!="")
                    wRet+="| ";
            wRet += "CSO_setupNormals ";
            }
    if (pFlag & CSO_setupTextures)
            {
            if (wRet!="")
                    wRet+="| ";
            wRet += "CSO_setupTextures ";
            }
    if (pFlag & CSO_ComputeNormals)
            {
            if (wRet!="")
                    wRet+="| ";
            wRet += "CSO_ComputeNormals ";
            }
    if (pFlag & CSO_ComputeTextures)
            {
            if (wRet!="")
                    wRet+="| ";
            wRet += "CSO_ComputeTextures ";
            }
    if (pFlag & CSO_setupShape)
            {
            if (wRet!="")
                    wRet+="| ";
            wRet += "CSO_setupShape ";
            }
    if (pFlag & CSO_setupNormVisu)
            {
            if (wRet!="")
                    wRet+="| ";
            wRet += "CSO_setupNormVisu ";
            }
    if (pFlag & CSO_setupUser)
            {
            if (wRet!="")
                    wRet+="| ";
            wRet += "CSO_setupUser ";
            }
    if (pFlag & CSO_DeleteDataAfter)
            {
            if (wRet!="")
                    wRet+="| ";
            wRet += "CSO_DeleteDataAfter ";
            }
    return wRet;
}


CSO_type encodeCSO(const char* pIn)
{
    CSO_type wRet=0;

    if (strcasestr(pIn , "CSO_setupNormals"))
            wRet |= CSO_setupNormals;
    if (strcasestr(pIn , "CSO_setupTextures"))
            wRet |= CSO_setupTextures;
    if (strcasestr(pIn , "CSO_ComputeNormals"))
            wRet |= CSO_ComputeNormals;
    if (strcasestr(pIn , "CSO_ComputeTextures"))
            wRet |= CSO_ComputeTextures;
    if (strcasestr(pIn , "CSO_setupShape"))
            wRet |= CSO_setupShape;
    if (strcasestr(pIn , "CSO_setupNormVisu"))
            wRet |= CSO_setupNormVisu;
    if (strcasestr(pIn , "CSO_setupUser"))
            wRet |= CSO_setupUser;
    if (strcasestr(pIn , "CSO_DeleteDataAfter"))
            wRet |= CSO_DeleteDataAfter;
    return wRet;
}

std::string decodeRBP(const RBP_type pFlag)
{
    std::string wRet="";

/* text positionning flag */

    if (pFlag & RBP_Center)
            {
            if (wRet!="")
                    wRet+="| ";
            wRet += "RBP_Center ";
            }
    if (pFlag & RBP_LeftJust)
            {
            if (wRet!="")
                    wRet+="| ";
            wRet += "RBP_LeftJust ";
            }
    if (pFlag & RBP_RightJust)
            {
            if (wRet!="")
                    wRet+="| ";
            wRet += "RBP_RightJust ";
            }
    if (pFlag & RBP_TopJust)
            {
            if (wRet!="")
                    wRet+="| ";
            wRet += "RBP_TopJust ";
            }
    if (pFlag & RBP_BotJust)
            {
            if (wRet!="")
                    wRet+="| ";
            wRet += "RBP_BotJust ";
            }
    if (pFlag & RBP_LineWrap)
            {
            if (wRet!="")
                    wRet+="| ";
            wRet += "RBP_LineWrap ";
            }
    if (pFlag & RBP_WordWrap)
            {
            if (wRet!="")
                    wRet+="| ";
            wRet += "RBP_WordWrap ";
            }
    if (pFlag & RBP_TruncChar)
            {
            if (wRet!="")
                    wRet+="| ";
            wRet += "RBP_TruncChar ";
            }
    if (pFlag & RBP_AdjustFSize)
            {
            if (wRet!="")
                    wRet+="| ";
            wRet += "RBP_AdjustFSize ";
            }

/* Text box drawing flag */

    if (pFlag & RBP_BoxVisible)
            {
            if (wRet!="")
                    wRet+="| ";
            wRet += "RBP_TruncChar ";
            }
    if (pFlag & RBP_BoxShape)
            {
            if (wRet!="")
                    wRet+="| ";
            wRet += "RBP_BoxShape ";
            }
    if (pFlag & RBP_BoxFill)
            {
            if (wRet!="")
                    wRet+="| ";
            wRet += "RBP_BoxFill ";
            }
    if (pFlag & RBP_BoxTexture)
            {
            if (wRet!="")
                    wRet+="| ";
            wRet += "RBP_BoxTexture ";
            }
    return wRet;
}


RBP_type encodeRBP(const char* pIn)
{
    RBP_type wRet=0;
/* text positionning flag */
    if (strcasestr(pIn , "RBP_Center"))
            wRet |= RBP_Center;
    if (strcasestr(pIn , "RBP_LeftJust"))
            wRet |= RBP_LeftJust;
    if (strcasestr(pIn , "RBP_RightJust"))
            wRet |= RBP_RightJust;
    if (strcasestr(pIn , "RBP_TopJust"))
            wRet |= RBP_TopJust;
    if (strcasestr(pIn , "RBP_BotJust"))
            wRet |= RBP_BotJust;
    if (strcasestr(pIn , "RBP_LineWrap"))
            wRet |= RBP_LineWrap;
    if (strcasestr(pIn , "RBP_WordWrap"))
            wRet |= RBP_WordWrap;
    if (strcasestr(pIn , "RBP_TruncChar"))
            wRet |= RBP_TruncChar;
    if (strcasestr(pIn , "RBP_AdjustFSize"))
            wRet |= RBP_AdjustFSize;

/* text box drawing flag */

    if (strcasestr(pIn , "RBP_BoxVisible"))
            wRet |= RBP_BoxVisible;
    if (strcasestr(pIn , "RBP_BoxShape"))
            wRet |= RBP_BoxShape;
    if (strcasestr(pIn , "RBP_BoxFill"))
            wRet |= RBP_BoxFill;
    if (strcasestr(pIn , "RBP_BoxTexture"))
            wRet |= RBP_BoxTexture;



    return wRet;
}
