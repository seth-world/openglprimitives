#include "zboxcomponents.h"
#include <zobject.h>

void ZBoxComponents::setup(const float pHigh,
               const float pWidth,
               const float pDepth)
{
    TopY = (pHigh /2.0f) ;
    LowY  =  - (pHigh /2.0f) ;

    RightX = (pWidth / 2.0f);
    LeftX = - (pWidth / 2.0f);

    Frontz =  (pDepth / 2.0f);
    Backz = - (pDepth / 2.0f);

    FTL=Vertice_type(LeftX,TopY,Frontz);
    FTR =Vertice_type (RightX,TopY,Frontz);/* front Top Right corner */
    FLL=Vertice_type(LeftX,LowY,Frontz);/* front Low Left corner */
    FLR=Vertice_type(RightX,LowY,Frontz);/* front Low Right corner */

    /* mids */

    FLMid =Vertice_type(LeftX,0.0,Frontz); /* Front left mid point : y axis is centered on origin */
    FRMid =Vertice_type(RightX,0.0,Frontz); /* Front right mid point */

    /* backward face */

    BTL=Vertice_type(LeftX,TopY,Backz);/* Backward Top Left corner */
    BTR=Vertice_type(RightX,TopY,Backz);/* Backward Top Right corner */
    BLL=Vertice_type(LeftX,LowY,Backz);/* Backward Low Left corner */
    BLR=Vertice_type(RightX,LowY,Backz);

    BLMid=Vertice_type (LeftX,0.0,Backz); /* Back left mid point */
    BRMid=Vertice_type (RightX,0.0,Backz); /* Back right mid point */

    FTMid= Vertice_type (0,TopY,Frontz);/* Top face Front mid point */
    BTMid= Vertice_type (0,TopY,Backz);/* Top Face Back mid point */

    LLMid= Vertice_type (0,TopY,Frontz);/* Low face Front mid point */
    LTMid= Vertice_type (0,TopY,Backz);/* Low Face Back mid point */

}//ZBoxComponents::setup

void ZBoxComponents::setupRawVertices(ZObject& pObject)
{

    pObject.addVec3(Draw, FLR,"FLR");
    FLRIdx= (GLuint)pObject.lastVertexIdx();

    pObject.addVec3(Draw, FTR,"FTR");
    FTRIdx= (GLuint)pObject.lastVertexIdx();

    pObject.addVec3(Draw, FTL,"FTL");
    FTLIdx= (GLuint)pObject.lastVertexIdx();

    pObject.addVec3(Draw, FLL,"FLL");
    FLLIdx= (GLuint)pObject.lastVertexIdx();


    pObject.addVec3(Draw, BLR,"BLR");
    BLRIdx= (GLuint)pObject.lastVertexIdx();

    pObject.addVec3(Draw, BTR,"BTR");
    BTRIdx= (GLuint)pObject.lastVertexIdx();

    pObject.addVec3(Draw, BTL,"BTL");
    BTLIdx= (GLuint)pObject.lastVertexIdx();

    pObject.addVec3(Draw, BLL,"BLL");
    BLLIdx= (GLuint)pObject.lastVertexIdx();
}//setupRawVertices

int ZBoxComponents::generateShape(ZObject& pObject,const char*pShaderName)
{

    zbs::ZArray<GLuint>       ShapeIndices;

    /* front face counter-clockwise */

    /* for GL_LINE_LOOP */
    //#ifdef __COMMENT__
        ShapeIndices << FTRIdx;
        ShapeIndices << FTLIdx;
        ShapeIndices << FLLIdx;
        ShapeIndices << FLRIdx;
        ShapeIndices << FTRIdx;

       /* closing the figure back to FTR point */
    //#endif
    /* Top face counter-clockwise  starting from wFTRIdx*/

        ShapeIndices << FTRIdx;
        ShapeIndices << BTRIdx;
        ShapeIndices << BTLIdx;
        ShapeIndices << FTLIdx;
        ShapeIndices << FTRIdx;
        /* closing the figure back to FTRpoint */

    /* Low Face */
        ShapeIndices << FLRIdx;
        ShapeIndices << FLLIdx;
        ShapeIndices << BLLIdx;
        ShapeIndices << BLRIdx;
        ShapeIndices << FLRIdx;

    /* Back Face */
        ShapeIndices << BLRIdx;
        ShapeIndices << BTRIdx;
        ShapeIndices << BTLIdx;
        ShapeIndices << BLLIdx;
        ShapeIndices << BLRIdx;

        ShapeIndices << BTRIdx; /* closing the volume loop */
        ShapeIndices << FTRIdx;

        /* closing the figure back to BTR point */

        /* OK */

        pObject.GLDesc[Shape]=new ZGLObjDescriptor(pObject.GLDesc[Draw]->VertexData,&ShapeIndices);

        if (pShaderName)
                return pObject.createShaderContextByName(Shape,pShaderName);
        return 0;
} // ZBoxComponents::generateShape


void ZBoxComponents::_cloneFrom (const ZBoxComponents &pIn)
{

    TopY=pIn.TopY ;
    LowY=pIn.LowY ;


    RightX=pIn.RightX ;
    LeftX=pIn.LeftX ;

    Frontz=pIn.Frontz;
    Backz=pIn.Backz ;

    FTL=pIn.FTL ;
    FTR=pIn.FTR ;/* front Top Right corner */
    FLL=pIn.FLL ;/* front Low Left corner */
    FLR=pIn.FLR ;/* front Low Right corner */

    /* mids */

    FLMid=pIn.FLMid ; /* Front left mid point : y axis is centered on origin */
    FRMid=pIn.FRMid ; /* Front right mid point */

    /* backward face */

    BTL=pIn.BTL;/* Backward Top Left corner */
    BTR=pIn.BTR;/* Backward Top Right corner */
    BLL=pIn.BLL;
    BLR=pIn.BLR;

    BLMid=pIn.BLMid; /* Back left mid point */
    BRMid=pIn.BRMid; /* Back right mid point */

    FTMid=pIn.FTMid;/* Top face Front mid point */
    BTMid=pIn.BTMid;/* Top Face Back mid point */

    LLMid=pIn.LLMid;/* Low face Front mid point */
    LTMid=pIn.LTMid;/* Low Face Back mid point */


    FLRIdx=pIn.FLRIdx;
    FTRIdx=pIn.FTRIdx;
    FTLIdx=pIn.FTLIdx;
    FLLIdx=pIn.FLLIdx;

    BLRIdx=pIn.BLRIdx;
    BTRIdx=pIn.BTRIdx;
    BTLIdx=pIn.BTLIdx;
    BLLIdx=pIn.BLLIdx;

}
