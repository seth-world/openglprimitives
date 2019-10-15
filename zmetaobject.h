#ifndef ZMETAOBJECT_H
#define ZMETAOBJECT_H
#define __USE_ZARRAY_COPY_CONTRUCTOR__
#include <zglconstants.h>
#include <ztoolset/zarray.h>
#include <zobject.h>


class ZMetaObject : public zbs::ZArray<ZObject>
{
public:
    ZMetaObject(const char*pName);

    ZMetaObject(ZMetaObject& pIn):zbs::ZArray<ZObject> (pIn)  {}
    ZMetaObject(ZMetaObject&& pIn):zbs::ZArray<ZObject> (pIn)  {}
    void generateNormVisu()
        {
            for (long wi=0;wi < count();wi++)
            {
                Tab[wi].generateNormVisu();
            }
            return;
        }

    void add(const ZObject& pObject)
    {
        push(pObject);
    }
    void add(const ZObject&& pObject)
    {
        push(pObject);
    }
    ZMetaObject& operator << (const ZObject& pIn) {add(pIn); return *this;}
    ZMetaObject& operator << (const ZObject&& pIn) {add(pIn); return *this;}

    void setDefaultColor(Color_type pColor)
    {
        for (long wi=0;wi < count();wi++)
        {
            Tab[wi].setDefaultColor(pColor);
        }
    }
    void setDefaultAlpha(float pAlpha)
    {
        for (long wi=0;wi < count();wi++)
        {
            Tab[wi].setDefaultAlpha(pAlpha);
        }
    }
    void setDrawFigure(unsigned int pMode)
    {
        for (long wi=0;wi < count();wi++)
        {
            Tab[wi].setDrawFigure(pMode);
        }
    }
    ZTexture* getTexture()  {return Tab[0].GLDescriptor->Texture;} /* share already set texture to other objects */

    void setUseTexture(bool pOnOff)
    {
        for (long wi=0;wi < count();wi++)
        {
            Tab[wi].setUseTexture(pOnOff);
        }

    }


    void setComputeNormals(bool pOnOff)
    {
        for (long wi=0;wi < count();wi++)
        {
            Tab[wi].setComputeNormals(pOnOff);
        }

    }
    void setComputeTexCoords(bool pOnOff)
    {
        for (long wi=0;wi < count();wi++)
        {
            Tab[wi].setComputeTexCoords(pOnOff);
        }
    }
    void setDrawFigure(const long pObjectIdx,GLenum pDrawFigure)
    {
        Tab[pObjectIdx].DrawFigure=pDrawFigure;
    }
    void setShader(const long pObjIdx,ZShader*pShader)
    {
        Tab[pObjIdx].setShader(pShader);
    }

/* DrawFigure  (GLenum equal to GL_TRIANGLES or GL_LINES ...) must be set Object per Object */
    void setupGL(ZShader *pShader,
                 uint8_t    pShaderSetupOpt=ZObject::SetupVertices,
                 ZTexture *pTexture=nullptr)
    {
        for (long wi=0;wi < count();wi++)
        {
            Tab[wi].setupGL(pShader,pShaderSetupOpt,Tab[wi].DrawFigure,pTexture);
        }

    }
    /* uses the specific per object shader
     * shader must be defined object per object using setShader() routine
     * or shader must have been defined during elementary object implementation
     */
    void setupGL(uint8_t    pShaderSetupOpt=ZObject::SetupVertices,
                 ZTexture *pTexture=nullptr)
    {
        for (long wi=0;wi < count();wi++)
        {
            Tab[wi].setupGL(Tab[wi].Shader,pShaderSetupOpt,Tab[wi].DrawFigure,pTexture);
        }

    }

    void setupGLShape(ZShader* pShader)
    {
        for (long wi=0;wi < count();wi++)
        {
            Tab[wi].setupGLShape(pShader);
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
            Tab[wi].setupGLShape(Tab[wi].Shader);
        }

    }

    void setupGLNormalVisu(ZShader* pShader)
    {
        for (long wi=0;wi < count();wi++)
        {
            Tab[wi].setupGLNormalVisu(pShader);
        }

    }


    void drawGL(ZShader *pShader)
    {
        for(long wi=0;wi<count();wi++)
                Tab[wi].drawGL(pShader,Tab[wi].DrawFigure);
    }
    void drawGL()
    {
        for(long wi=0;wi<count();wi++)
                Tab[wi].drawGL(Tab[wi].Shader,Tab[wi].DrawFigure);
    }

    void drawGLNormalVisu(ZShader* pShader);
    void drawGLShape(ZShader* pShader);


    const char* Name=nullptr;


}; // ZMetaObject


#endif // ZMETAOBJECT_H
