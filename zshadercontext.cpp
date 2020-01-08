#include <zshadercontext.h>

#include <zshader.h>
#include <ztexture.h>
#include <zmaterial.h>
#include <zglconstants.h>

/*-----------ShaderUniform (element of ZShaderContext)--------------*/

void ShaderUniform::addTexture(ZTexture* pValue)
    {
    Type = SHU_Texture;
    Name="Texture_specific";
    Value.Texture=pValue;
    }
void ShaderUniform::addLight(const ZLight* pValue)
    {
    Type = SHU_Light;
    Name="Light_specific";
    Value.Light=pValue;
    }
void ShaderUniform::addMaterial(const ZMaterial* pValue)
    {
    Type = SHU_Material;
    Name="Material_specific";
    Value.Material=pValue;
    }
void ShaderUniform::addBool(const char*pName, bool pValue)
{
    Type = SHU_Bool;
    Name=pName;
    Value.Bool=pValue;
}
void ShaderUniform::addFloat(const char*pName, float pValue)
{
    Type = SHU_Float;
    Name=pName;
    Value.Float=pValue;
}

void ShaderUniform::addVec2(const char*pName, const glm::vec2* pValue)
{
    Type = SHU_Vec2;
    Name=pName;
    Value.Vec2=pValue;
}
void ShaderUniform::addVec3(const char*pName, const glm::vec3* pValue)
{
    Type = SHU_Vec3;
    Name=pName;
    Value.Vec3=pValue;
}
void ShaderUniform::addVec4(const char*pName, const glm::vec4* pValue)
{
    Type = SHU_Vec4;
    Name=pName;
    Value.Vec4=pValue;
}
void ShaderUniform::addMat2(const char*pName, glm::mat2* pValue)
{
    Type = SHU_Mat2;
    Name=pName;
    Value.Mat2=pValue;
}
void ShaderUniform::addMat3(const char*pName, glm::mat3* pValue)
{
    Type = SHU_Mat3;
    Name=pName;
    Value.Mat3=pValue;
}
void ShaderUniform::addMat4(const char*pName, glm::mat4* pValue)
{
    Type = SHU_Mat4;
    Name=pName;
    Value.Mat4=pValue;
}
void ShaderUniform::addMat4Transpose(const char*pName, glm::mat4* pValue)
{
    Type = SHU_Mat4Transpose;
    Name=pName;
    Value.Mat4=pValue;
}

void ShaderUniform::addLineWidth(float pLineSize)
{
    Type=SHU_LineWidth;
    Name="glLineSize";
    Value.Float=pLineSize;
}


void ShaderUniform::add(ShaderUniform_type pType, const char*pName, void* pValue)
{
    Type=pType;
    Name=pName;
    Value.Void=pValue;
}

/*---------------ZShaderContext--------------------------------------*/

ZShaderContext::~ZShaderContext()
{
    if (Texture)
        delete Texture;
    if (Shader)
        delete Shader;
}

ZShaderContext& ZShaderContext::_cloneFrom(ZShaderContext&pIn)
{
    _Base::reset();
    for(long wi=0;wi<pIn.count();wi++)
            push(pIn[wi]);

    if (Shader)
            {
            delete Shader;
            Shader=nullptr;
            }
    if (pIn.Shader)
            Shader=pIn.Shader->sharePtr();
    if (Texture)
            delete Texture;
    Texture = nullptr;
    if (pIn.Texture)
            Texture=pIn.Texture->sharePtr();

//    DrawFigure = pIn.DrawFigure;
    return *this;
}

ShaderUniform* ZShaderContext::getUniformByName(const char*pName)
{
    for (long wi=0;wi<count();wi++)
            if (strcmp(Tab[wi].Name,pName)==0)
                        return &Tab[wi];
    return nullptr;
}
ShaderUniform* ZShaderContext::getUniformByNameCase(const char*pName)
{
    for (long wi=0;wi<count();wi++)
            if (strcasecmp(Tab[wi].Name,pName)==0)
                        return &Tab[wi];
    return nullptr;
}
/*int ZShaderContext::setShader(ZShader* pShader)
{
    if (Shader!=nullptr)
            return -1;
    if (!pShader)
            return -1;
    Shader=pShader->share();
    return 0;
}*/
int ZShaderContext::setShader(ZShader* pShader)
{
    if (!pShader)
                    return -1;
    if (Shader)
            delete Shader;
    Shader=pShader->sharePtr();
    return 0;
}

void ZShaderContext::setTexture(ZTexture* pValue)
    {
    if (Texture)
            GLResources->deregisterTexture(Texture);
    Texture = pValue->sharePtr();
    _addTexture(Texture);
    }
