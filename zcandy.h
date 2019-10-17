#ifndef ZCANDY_H
#define ZCANDY_H

#define __USE_ZARRAY_COPY_CONTRUCTOR__
#include <zresource.h>
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
        while (count())
            delete popR();

    }
    ZCandy& operator=(ZCandy&& pIn) {_cloneFrom(pIn); return *this;}
    ZCandy& operator=(ZCandy pIn) {_cloneFrom(pIn); return *this;}

    /* Shape has a special processing with ZCandy */
    void setupGLShape(ZShader* pShader);
    void drawGLShape(ZShader* pShader);

    GLuint                      BackStart=0;
    ZGLObjDescriptor*           FrontShapeDesc=nullptr;
//    ZGLObjDescriptor*           BackShapeDesc=nullptr;
    zbs::ZArray<Vertice_type>   FrontShape;
    zbs::ZArray<Vertice_type>   BackShape;

};

#endif // ZCANDY_H
