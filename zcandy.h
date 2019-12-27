#ifndef ZCANDY_H
#define ZCANDY_H

#ifdef __USE_GLAD__
#include <glad/glad.h>
#else
#include <GL/glew.h>
#endif

#define __USE_ZARRAY_COPY_CONTRUCTOR__
#include <zglresource.h>
#include <zmetaobject.h>

class ZCandy : public ZMetaObject
{
private:
    void _cloneFrom(ZCandy& pIn)
    {
        for (long wi=0;wi<FrontShape.count();wi++)
                FrontShape.push_back(pIn.FrontShape[wi]);
        for (long wi=0;wi<BackShape.count();wi++)
                BackShape.push_back(pIn.BackShape[wi]);
    }
public:
    ZCandy(const char* pName):ZMetaObject(pName) {}
    /* ToDo : register GL object Candy and release GL resources */
        //{GLResources->registerObject()}
    ZCandy(ZCandy& pIn):ZMetaObject((ZMetaObject&&)pIn)
    {
        _cloneFrom((ZCandy&)pIn);
    }

    ~ZCandy()
    {
        if (FrontShapeDesc)
                delete FrontShapeDesc;
    }

    ZCandy& operator=(ZCandy&& pIn) {_cloneFrom(pIn); return *this;}
    ZCandy& operator=(ZCandy pIn) {_cloneFrom(pIn); return *this;}

    /* Shape has a special processing with ZCandy */
    void setupGLShape(uint16_t pAction);
    void computeShapeOp();
    void drawGLShape();

    /* 2 overloads for taking care of special Shape processing for ZCandy */

    void setupGLByContext(const DrawContext_type pCtx,uint8_t    pAction=CSO_setupVertices);
    void drawGLByContext(const DrawContext_type pCtx);

    void TestShaderContext()
    {
        for (long wi=0;wi < count();wi++)
            {
            for (long wj=0;wj < MaxShaderContext;wj++)
            if (Tab[wi]->ShaderContext[wj])
                    abort();
            }
    }


    long                        BackStart=0;
    long                        BackCount=0;
    ZGLObjDescriptor*           FrontShapeDesc=nullptr;
    zbs::ZArray<ZVertice>       FrontShape;
    zbs::ZArray<ZVertice>       BackShape;
    zbs::ZArray<glm::vec3>      ShapeOp;

    float                       ShapeDistance=0.01f;


};

#endif // ZCANDY_H
