#ifndef ZBOXCOMPONENTS_H
#define ZBOXCOMPONENTS_H

#include <glad/glad.h>
#include <zvertice.h>

class ZObject;

class ZBoxComponents
{
public:

    ZBoxComponents ()=default;
    ZBoxComponents (ZBoxComponents& pIn)
    {
        _cloneFrom(pIn);
    }
    ZBoxComponents (ZBoxComponents&& pIn)
    {
        _cloneFrom(pIn);
    }

    ZBoxComponents& operator=(ZBoxComponents&pIn) {_cloneFrom(pIn); return *this;}

    void _cloneFrom (ZBoxComponents& pIn);

    void setup(const float pHigh,
                   const float pWidth,
                   const float pDepth);

    void setupRawVertices(ZObject& pObject);

    void generateShape(ZObject& pObject);

    float TopY ;
    float LowY ;


    float RightX ;
    float LeftX ;

    float Frontz;
    float Backz ;

    Vertice_type FTL;
    Vertice_type FTR;/* front Top Right corner */
    Vertice_type FLL;/* front Low Left corner */
    Vertice_type FLR;/* front Low Right corner */

    /* mids */

    Vertice_type FLMid; /* Front left mid point : y axis is centered on origin */
    Vertice_type FRMid; /* Front right mid point */

    /* backward face */

    Vertice_type BTL;/* Backward Top Left corner */
    Vertice_type BTR;/* Backward Top Right corner */
    Vertice_type BLL;/* Backward Low Left corner */
    Vertice_type BLR;

    Vertice_type BLMid; /* Back left mid point */
    Vertice_type BRMid; /* Back right mid point */

    Vertice_type FTMid;/* Top face Front mid point */
    Vertice_type BTMid;/* Top Face Back mid point */

    Vertice_type LLMid;/* Low face Front mid point */
    Vertice_type LTMid;/* Low Face Back mid point */


    GLuint FLRIdx;
    GLuint FTRIdx;
    GLuint FTLIdx;
    GLuint FLLIdx;

    GLuint BLRIdx;
    GLuint BTRIdx;
    GLuint BTLIdx;
    GLuint BLLIdx;


};


#endif // ZBOXCOMPONENTS_H