int ZShaderContext::_addTexture(ZTexture* pValue)
{
    ShaderUniform wSU;
    wSU.addTexture(pValue);
    return push(wSU);
}
int ZShaderContext::setTextureByName(const char* pName)
    {
    if (Texture)
            GLResources->deregisterTexture(Texture);
    Texture = GLResources->getTextureByName(pName);
    if (Texture==nullptr)
            return -1;

    addBool(__SHD_USE_TEXTURE_UN__,true);
    return _addTexture(Texture);
    }
int ZShaderContext::setTextureByRank(const long pRank)
    {
    if (Texture)
            GLResources->deregisterTexture(Texture);
    Texture = GLResources->getTextureByRank(pRank);
    if (Texture==nullptr)
            return -1;
    return _addTexture(Texture);
    }

ZTexture* ZShaderContext::getTexture()
{
    if(Texture)
        return Texture->sharePtr();
    return nullptr;
}

bool    ZShaderContext::hasTexture() {return Texture!=nullptr;}

int ZShaderContext::bindTexture()
{
    if (!Texture)
            return -1;
    Texture->bind();
    return 0;
}
void ZShaderContext::releaseTexture()
{
    if (!Texture)
            return ;
    Texture->unbind();
    return ;
}
int ZShaderContext::bindShader()
{
    if (!Shader)
            return -1;
    Shader->use();
    return 0;
}
void ZShaderContext::releaseShader()
{
    if (!Shader)
            return ;
    Shader->release();
    return ;
}
/*
int ZShaderContext::setupShader(ZShader* pShader,zbs::ZArray<ShaderUniform>* pParams)
{
    pShader->use();
    for (long wi=0;wi<pParams->count();wi++)
            setShader(pShader,pParams->Tab[wi]);
    return 0;
}
*/
const char* decodeShdCtx(const int pCtx)
{
    switch (pCtx)
    {
    case Draw:
        return "Draw";
    case Shape:
        return "Shape";
    case NormVisu:
        return "NormVisu";
    default:
        return "UserDefined";
    }
}
int encodeShdCtx(const char*pIn)
{
    int wRet=0;

    if (strcasestr(pIn , "Draw"))
            return Draw;
    if (strcasestr(pIn , "Shape"))
            return Shape;
    if (strcasestr(pIn , "NormVisu"))
            return NormVisu;
    if (strcasestr(pIn , "UserDefined"))
            return UserDefined;
}


int ZShaderContext::applyRules()
{
//    fprintf(stdout,"ZShaderContext::setupShader-I-SETSHD For Shader context setting up shader <%s> uniform values.\n",
//            Shader->getName());

    Shader->use();
    for (long wi=0;wi<count();wi++)
                    _applyShader(Tab[wi]);
//        {
//            if (Tab[wi].Type==SHU_LineWidth)
//                {
//                LineWidth_IsSet=true;
//                 glGetFloatv(GL_LINE_WIDTH,&LineWidth_restore); /* get current line width */
//                glLineWidth(Tab[wi].Value.Float);
//                continue;
//                }
//        _applyShader(Tab[wi]);
//        }
    return 0;
//    return _setupShader(Shader,(zbs::ZArray<ShaderUniform>*)this);
}

void ZShaderContext::postProcess()
{
    if (LineWidth_IsSet)
    {
        LineWidth_IsSet=false;
        glLineWidth(LineWidth_restore);/* restore line width */
    }
    releaseTexture();
    releaseShader();
}

/* shader must be current (ZShader::use() ) */
inline int
ZShaderContext::_applyShader(ShaderUniform pSHU )
{
    switch (pSHU.Type)
    {
    case SHU_Bool:
        return Shader->setBool(pSHU.Name,pSHU.Value.Bool);
    case SHU_Float:
        return Shader->setFloat(pSHU.Name,pSHU.Value.Float);
    case SHU_Vec2:
        return Shader->setVec2(pSHU.Name,*pSHU.Value.Vec2);
    case SHU_Vec3:
        return Shader->setVec3(pSHU.Name,*pSHU.Value.Vec3);
    case SHU_Vec4:
        return Shader->setVec4(pSHU.Name,*pSHU.Value.Vec4);
    case SHU_Mat2:
        return Shader->setMat2(pSHU.Name,*pSHU.Value.Mat2);
    case SHU_Mat3:
        return Shader->setMat3(pSHU.Name,*pSHU.Value.Mat3);
    case SHU_Mat4:
        return Shader->setMat4(pSHU.Name,*pSHU.Value.Mat4);
    case SHU_Mat4Transpose:
        return Shader->setMat4Transpose(pSHU.Name,*pSHU.Value.Mat4);
    case SHU_Texture:
        pSHU.Value.Texture->bind();
        return Shader->setupTexSampler(pSHU.Value.Texture);
    case SHU_Material:
        return Shader->setupMaterial(pSHU.Value.Material);
    case SHU_LineWidth:
        this->LineWidth_IsSet=true;
        glGetFloatv(GL_LINE_WIDTH,&LineWidth_restore); /* get current line width */
        glLineWidth(pSHU.Value.Float);
        return 0;
    case SHU_Light:
        return Shader->setupLight(pSHU.Value.Light);
    default:
        fprintf (stderr,"setShader-E-SHUUNKN SHU type id <%d> unkown \n",(int)pSHU.Type);
        return -1;
    }
}//_applyShader

