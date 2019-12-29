#include "zmetaobject.h"

ZMetaObject::ZMetaObject(const char*pName)
{
    Name=pName;
    MatCtx=new ZMatCtx;
    for (long wi=0;wi<MaxShaderContext;wi++)    /* Nb: MetaObject has no ZGLDescriptor object because no proper vertex array*/
            {
            ShaderContext[wi]=nullptr;
            DrawFigure[wi]=0;
            }
}

ZMetaObject:: ~ZMetaObject()
{
    for (int wi=0;wi<MaxShaderContext;wi++)
            if (ShaderContext[wi])
                    delete ShaderContext[wi];

    /* meta object has no GLDescriptor because no vertexes */

    if (MatCtx)
            delete MatCtx;
    while (count())
        delete popR();
}

void ZMetaObject::_cloneFrom(ZMetaObject& pIn)
{
    Name=pIn.Name;


    for (int wi=0;wi<MaxShaderContext;wi++)
    {
        DrawFigure[wi]=pIn.DrawFigure[wi];
        if (ShaderContext[wi])
                delete ShaderContext[wi];
        ShaderContext[wi]=nullptr;
        if (pIn.ShaderContext[wi])
                ShaderContext[wi]=new ZShaderContext(*pIn.ShaderContext[wi]);
    }


/* matrices */
    if (MatCtx)
            delete MatCtx;
    MatCtx=new ZMatCtx(*pIn.MatCtx);

}

ZObject* ZMetaObject::search(const char* pName)
{
    for (long wi=0;wi<count();wi++)
            if (strcmp(pName,Tab[wi]->Name)==0)
                    return Tab[wi];
    return nullptr;
}

ZObject* ZMetaObject::searchCase(const char* pName)
{
    for (long wi=0;wi<count();wi++)
            if (strcasecmp(pName,Tab[wi]->Name)==0)
                    return Tab[wi];
    return nullptr;
}

int ZMetaObject::createShaderContextByName(const DrawContext_type pCtx, const char* pShaderName)
{
    if (ShaderContext[pCtx])
            delete ShaderContext[pCtx];
    ZShader* wSh=GLResources->getShaderByNamePtr(pShaderName);
    if (!wSh)
            return -1;

    ShaderContext[pCtx]= new ZShaderContext(wSh);
    return 0;
}
int ZMetaObject::createShaderContextByNameCase(const DrawContext_type pCtx,const char* pShaderName)
{
    if (ShaderContext[pCtx])
            delete ShaderContext[pCtx];
    ZShader* wSh=GLResources->getShaderByNameCasePtr(pShaderName);
    if (!wSh)
            return -1;

    ShaderContext[pCtx]= new ZShaderContext(wSh);
    return 0;
}
int ZMetaObject::createShaderContextByRank(const DrawContext_type pCtx,const long pShaderIdx)
{
    if (ShaderContext[pCtx])
            delete ShaderContext[pCtx];
    ZShader* wSh=GLResources->getShaderByRankPtr(pShaderIdx);
    if (!wSh)
            return -1;

    ShaderContext[pCtx]= new ZShaderContext(wSh);
    return 0;
}
void ZMetaObject::createMatrices(uint8_t pFlag)
{
    if (!MatCtx)
        MatCtx=new ZMatCtx;
    MatCtx->createMatrices(pFlag);
//    MatricesSetUp=true;
}//createAllMatrices

void ZMetaObject::createAllMatrices()
{
    if (!MatCtx)
        MatCtx=new ZMatCtx;
    MatCtx->createMatrices(MAT_All);
//    MatricesSetUp=true;
}//createAllMatrices

void ZMetaObject::createModel ()
{
    if (!MatCtx)
            MatCtx=new ZMatCtx;
    MatCtx->createModel();
}
void ZMetaObject::createView ()
{
    if (!MatCtx)
            MatCtx=new ZMatCtx;
    MatCtx->createView();
}
void ZMetaObject::createProjection ()
{
    if (!MatCtx)
            MatCtx=new ZMatCtx;
    MatCtx->createProjection();
}
void ZMetaObject::createNormal ()
{
    if (!MatCtx)
            MatCtx=new ZMatCtx;
    MatCtx->createNormal();
}


/*------------Component searches--------------__*/
ZObject* ZMetaObject::getObjectByName(const char* pName)
{

    for (long wi=0;wi<count();wi++)
        if (strcmp(Tab[wi]->Name,pName)==0)
                return Tab[wi];
    return nullptr;
}

ZObject* ZMetaObject::getObjectByNameCase(const char* pName)
{

    for (long wi=0;wi<count();wi++)
        if (strcasecmp(Tab[wi]->Name,pName)==0)
                return Tab[wi];
    return nullptr;
}

long    ZMetaObject::getObjectRank(const char* pName)
{

    for (long wi=0;wi<count();wi++)
        if (strcmp(Tab[wi]->Name,pName)==0)
                return wi;
    return -1;
}
long    ZMetaObject::getObjectRankCase(const char* pName)
{

    for (long wi=0;wi<count();wi++)
        if (strcasecmp(Tab[wi]->Name,pName)==0)
                return wi;
    return -1;
}

ZObject* ZMetaObject::getObjectByRank(const long pRank)
{
    if (!exists(pRank))
            return nullptr;
    return Tab[pRank];
}



void ZMetaObject::setupGLByContext(const DrawContext_type pCtx,uint16_t pAction)
{
    if (ShaderContext[pCtx])
            ShaderContext[pCtx]->bindShader();
    for (long wi=0;wi < count();wi++)
        {
        if (ShaderContext[pCtx])
                ShaderContext[pCtx]->bindShader();
            Tab[wi]->setupGL(pCtx,pAction);
        }
}//setupGLByContext



/*-----------Draw actions--------------------*/

void ZMetaObject::_preprocessGLMatrices(ZShader* wShader)
{

    if (!MatCtx)        /* No Matrix context : return without doing anything */
            return;
    MatCtx->_preprocessGL(wShader);

}//_preprocessGLDraw


void ZMetaObject::drawGLByContext(const DrawContext_type pCtx)
{
    if (ShaderContext[pCtx]!=nullptr)
        {
        ShaderContext[pCtx]->applyRules();
        _preprocessGLMatrices(ShaderContext[pCtx]->Shader);
        }
/* render all components in the order of storage */
   for(long wi=0;wi<count();wi++)
        {
        if (ShaderContext[pCtx]!=nullptr)
            ShaderContext[pCtx]->applyRules();
        Tab[wi]->drawGLByContext(pCtx);
        }
}

glm::vec3 ZMetaObject::getPosition()
{
    if (!MatCtx)
            return glm::vec3(0.0f);
    return MatCtx->getPosition();
}

void ZMetaObject::setPosition(Vertice_type pPosition)
{
    if (!MatCtx)
        MatCtx=new ZMatCtx;
    MatCtx->setPosition(pPosition);
}

void ZMetaObject::setRotate(float pAngle,glm::vec3 pAxis)
{
    if (!MatCtx)
        MatCtx=new ZMatCtx;
    MatCtx->setRotate(pAngle,pAxis);
}
void ZMetaObject::setRotateDeg(float pAngle,glm::vec3 pAxis)
{
    if (!MatCtx)
        MatCtx=new ZMatCtx;
    MatCtx->setRotate(glm::radians(pAngle),pAxis);
}

