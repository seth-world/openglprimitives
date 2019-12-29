#ifndef ZMETAOBJECT_H
#define ZMETAOBJECT_H

#ifdef __USE_GLAD__
#include <glad/glad.h>
#else
#include <GL/glew.h>
#endif

#define __USE_ZARRAY_COPY_CONTRUCTOR__
#include <zglconstants.h>
#include <ztoolset/zarray.h>
#include <zobject.h>

/**
 * @brief The ZMetaObject class   holds the shaders and the matrices for all dependent component ZObjects
 */

class ZMetaObject : public zbs::ZArray<ZObject*>
{
private:
    void _cloneFrom(ZMetaObject& pIn);

public:
    ZMetaObject(const char*pName);

    ZMetaObject(ZMetaObject& pIn):zbs::ZArray<ZObject*> (pIn)  {_cloneFrom(pIn);}
    ZMetaObject(ZMetaObject&& pIn):zbs::ZArray<ZObject*> (pIn)  {_cloneFrom(pIn);}

    ~ZMetaObject();

    ZMetaObject& operator=(ZMetaObject&& pIn) {_cloneFrom(pIn); return *this;}
    ZMetaObject& operator=(ZMetaObject& pIn) {_cloneFrom(pIn); return *this;}


    ZObject *search(const char* pName);
    ZObject *searchCase(const char* pName);

    int createNormVisuContextByName(const char* pShaderName)
        {
        int wRet,wRetRet=0;
        for (long wi=0;wi < count();wi++)
            {
                wRet=Tab[wi]->createNormVisuContextByName(pShaderName);
                if (wRet!=0)
                        wRetRet=wRet;
            }
            return wRetRet;
        }
    int createNormVisuContextByRank(const long pShaderRank)
        {
        int wRet,wRetRet=0;
        for (long wi=0;wi < count();wi++)
            {
                wRet=Tab[wi]->createNormVisuContextByRank(pShaderRank);
                if (wRet!=0)
                        wRetRet=wRet;
            }
            return wRetRet;
        }

    void add(ZObject* pObject)
    {
 //       pObject->setAllMatrices( Model->share(),View->share(),Projection->share(),Normal->share());
        push(pObject);
        pObject->setMetaObject(this);
    }

    ZMetaObject& operator << (ZObject* pIn) {add(pIn); return *this;}

    /* Retreives a component by its name */
    ZObject* getObjectByName(const char* pName);
    /* Retreives a component by its name case regarless */
    ZObject* getObjectByNameCase(const char* pName);

    long    getObjectRank(const char* pName);
    long    getObjectRankCase(const char* pName);

    ZObject* getObjectByRank(const long pRank);

    ZObject* operator[] (const long pIdx) {return getObjectByRank(pIdx);}
    ZObject* operator[] (const char*pName) {return getObjectByName(pName);}



    /* Meta object has no proper ZGLDescriptor: any value depending on ZGLDescriptor is assigned to first object of meta object list */
/*    int setTextureByName(const char* pIntlName) {return Tab[0]->setTextureByName(pIntlName);}
    int setTextureByRank(const long pIdx) {return Tab[0]->setTextureByRank(pIdx);}
*/

    void setDrawFigure(GLenum pMode,const int pCtx){DrawFigure[pCtx]=pMode;}

    void setDrawFigureAllObjects(GLenum pMode,const int pCtx)
    {
        for (long wi=0;wi < count();wi++)
            Tab[wi]->DrawFigure[pCtx]=pMode;
    }

/*    void setAction(const DrawContext_type pCtx,uint16_t pAction )
        {
        for (long wi=0;wi<count();wi++)
                Tab[wi]->setAction(pCtx,pAction);
        }
*/
    int setGlobalShaderByName(const int pCtx,const char* pShader)
         {
            ZShader* wSh=GLResources->getShaderByNamePtr(pShader);
            if (!wSh)
                    return -1;

                if (ShaderContext[pCtx])
                    ShaderContext[pCtx]->setShader(wSh);
                else
                    return -1;
         }
    int setGlobalShaderByRank(const int pCtx,const long pShIdx)
         {
            ZShader* wSh=GLResources->getShaderByRankPtr(pShIdx);
            if (!wSh)
                    return -1;

                if (ShaderContext[pCtx])
                    ShaderContext[pCtx]->setShader(wSh);
                else
                    return -1;
         }

