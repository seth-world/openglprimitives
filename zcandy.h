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
protected:
    void _cloneFrom(ZCandy& pIn)
    {
        for (long wi=0;wi<FrontShape.count();wi++)
                FrontShape.push_back(pIn.FrontShape[wi]);
        for (long wi=0;wi<BackShape.count();wi++)
                BackShape.push_back(pIn.BackShape[wi]);
        ZMetaObject::_cloneFrom(pIn);
    }
    void _cloneFrom(ZCandy&& pIn)
    {
        for (long wi=0;wi<FrontShape.count();wi++)
                FrontShape.push_back(pIn.FrontShape[wi]);
        for (long wi=0;wi<BackShape.count();wi++)
                BackShape.push_back(pIn.BackShape[wi]);
        ZMetaObject::_cloneFrom(pIn);
    }
    ZCandy()=default;
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
    void setupGLShape(CSO_type pAction);
    void computeShapeOp();
    void drawGLShape();

    /* 2 overloads for taking care of special Shape processing for ZCandy */

    void setupGLByContext(const DrawContext_type pCtx, CSO_type pAction=CSO_setupVertices);
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

    TextZone getTextZone() {return Tab[0]->getTextZone();}
     TextZone getTextZoneHigh( const float pMargin=0.01f) {        return _getTextZone(3.0,pMargin); }
    TextZone getTextZoneNormal( const float pMargin=0.01f) {        return _getTextZone(5.0,pMargin); }
    TextZone getTextZoneThin( const float pMargin=0.01f) {        return _getTextZone(8.0,pMargin); }


    TextZone _getTextZone(double pDiv=5.0, const float pMargin=0.01f) ;

    long                        BackStart=0;
    long                        BackCount=0;
    ZGLObjDescriptor*           FrontShapeDesc=nullptr;
    zbs::ZArray<ZVertice>       FrontShape;
    zbs::ZArray<ZVertice>       BackShape;
    zbs::ZArray<glm::vec3>      ShapeOp;

    float                       ShapeDistance=0.01f;


};

#endif // ZCANDY_H
