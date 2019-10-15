#ifndef ZCANDY_H
#define ZCANDY_H

#include <zmetaobject.h>

class ZCandy : public ZMetaObject
{
public:
    ZCandy(const char* pName):ZMetaObject(pName) {}

    ZCandy(const ZCandy&& pIn):ZMetaObject((ZMetaObject&&)pIn)
    {
        for (long wi=0;wi<FrontShape.count();wi++)
                FrontShape.push_back(pIn.FrontShape[wi]);
        for (long wi=0;wi<BackShape.count();wi++)
                BackShape.push_back(pIn.BackShape[wi]);
    }
    void cloneFrom(const ZCandy&& pIn)
    {
        for (long wi=0;wi<FrontShape.count();wi++)
                FrontShape.push_back(pIn.FrontShape[wi]);
        for (long wi=0;wi<BackShape.count();wi++)
                BackShape.push_back(pIn.BackShape[wi]);
    }

    void setupGLShape(ZShader* pShader)
    {

    }

    void drawGLShape(ZShader* pShader)
    {

    }

    zbs::ZArray<Vertice_type> FrontShape;
    zbs::ZArray<Vertice_type> BackShape;
};

#endif // ZCANDY_H