    ZTexture* getTexture(const int pCtx)
    {
        if (!ShaderContext[pCtx])
                return nullptr;
        return ShaderContext[pCtx]->getTexture();
    } /* share already set texture to other objects */


    void addMaterialAll(const int pCtx,const ZMaterial* pMaterial)
    {
        for (long wi=0;wi<count();wi++)
            if (Tab[wi]->ShaderContext[pCtx])
                Tab[wi]->ShaderContext[pCtx]->addMaterial(pMaterial);
    }

/*    void setComputeNormals(const DrawContext_type pCtx,bool pOnOff)
    {
        for (long wi=0;wi < count();wi++)
        {
            Tab[wi]->setComputeNormals(pCtx,pOnOff);
        }

    }
    void setComputeTexCoords(const DrawContext_type pCtx,bool pOnOff)
    {
        for (long wi=0;wi < count();wi++)
        {
            Tab[wi]->setComputeTexCoords(pCtx,pOnOff);
        }
    }*/

/* shaders */
    int setShaderByRank(const DrawContext_type pCtx,const long pShader )
         {
            if (!ShaderContext[pCtx])
                    return -1;

            ZShader* wS=GLResources->getShaderByRankPtr(pShader);
            if (!wS)
                    return -1;
            ShaderContext[pCtx]->setShader(wS);
            return 0;
         }
    int setShaderByName(const DrawContext_type pCtx,const char* pShader )
        {
       if (!ShaderContext[pCtx])
               return -1;
       ZShader* wS=GLResources->getShaderByNamePtr(pShader);
       if (!wS)
               return -1;
       ShaderContext[pCtx]->setShader(wS);
        }


/* ----Prepare and render meta object ------_*/

    /* DrawFigure  (GLenum equal to GL_TRIANGLES or GL_LINES ...) must be set Object per Object */
    void setupGLByContext(const DrawContext_type pCtx, uint16_t pAction);

    /* setup matrices before rendering components */
    void _preprocessGLMatrices(ZShader *wShader);

    void drawGLByContext(const DrawContext_type pCtx);

    void setDrawUserDefined(void (*pDrawFunction)()) {drawGLUserDefined=pDrawFunction;}

    void setPosition(Vertice_type pPosition);
    glm::vec3 getPosition();
    void setRotate(float pAngle,glm::vec3 pAxis);
    void setRotateDeg(float pAngle,glm::vec3 pAxis);

/* matrices : create matrices as global to all components  : */
    /**
     * @brief createMatrices @see ZObject::createMatrices for more
     */
    void createMatrices(uint8_t pFlag);
    void createAllMatrices();

    void createModel ();
    void createView ();
    void createProjection ();
    void createNormal ();

 /* create ShaderContexts per draw context (Draw, Shape, NormVisu and UserDefined */

    int createShaderContextByName(const DrawContext_type pCtx, const char *pShaderName);
    int createShaderContextByNameCase(const DrawContext_type pCtx, const char *pShaderName);
    int createShaderContextByRank(const DrawContext_type pCtx, const long pShaderIdx);


    const char*     Name=nullptr;

    void (*drawGLUserDefined)() =nullptr;


    ZMatCtx*    MatCtx=nullptr;

 /*
    Vertice_type*   Position=nullptr;

    float           RotationAngle=0.0f;
    glm::vec3*      RotationAxis=nullptr;



    ZMat4 *   Model=nullptr;
    ZMat4*    Projection=nullptr;
    ZMat4*    View=nullptr;
    ZMat4*    Normal=nullptr;

    bool        MatricesSetUp=false;
*/

/*
    ZShader*        ShapeShader=nullptr;
    ZShader*        NormVisuShader=nullptr;
    ZShader*        DrawShader=nullptr;

    ZShaderContext* DrawContext=nullptr;
    ZShaderContext* ShapeContext=nullptr;
    ZShaderContext* NormVisuContext=nullptr;
*/
    GLenum          DrawFigure[MaxShaderContext];
    ZShaderContext* ShaderContext[MaxShaderContext];

}; // ZMetaObject


#endif // ZMETAOBJECT_H
