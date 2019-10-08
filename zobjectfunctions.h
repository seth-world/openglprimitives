#ifndef ZOBJECTFUNCTIONS_H
#define ZOBJECTFUNCTIONS_H

#define __USE_ZARRAY_COPY_CONTRUCTOR__
#include <ztoolset/zarray.h>
#include <glm/glm.hpp>
#include <zobject.h>

unsigned int loadTexture(char const * path);

Vertice_type CalculateSurfaceNormal (Vertice_type* pTriangle);

zbs::ZArray<Vertice_type>  generateVNormal(zbs::ZArray<Vertice_type> &wVertex,
                                           zbs::ZArray<ZObject::NormalDirection> &pNormDir);

ZObject boxIndexSetup (const float pHigh,
                       const float pWidth,
                       const float pDepth,
                       zbs::ZArray<Vertice_type>* pMids=nullptr,
                       const char*pName=nullptr);
ZObject boxSetup (const Color_type &pColor,
                  const float pHigh,
                  const float pWidth,
                  const float pDepth,
                  zbs::ZArray<Vertice_type>* pMids=nullptr,
                  const char*pName=nullptr);

ZObject openboxSetup(const float pHigh,
                     const float pWidth,
                     const float pDepth,
                     zbs::ZArray<Vertice_type>* pMids=nullptr,
                     const char*pName=nullptr);


zbs::ZArray<Vertice_type> perfect_arc_right(Vertice_type pCenter,float pRadiusLen,  int pNumber);
zbs::ZArray<Vertice_type> perfect_arc_left(Vertice_type pCenter,float pRadiusLen,  int pNumber);

ZObject generate_Arc(Vertice_type pCenter,
                     float pRadiusLen,
                     int pNumber,
                     ZObject::Direction pDirection,
                     ZObject::NormalDirection pNormDir,
                     const char *pName=nullptr);

ZObject generate_ArcStripsRight(ZObject &pArcFront,
                                ZObject &pArcBack,
                                const char*pName=nullptr);

ZObject generate_ArcStripsLeft(ZObject &pArcFront,
                               ZObject &pArcBack,
                               const char*pName=nullptr);

ZObject generate_Circle (Color_type pColor,
                         Vertice_type pCenter,
                         float pRadiusLen,
                         int pNumber,
                         ZObject::CircleMade pCircleDir,
                         ZObject::NormalDirection pNormDir=ZObject::Front,
                         const char *pName=nullptr);

ZObject
generateCylinder(Color_type pColor,
                 Vertice_type pBeginCenter,
                 float pBeginRadius,
                 Vertice_type pTargetCenter,
                 float pTargetRadius,
                 int pNumber,
                 ZObject::CircleMade pBeginCircleState,
                 ZObject::CircleMade pTargetCircleState,
                 const char*pName=nullptr);

ZObject
generateRegularCylinder(glm::vec3 pColor,
                        Vertice_type pBeginCenter,
                        Vertice_type pTargetCenter,
                        float pRadius,
                        int pNumber,
                        ZObject::CircleMade pCircleState,
                        const char*pName=nullptr);

inline Vertice_type absVector(const Vertice_type &pVector)
{
    Vertice_type wOutVector=pVector;
    if (wOutVector.x<0.0f)
         wOutVector.x=-wOutVector.x;
    if (wOutVector.y<0.0f)
         wOutVector.y=-wOutVector.y;
    if (wOutVector.z<0.0f)
         wOutVector.z=-wOutVector.z;
    return wOutVector;
}
inline Vertice_type negVector(const Vertice_type &pVector)
{
    Vertice_type wOutVector=pVector;
    if (wOutVector.x>0.0f)
         wOutVector.x=-wOutVector.x;
    if (wOutVector.y>0.0f)
         wOutVector.y=-wOutVector.y;
    if (wOutVector.z>0.0f)
         wOutVector.z=-wOutVector.z;
    return wOutVector;
}
#endif // ZOBJECTFUNCTIONS_H
