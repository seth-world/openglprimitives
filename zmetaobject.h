#ifndef ZMETAOBJECT_H
#define ZMETAOBJECT_H
#define __USE_ZARRAY_COPY_CONTRUCTOR__
#include <zglconstants.h>
#include <ztoolset/zarray.h>
#include <zobject.h>


class ZMetaObject : public zbs::ZArray<ZObject*>
{
public:
    ZMetaObject(const char*pName);

    ZMetaObject(ZMetaObject& pIn):zbs::ZArray<ZObject*> (pIn)  {}
    ZMetaObject(ZMetaObject&& pIn):zbs::ZArray<ZObject*> (pIn)  {}

    ZMetaObject& operator=(ZMetaObject&& pIn) {_cloneFrom(pIn); return *this;}
    ZMetaObject& operator=(ZMetaObject pIn) {_cloneFrom(pIn); return *this;}


    void generateNormVisu()
        {
            for (long wi=0;wi < count();wi++)
            {
                Tab[wi]->generateNormVisu();
            }
            return;
        }

    void add(ZObject* pObject)
    {
        push(pObject);
    }
/*    void add(ZObject&& pObject)
    {
        push(pObject);
    }
*/
    ZMetaObject& operator << (ZObject* pIn) {add(pIn); return *this;}

    void setDefaultColorAll(const Color_type pColor)
    {
        for (long wi=0;wi < count();wi++)
        {
            Tab[wi]->setDefaultColor(pColor);
        }
    }
    void setDefaultColor(const long pIdx,const Color_type pColor)
    {
            Tab[pIdx]->setDefaultColor(pColor);
    }
    void setDefaultAlphaAll(float pAlpha)
    {
        for (long wi=0;wi < count();wi++)
        {
            Tab[wi]->setDefaultAlpha(pAlpha);
        }
    }
    void setDefaultAlpha(const long pIdx,const float pAlpha)
    {
            Tab[pIdx]->setDefaultAlpha(pAlpha);
    }
    void setDrawFigureAll(GLenum pMode)
    {
        for (long wi=0;wi < count();wi++)
        {
            Tab[wi]->setDrawFigure(pMode);
        }
    }
    void setDrawFigure(long pIdx,GLenum pMode)
    {
            Tab[pIdx]->setDrawFigure(pMode);
    }

    void setTexture(ZTexture* pTexture)
    {
        for (long wi=0;wi < count();wi++)
        {
            Tab[wi]->setTexture(pTexture);
        }
    }

    ZTexture* getTexture()  {return Tab[0]->GLDescriptor->Texture;} /* share already set texture to other objects */

    void setUseTextureAll(bool pOnOff)
    {
        for (long wi=0;wi < count();wi++)
        {
            Tab[wi]->setUseTexture(pOnOff);
        }

    }
    void setUseTexture(const long pIdx,bool pOnOff)
    {
            Tab[pIdx]->setUseTexture(pOnOff);
    }

    void setMaterialAll(const ZMaterial& pMaterial)
    {
        for (long wi=0;wi < count();wi++)
        {
            Tab[wi]->setMaterial(pMaterial);
        }
    }
    void setMaterial(const long pIdx,const ZMaterial& pMaterial)
    {
            Tab[pIdx]->setMaterial(pMaterial);
    }

    void setComputeNormals(bool pOnOff)
    {
        for (long wi=0;wi < count();wi++)
        {
            Tab[wi]->setComputeNormals(pOnOff);
        }

    }
    void setComputeTexCoords(bool pOnOff)
    {
        for (long wi=0;wi < count();wi++)
        {
            Tab[wi]->setComputeTexCoords(pOnOff);
        }
    }

    void setShader(const long pIdx,ZShader*pShader)
    {
        Tab[pIdx]->setShader(pShader);
    }
    void setShaderAll(ZShader*pShader)
    {
        for (long wi=0;wi < count();wi++)
        {
            Tab[wi]->setShader(pShader);
        }
    }

/* DrawFigure  (GLenum equal to GL_TRIANGLES or GL_LINES ...) must be set Object per Object */
    void setupGL(ZShader *pShader,
                 uint8_t    pShaderSetupOpt=ZObject::setupVertices,
                 ZTexture *pTexture=nullptr)
    {
        for (long wi=0;wi < count();wi++)
        {
            Tab[wi]->setupGL(pShader,pShaderSetupOpt,Tab[wi]->DrawFigure,pTexture);
        }

    }
    /* uses the specific per object shader
     * shader must be defined object per object using setShader() routine
     * or shader must have been defined during elementary object implementation
     */
    void setupGL(uint8_t    pShaderSetupOpt=ZObject::setupVertices,
                 ZTexture *pTexture=nullptr)
    {
        for (long wi=0;wi < count();wi++)
        {
            Tab[wi]->setupGL(Tab[wi]->Shader,pShaderSetupOpt,Tab[wi]->DrawFigure,pTexture);
        }

    }

    /* ZCandy : get hidden by overloading method */
    void setupGLShape(ZShader* pShader)
    {
        for (long wi=0;wi < count();wi++)
        {
            Tab[wi]->setupGLShape(pShader);
        }

    }

    /* uses the specific per object shader
     * shader must be defined object per object using setShader() routine
     * or shader must have been defined during elementary object implementation
     */
    void setupGLShape()
    {
        for (long wi=0;wi < count();wi++)
        {
            Tab[wi]->setupGLShape(Tab[wi]->Shader);
        }

    }

    void setupGLNormalVisu(ZShader* pShader)
    {
        for (long wi=0;wi < count();wi++)
        {
            Tab[wi]->setupGLNormalVisu(pShader);
        }

    }


    void drawGL(ZShader *pShader)
    {
        for(long wi=0;wi<count();wi++)
                Tab[wi]->drawGL(pShader,Tab[wi]->DrawFigure);
    }
    void drawGL()
    {
        for(long wi=0;wi<count();wi++)
                Tab[wi]->drawGL(Tab[wi]->Shader,Tab[wi]->DrawFigure);
    }

    void drawGLNormalVisu(ZShader* pShader)
    {
        for(long wi=0;wi<count();wi++)
                Tab[wi]->drawGLNormalVisu(pShader);

    }
    void drawGLShape(ZShader* pShader)
    {
        for(long wi=0;wi<count();wi++)
                Tab[wi]->drawGLShape(pShader);

    }

    void setPosition(Vertice_type pPosition)
    {
        DefaultPosition=pPosition;
        /* no
        for (long wi=0;wi<count();wi++)
                Tab[wi]->DefaultPosition += DefaultPosition;
                */
    }

    const char*     Name=nullptr;
    Vertice_type    DefaultPosition=ZModelOrigin;

}; // ZMetaObject


#endif // ZMETAOBJECT_H