GLuint ZShaderContext::getPositionAttribute()
{
    int wPosition= Shader->getNamedAttributeLocation(__SHD_POSITION_ATTR__,false);
    if (wPosition<0)
        wPosition= cst_defaultPositionLocation;

    return wPosition;
}
GLuint ZShaderContext::getNormalAttribute()
{
    int wPosition= Shader->getNamedAttributeLocation(__SHD_NORMAL_ATTR__,false);
    if (wPosition<0)
        wPosition= cst_defaultPositionLocation;

    return wPosition;
}
GLuint ZShaderContext::getTexCoordsAttribute()
{
    int wPosition= Shader->getNamedAttributeLocation(__SHD_TEXCOORDS_ATTR__,false);
    if (wPosition<0)
        wPosition= cst_defaultPositionLocation;

    return wPosition;
}






char wSHO[300];
char*
displayShaderRule(const ShaderUniform pSHU )
{
    memset(wSHO,0,sizeof(wSHO));
    switch (pSHU.Type)
    {
    case SHU_Bool:
        sprintf(wSHO,"%15s <%20s> <%s>",
                "SHU_Bool",
                pSHU.Name,
                pSHU.Value.Bool?"true":"false");

        return wSHO;
    case SHU_Float:
        sprintf(wSHO,"%15s  <%20s> <%f>",
                "SHU_Float",
                pSHU.Name,
                pSHU.Value.Float);
        return wSHO;
    case SHU_Vec2:
        sprintf(wSHO,"%15s  <%20s> <x:%f , y:%f>",
                "SHU_Vec2",
                pSHU.Name,
                pSHU.Value.Vec2->x,pSHU.Value.Vec2->y);
        return wSHO;
    case SHU_Vec3:
        sprintf(wSHO,"%15s  <%20s> <x:%f , y:%f , z:%f>",
                "SHU_Vec3",
                pSHU.Name,
                pSHU.Value.Vec3->x,pSHU.Value.Vec3->y,pSHU.Value.Vec3->z);
        return wSHO;
    case SHU_Vec4:
        sprintf(wSHO,"%15s  <%20s> <x:%f , y:%f , z:%f , w:%f>",
                "SHU_Vec4",
                pSHU.Name,
                pSHU.Value.Vec4->x,pSHU.Value.Vec4->y,pSHU.Value.Vec4->z,pSHU.Value.Vec4->w);
        return wSHO;
    case SHU_Mat2:
        sprintf(wSHO,"%15s <%20s> <%s>",
                "SHU_Mat2",
                pSHU.Name,
                "--glm::mat2--");
        return wSHO;
    case SHU_Mat3:
        sprintf(wSHO,"%15s  <%20s> <%s>",
                "SHU_Mat3",
                pSHU.Name,
                "--glm::mat3--");
        return wSHO;
    case SHU_Mat4:
        sprintf(wSHO,"%15s  <%20s> <%s>",
                "SHU_Mat4",
                pSHU.Name,
                "--glm::mat4--");
        return wSHO;
    case SHU_Mat4Transpose:
        sprintf(wSHO,"%15s  <%20s> <%s>",
                "SHU_Mat4Transp.",
                pSHU.Name,
                "--glm::mat4--");
        return wSHO;
    case SHU_Texture:
        sprintf(wSHO,"%15s  <%20s> <%s> GLid <%d> GL Engine <%d>",
                "SHU_Texture",
                pSHU.Name,
                pSHU.Value.Texture->getName(),
                pSHU.Value.Texture->getGLId(),
                pSHU.Value.Texture->getTextureEngineNumber());
        return wSHO;

    case SHU_Light:
        sprintf(wSHO,"%15s  <%20s> %s",
                "SHU_Light",
                pSHU.Name,
                pSHU.Value.Light->display());
        return wSHO;
    case SHU_LineWidth:
        sprintf(wSHO,"%15s  <%20s> <%f>        NB: followed by postprocess rule",
                "SHU_LineWidth",
                pSHU.Name,
                pSHU.Value.Float);
        return wSHO;
    case SHU_Material:
        sprintf(wSHO,"%15s  <%20s> %s",
                "SHU_Material",
                pSHU.Name,
                pSHU.Value.Material->display()
                );
        return wSHO;
     default:
        sprintf(wSHO,"<--Unknown Shader rule code-->");
        return wSHO;
    }
}//displayShaderRule

