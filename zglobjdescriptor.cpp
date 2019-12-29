#include "zglobjdescriptor.h"
#ifdef __USE_GLAD__
#include <glad/glad.h>
#else
#include <GL/glew.h>
#endif

#include <glm/glm.hpp>

#include <ztexture.h>

ZGLObjDescriptor::~ZGLObjDescriptor()
{
    deleteGLContext();

    /* delete vertex and index coordinates and references */

    if (VertexData)
        delete VertexData;
    if (Indexes)
            delete Indexes;
    if (VName)
            delete VName;
    if (VNormalDir)
            delete VNormalDir;
  /*  if (Texture)
            GLResources->deregisterTexture( Texture);
  */
}
 ZGLObjDescriptor& ZGLObjDescriptor::cloneFrom(const ZGLObjDescriptor& pIn)
{
    VBO=pIn.VBO;
    VAO=pIn.VAO;
    NormVBO=pIn.NormVBO;
    TexVBO=pIn.TexVBO;

    EBO=pIn.EBO;

    ComputeNormals=pIn.ComputeNormals;
    ComputeTexCoords=pIn.ComputeTexCoords;
    ComputeNormVisu=pIn.ComputeNormVisu;
    KeepVertices=pIn.KeepVertices;
    LineSize=pIn.LineSize;


    if (VertexData)
            delete VertexData;
    VertexData=nullptr;
    if (pIn.VertexData)
        {
        VertexData = new zbs::ZArray<ZVertice>;
        for (long wi=0;wi<pIn.VertexData->count();wi++)
            {
                VertexData->push(pIn.VertexData->Tab[wi]);
            }
        }

    if (Indexes)
            delete Indexes;
    Indexes=nullptr;
    if (pIn.Indexes)
        {
        Indexes = new zbs::ZArray<unsigned int>;
        for (long wi=0;wi<pIn.Indexes->count();wi++)
            {
                Indexes->push(pIn.Indexes->Tab[wi]);
            }
        }
    if (VNormalDir)
        {
        delete VNormalDir;
        VNormalDir=nullptr;
        if (pIn.VNormalDir)
            VNormalDir = new zbs::ZArray<NormalDirection>(*pIn.VNormalDir);
        }
    if (VName)
        {
        delete VName;
        VName=nullptr;
        if (pIn.VName)
            VName = new zbs::ZArray<const char*>(*pIn.VName);
        }
    return *this;
}


int ZGLObjDescriptor::setupVertex(zbs::ZArray<ZVertice>* pVertices, zbs::ZArray<unsigned int>* pIndexes)
 {
    if (!pVertices)
        return -1;
     if (VertexData)
             delete VertexData;
     VertexData = new zbs::ZArray<ZVertice>;
     for (long wi=0;wi<pVertices->count();wi++)
     {
         VertexData->push(pVertices->Tab[wi]);
     }
     if (!pIndexes)
             return 0;
     if (Indexes)
             delete Indexes;
     Indexes = new zbs::ZArray<unsigned int>;
     for (long wi=0;wi<pIndexes->count();wi++)
     {
         Indexes->push(pIndexes->Tab[wi]);
     }
     return 0;
 }
int ZGLObjDescriptor::setupVec3(zbs::ZArray<glm::vec3>* pVertices, zbs::ZArray<unsigned int>* pIndexes)
 {
    if (!pVertices)
        return -1;
     if (VertexData)
             delete VertexData;
     VertexData = new zbs::ZArray<ZVertice>;
     for (long wi=0;wi<pVertices->count();wi++)
        {
        VertexData->push(ZVertice(pVertices->Tab[wi]));
        }
     if (!pIndexes)
             return 0;
     if (Indexes)
             delete Indexes;
     Indexes = new zbs::ZArray<unsigned int>;
     for (long wi=0;wi<pIndexes->count();wi++)
     {
         Indexes->push(pIndexes->Tab[wi]);
     }
     return 0;
 }
#ifdef __COMMENT__
void ZGLObjDescriptor::setTexture(ZTexture* pTexture)
{
    if (pTexture != nullptr)
        {
            Texture=pTexture->share();
            if (Texture->isValid()) /* if valid texture */
                    setUseTexture(true); /* use it */
                else
                    setUseTexture(false);
            return;
        }

    setUseTexture(false);
    return;
}

ZTexture*
ZGLObjDescriptor::loadTexture(const char* pTextureName, const char* pIntlName, GLenum pTextureEngine)
{
//    Shader->use();
    Texture =GLResources->getTextureByRank( GLResources->loadTexture(pTextureName,pIntlName,pTextureEngine));
    if (Texture!=nullptr)
            {
            setUseTexture(true);
            return Texture;
            }
    setUseTexture(false);
    return nullptr;
}

int ZGLObjDescriptor::setTextureByName(const char* pIntlName)
{
    setUseTexture(false);
    if (Texture!=nullptr)
            GLResources->deregisterTexture(Texture);
    Texture=GLResources->getTextureByName(pIntlName);

    if (Texture==nullptr)
                    return -1;
    setUseTexture(true);
    return 0;
}


int ZGLObjDescriptor::setTextureByRank(const long pIdx)
{
    setUseTexture(false);
    if (Texture!=nullptr)
            GLResources->deregisterTexture(Texture);
    Texture=GLResources->getTextureByRank(pIdx);
    if (Texture == nullptr)
            return -1;
    setUseTexture(true);
    return 0;
}
#endif
