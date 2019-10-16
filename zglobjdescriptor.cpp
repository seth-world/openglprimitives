#include "zglobjdescriptor.h"

GLuint ZGLObjDescriptor::getPositionAttribute(ZShader* pShader)
{
    int wPosition=(GLuint)glGetAttribLocation(pShader->getShaderId(),__POSITION_ATR_NAME__);
    if (wPosition<0)
        PositionAttribArray= cst_defaultPositionLocation;
    else
        PositionAttribArray=(GLuint)wPosition;
    return PositionAttribArray;
}
GLuint ZGLObjDescriptor::getNormalAttribute(ZShader* pShader)
{
    int wNormal=(GLuint)glGetAttribLocation(pShader->getShaderId(),__NORMAL_ATR_NAME__);
    if (wNormal<0)
        NormalAttribArray= cst_defaultNormalLocation;
    else
        NormalAttribArray=(GLuint)wNormal;
    return NormalAttribArray;
}
GLuint ZGLObjDescriptor::getTexCoordsAttribute(ZShader* pShader)
{
    int wTextCoords=(GLuint)glGetAttribLocation(pShader->getShaderId(),__TEXCOORDS_ATR_NAME__);
    if (wTextCoords<0)
        TexCoordsAttribArray= cst_defaultTexCoordsLocation;
    else
        TexCoordsAttribArray=(GLuint)wTextCoords;
    return TexCoordsAttribArray;
}