void ZShaderContext::display(FILE*pOutput)
{
    fprintf (pOutput,
             "Shader Context for shader <%s> id <%d>-- Number of rules <%ld>---\n",
             Shader->getName(),
             Shader->getShaderGLId(),
             count());
    for (long wi=0;wi < count();wi++)
        {
        fprintf (pOutput,
                 "%3ld> %s\n",
                 wi,
                 displayShaderRule(Tab[wi]));
        }
    fprintf (pOutput,
             "----------------------------------------------------------------------\n");
    return;
} // print







char*
exportXMLSHURule(const ShaderUniform pSHU )
{
    memset(wSHO,0,sizeof(wSHO));

    switch (pSHU.Type)
    {
    case SHU_Bool:
        sprintf(wSHO,
                "<shutype>%s</shutype>\n"
                "<shuname>%s</shuname>\n"
                "<shuvalue>%s</shuvalue>\n",
                decodeSHU(pSHU.Type),
                pSHU.Name,
                pSHU.Value.Bool?"true":"false");

        return wSHO;
    case SHU_Float:
        sprintf(wSHO,"%15s  <%20s> <%f>",
                "SHU_Float",
                pSHU.Name,
                pSHU.Value.Float);
        return wSHO;
    case SHU_Vec2:
        sprintf(wSHO,"%15s  <%20s> <x:%f , y:%f>",
                "SHU_Vec2",
                pSHU.Name,
                pSHU.Value.Vec2->x,pSHU.Value.Vec2->y);
        return wSHO;
    case SHU_Vec3:
        sprintf(wSHO,"%15s  <%20s> <x:%f , y:%f , z:%f>",
                "SHU_Vec3",
                pSHU.Name,
                pSHU.Value.Vec3->x,pSHU.Value.Vec3->y,pSHU.Value.Vec3->z);
        return wSHO;
    case SHU_Vec4:
        sprintf(wSHO,"%15s  <%20s> <x:%f , y:%f , z:%f , w:%f>",
                "SHU_Vec4",
                pSHU.Name,
                pSHU.Value.Vec4->x,pSHU.Value.Vec4->y,pSHU.Value.Vec4->z,pSHU.Value.Vec4->w);
        return wSHO;
    case SHU_Mat2:
        sprintf(wSHO,"%15s <%20s> <%s>",
                "SHU_Mat2",
                pSHU.Name,
                "--glm::mat2--");
        return wSHO;
    case SHU_Mat3:
        sprintf(wSHO,"%15s  <%20s> <%s>",
                "SHU_Mat3",
                pSHU.Name,
                "--glm::mat3--");
        return wSHO;
    case SHU_Mat4:
        sprintf(wSHO,"%15s  <%20s> <%s>",
                "SHU_Mat4",
                pSHU.Name,
                "--glm::mat4--");
        return wSHO;
    case SHU_Mat4Transpose:
        sprintf(wSHO,"%15s  <%20s> <%s>",
                "SHU_Mat4Transp.",
                pSHU.Name,
                "--glm::mat4--");
        return wSHO;
    case SHU_Texture:
        sprintf(wSHO,"%15s  <%20s> <%s> GLid <%d> GL Engine <%d>",
                "SHU_Texture",
                pSHU.Name,
                pSHU.Value.Texture->getName(),
                pSHU.Value.Texture->getGLId(),
                pSHU.Value.Texture->getTextureEngineNumber());
        return wSHO;

    case SHU_Light:
        sprintf(wSHO,"%15s  <%20s> %s",
                "SHU_Light",
                pSHU.Name,
                pSHU.Value.Light->display());
        return wSHO;
    case SHU_LineWidth:
        sprintf(wSHO,"%15s  <%20s> <%f>        NB: followed by postprocess rule",
                "SHU_LineWidth",
                pSHU.Name,
                pSHU.Value.Float);
        return wSHO;
    case SHU_Material:
        sprintf(wSHO,"%15s  <%20s> %s",
                "SHU_Material",
                pSHU.Name,
                pSHU.Value.Material->display()
                );
        return wSHO;
     default:
        sprintf(wSHO,"<--Unknown Shader rule code-->");
        return wSHO;
    }
}//displayShaderRule


std::string
ZShaderContext::exportXML()
{
    std::string wXml="          <shadercontext>\n";

    wXml += "           <shadername>\n";
    wXml += Shader->getName();
    wXml += "</shadername>";

    wXml += "           <rules>\n";
    for (long wi=0;wi<count();wi++)
        {
        wXml += exportXMLSHURule(Tab[wi]);
        }

    wXml += "           </rules>\n";
    wXml += "       </ShaderContext>\n";
    return wXml;
}







