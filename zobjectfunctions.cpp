#include <zobjectfunctions.h>
#include <stb_image.h>
#include <zresource.h>
#include <zboxcomponents.h>
#include <zcandy.h>

/* compute a triangle's surface normal vec3 vector */

//Vertice_type CalculateSurfaceNormal (Vertice_type* pTriangle, ZObject::NormalDirection pNormDir)
Vertice_type calculateSurfaceNormal (Vertice_type* pTriangle)
{

    Vertice_type wNormal= glm::normalize(glm::cross(pTriangle[2] - pTriangle[0], pTriangle[1] - pTriangle[0]));

    return wNormal;

}//CalculateSurfaceNormal

const Vertice_type calculateCenter(Vertice_type* pTriangle)
{
    Vertice_type wCenter;

    wCenter.x=(pTriangle[0].x+pTriangle[1].x+pTriangle[2].x)/3.0f;
    wCenter.y=(pTriangle[0].y+pTriangle[1].y+pTriangle[2].y)/3.0f;
    wCenter.z=(pTriangle[0].z+pTriangle[1].z+pTriangle[2].z)/3.0f;
    return wCenter;
}//calculateCenter


zbs::ZArray<Vertice_type>  generateVNormal(zbs::ZArray<Vertice_type> &wVertex,
                                           zbs::ZArray<ZObject::NormalDirection>& pNormDir)
{
    Vertice_type wNormal;
    Vertice_type wTriangle[3];
    zbs::ZArray<Vertice_type> pReturn;
    long wNormIdx=0;
    for (unsigned int wi=0;wi<wVertex.size();wi=wi+3)
       {
        if ((wVertex.size()-wi)<3)
        {
          fprintf (stderr," Vertices object are not grouped per triangle\n");
          return wNormal;
        }
        if (wNormIdx>=pNormDir.size())
        {
        fprintf (stderr," Normal directions are not homogeneous per triangles for object\n");
        exit (EXIT_FAILURE);
        }
       switch (pNormDir[wNormIdx])
           {
           case ZObject::Front:
               wNormal= ZObject::NDirFront;
           break;
           case ZObject::Back:
               wNormal= ZObject::NDirBack;
           break;
           case ZObject::Top:
               wNormal= ZObject::NDirTop;
           break;
           case ZObject::Bottom:
               wNormal= ZObject::NDirBottom;
           break;
           case ZObject::Left:
               wNormal= ZObject::NDirLeft;
           break;
           case ZObject::Right:
               wNormal= ZObject::NDirRight;
           break;
           default: /* case ZObject::Compute */
           wTriangle[0]=wVertex[wi];/* compute normal for a triangle A B C */
           wTriangle[1]=wVertex[wi+1];
           wTriangle[2]=wVertex[wi+2];
           wNormal=calculateSurfaceNormal(wTriangle);
           } // switch
       pReturn.push_back(wNormal); /* per vertex normal : same for all three vertexes */
       pReturn.push_back(wNormal);
       pReturn.push_back(wNormal);

       wNormIdx++;
       }// for loop

    return pReturn;
}

/*  PI/2 --> 0
 *
 *           2*PI ---> 3*PI/2
 */
zbs::ZArray<Vertice_type> perfect_arc_right(Vertice_type pCenter,float pRadiusLen,  int pNumber)
{

//#define NUMBER_OF_VERTICES 16

double wRadius = pRadiusLen;
zbs::ZArray<Vertice_type> vertexBuffer;


Vertice_type wPoint;
//double wIncrement =  2.0 *(double)M_PI /(double)pNumber ;

//double wLimit = 2.0 * (double)M_PI;
double wLimit = (double)M_PI;

/* left from PI / 2  (top- 90°) -> PI ->  to 3*PI / 2   (bottom - 270 °) */

/* rigth from  PI/2 (top-90°) -> 0 -> 3*PI/2 */

    vertexBuffer.push_back(pCenter); /* center is always first element */

    double wT=(double)M_PI/2.0;
    double wIncrement =  (double)M_PI /(double)pNumber ;
    while (wT>0)
    {
        wPoint.x=pCenter.x + (cos(wT) * wRadius);
        wPoint.y=pCenter.y + (sin(wT) * wRadius);
        wPoint.z=pCenter.z;
        vertexBuffer.push_back( wPoint);
        wT-=wIncrement;
    }
    wPoint.x=pCenter.x + (cos(0.0) * wRadius);
    wPoint.y=pCenter.y + (sin(0.0) * wRadius);
    wPoint.z=pCenter.z;
    vertexBuffer.push_back( wPoint);

    wT = 2.0*(double)M_PI;
    wT -= wIncrement;
    wLimit = 3.0*(double)M_PI/2.0;
    while (wT>=wLimit)
    {
        wPoint.x=pCenter.x + (cos(wT) * wRadius);
        wPoint.y=pCenter.y + (sin(wT) * wRadius);
        wPoint.z=pCenter.z;
        vertexBuffer.push_back( wPoint);
        wT-=wIncrement;
    }
/*
    for(double i = M_PI/2.0; i <  wLimit; i += wIncrement){
        wPoint.x=pCenter.x + (cos(i) * wRadius);
        wPoint.y=pCenter.y + (sin(i) * wRadius);
        wPoint.z=pCenter.z;
        vertexBuffer.push_back( wPoint);
        }// for
        */
    return vertexBuffer;
}//perfect_arc_right
/*
 *                  PI/2
 *
 *
 *      PI                          0-2*PI
 *
 *
 *
 *
 *                  3PI/2
 */



zbs::ZArray<Vertice_type> perfect_arc_left(Vertice_type pCenter,float pRadiusLen,  int pNumber)
{

//#define NUMBER_OF_VERTICES 16

double wRadius = pRadiusLen;
zbs::ZArray<Vertice_type> vertexBuffer;


Vertice_type wPoint;
double wIncrement =  (double)M_PI /(double)pNumber ;
//double wLimit = - 2.0 * (double)M_PI;

/* left from PI / 2  (top- 90°) -> PI ->  to 3*PI / 2   (bottom - 270 °) */

/* rigth from  PI/2 (top-90°) -> 0 -> 3*PI/2 */

    vertexBuffer.push_back(pCenter); /* center is always first element */

    double wT= M_PI/2.0;
    double wLimit = 3.0 *(double) M_PI / 2.0;


    while (wT <= wLimit)
    {
        wPoint.x=pCenter.x+(cos(wT) * wRadius);
        wPoint.y=pCenter.y+(sin(wT) * wRadius);
        wPoint.z=pCenter.z;
        vertexBuffer.push_back( wPoint);
        wT+= wIncrement;
    }

    return vertexBuffer;
}//perfect_arc_setup
zbs::ZArray<Vertice_type> face_circle(Vertice_type pCenter,float pRadiusLen,  int pNumber)
{

//#define NUMBER_OF_VERTICES 16

double wRadius = pRadiusLen;
zbs::ZArray<Vertice_type> vertexBuffer;

const double wM_PI= (double)M_PI;

Vertice_type wPoint;
double wIncrement =  (2.0*wM_PI) /(double)pNumber ;
//double wLimit = - 2.0 * (double)M_PI;

/* left from PI / 2  (top- 90°) -> PI ->  to 3*PI / 2   (bottom - 270 °) */

/* rigth from  PI/2 (top-90°) -> 0 -> 3*PI/2 */

    vertexBuffer.push_back(pCenter); /* center is always first element */

    double wT= 0;
    double wLimit = 2.0 *wM_PI;


    while (wT <= wLimit+1)
    {
        wPoint.x=pCenter.x+(cos(wT) * wRadius);
        wPoint.y=pCenter.y+(sin(wT) * wRadius);
        wPoint.z=pCenter.z;
        vertexBuffer.push_back( wPoint);
        wT+= wIncrement;
    }

    return vertexBuffer;
}//facing_circle
zbs::ZArray<Vertice_type> sequent_circle(Vertice_type pCenter,float pRadiusLen,  int pNumber)
{

//#define NUMBER_OF_VERTICES 16

double wRadius = pRadiusLen;
zbs::ZArray<Vertice_type> vertexBuffer;

const double wM_PI= (double)M_PI;

Vertice_type wPoint;
double wIncrement =  (2.0*wM_PI) /(double)pNumber ;
//double wLimit = - 2.0 * (double)M_PI;

/* left from PI / 2  (top- 90°) -> PI ->  to 3*PI / 2   (bottom - 270 °) */

/* rigth from  PI/2 (top-90°) -> 0 -> 3*PI/2 */

    vertexBuffer.push_back(pCenter); /* center is always first element */

    double wT= 0;
    double wLimit = 2.0 *wM_PI;


    while (wT <= wLimit+1)
    {
        wPoint.x=pCenter.x;
        wPoint.y=pCenter.y+(sin(wT) * wRadius);
        wPoint.z=pCenter.z+(cos(wT) * wRadius);
        vertexBuffer.push_back( wPoint);
        wT+= wIncrement;
    }

    return vertexBuffer;
}//flat_circle

zbs::ZArray<Vertice_type> flat_circle(Vertice_type pCenter,float pRadiusLen,  int pNumber)
{

//#define NUMBER_OF_VERTICES 16

double wRadius = pRadiusLen;
zbs::ZArray<Vertice_type> vertexBuffer;

const double wM_PI= (double)M_PI;

Vertice_type wPoint;
double wIncrement =  (2.0*wM_PI) /(double)pNumber ;
//double wLimit = - 2.0 * (double)M_PI;

/* left from PI / 2  (top- 90°) -> PI ->  to 3*PI / 2   (bottom - 270 °) */

/* rigth from  PI/2 (top-90°) -> 0 -> 3*PI/2 */

    vertexBuffer.push_back(pCenter); /* center is always first element */

    double wT= 0;
    double wLimit = 2.0 *wM_PI;


    while (wT <= wLimit+1)
    {
        wPoint.x=pCenter.x+(cos(wT) * wRadius);
        wPoint.y=pCenter.y;
        wPoint.z=pCenter.z+(sin(wT) * wRadius);
        vertexBuffer.push_back( wPoint);
        wT+= wIncrement;
    }

    return vertexBuffer;
}//flat_circle

/**
 * @brief generate_Arc
 *
 *  Creates a plane surface as an arc of circle equals to half a circle with center pCenter.
 *
 * @param pStart
 * @param pEnd
 * @param pDirection ZObject::Direction either 0 (right) or (-1) left
 * @param pNumber
 */


ZObject generate_Arc(Vertice_type pCenter,
                     float pRadiusLen,
                     int pNumber,
                     ZObject::Direction pDirection,
                     ZObject::NormalDirection pNormDir,
                     const char*pName)
{
ZObject wArc("wArc");
zbs::ZArray<Vertice_type> vertexBuffer;


if (pDirection<0)
    {
//     pCenter.x -= 0.25 ;
    vertexBuffer = perfect_arc_left(pCenter,pRadiusLen,pNumber);
    }
    else
    {
//    pCenter.x += 0.25 ;
    vertexBuffer = perfect_arc_right(pCenter,pRadiusLen,pNumber);
    }
    for (int wi=2;wi<vertexBuffer.size();wi++)
        {
        wArc << vertexBuffer[0];
        wArc << vertexBuffer[wi-1];
        wArc << vertexBuffer[wi];
        wArc.VNormalDir.push_back( pNormDir); /* one direction per triangle */

        wArc.ShapeIndices << (GLuint)wArc.lastVertexIdx()-1;
        wArc.ShapeIndices << (GLuint)wArc.lastVertexIdx();
        }
    wArc.setDrawFigure(GL_TRIANGLE_FAN);
    return wArc;
}//generate_Arc

ZObject
generate_ArcStripsRight(ZObject &pArcFront,
                        ZObject &pArcBack,
                        const char *pName)
{
ZObject wArcStrips(pName);


    /* Nota Bene : no ShapeIndices for this object */

        /* start skipping arc center (first) */
        for (long wi=1; (wi < pArcFront.verticeCount())&&(wi<pArcBack.verticeCount());wi+=3) /* each 2 and skip arc center*/
            {
            /* first triangle counter-clockwise*/
            wArcStrips << pArcFront[wi+1];
            wArcStrips << pArcFront[wi];
            wArcStrips << pArcBack[wi];

            wArcStrips.VNormalDir.push_back( ZObject::Compute); /* one normal direction per triangle */

            /* second adjacent triangle counter-clockwise*/
            wArcStrips << pArcBack[wi];
            wArcStrips << pArcBack[wi+1];
            wArcStrips << pArcFront[wi+1];


            wArcStrips.VNormalDir.push_back( ZObject::Compute); /* one normal direction per triangle */
            }
    wArcStrips.setDrawFigure(GL_TRIANGLES);
    return wArcStrips;
}//generate_ArcStrips
ZObject generate_ArcStripsLeft(ZObject &pArcFront,
                               ZObject &pArcBack,
                               const char *pName)
{
ZObject wArcStrips(pName);

    /* start skipping arc center (first) */
    for (long wi=1; (wi < pArcFront.verticeCount())&&(wi<pArcBack.verticeCount());wi+=3) /* each 2 and skip arc center*/
        {
        /* first triangle counter-clockwise*/
        wArcStrips << pArcFront[wi];
        wArcStrips << pArcFront[wi+1];
        wArcStrips << pArcBack[wi];

//        wArcStrips.VNormalDir.push_back( pNormDir); /* one normal direction per triangle */
        wArcStrips.VNormalDir.push_back( ZObject::Compute); /* one normal direction per triangle */

        /* second adjacent triangle counter-clockwise*/
        wArcStrips << pArcFront[wi+1];
        wArcStrips << pArcBack[wi+1];
        wArcStrips << pArcBack[wi];

        wArcStrips.VNormalDir.push_back( ZObject::Compute); /* one normal direction per triangle */
        }//for

    wArcStrips.setDrawFigure(GL_TRIANGLES);

    return wArcStrips;
}//generate_ArcStripsLeft


ZObject generate_Circle (Color_type pColor,
                         Vertice_type pCenter,
                         float pRadiusLen,
                         int pNumber,
                         ZObject::CircleMade pCircleDir,
                         ZObject::NormalDirection pNormDir,
                         const char*pName)
{
ZObject wCircle(pName);
zbs::ZArray<Vertice_type> vertexBuffer;

    wCircle.setDefaultColor(pColor);

    if (pCircleDir==ZObject::Face)
    {
       vertexBuffer = face_circle(pCenter,pRadiusLen,pNumber);
    }
    else
    if (pCircleDir==ZObject::Sequent)
    {
       vertexBuffer = sequent_circle(pCenter,pRadiusLen,pNumber);
    }
    else
    if (pCircleDir==ZObject::Flat)
    {
       vertexBuffer = flat_circle(pCenter,pRadiusLen,pNumber);
    }

    for (int wi=2;wi<vertexBuffer.size();wi++)
        {
        wCircle << vertexBuffer[0];
        wCircle << vertexBuffer[wi-1];
        wCircle << vertexBuffer[wi];
        wCircle.VNormalDir.push_back( pNormDir);/* one normal direction per triangle */

        }
    wCircle.setDrawFigure(GL_TRIANGLE_FAN);
    return wCircle;
}//generate_Circle

ZObject boxIndexSetup (const float pHigh,
                       const float pWidth,
                       const float pDepth,
                       const char* pName)
{
    ZBoxComponents pBoxComponents;
    return boxIndexSetup(pHigh,pWidth,pDepth,pBoxComponents,pName);
}


ZObject boxIndexSetup (const float pHigh,
                       const float pWidth,
                       const float pDepth,
                       ZBoxComponents& pBoxComponents,
                       const char* pName)
{
ZObject pObject(pName);

    pBoxComponents.setup(pHigh,pWidth,pDepth);

    /* clock wise */

    /* front face clock wise */
    pObject << pBoxComponents.FTL ;
    pObject.VName.push_back("FTL");
    unsigned int wFTLIdx= (unsigned int)pObject.lastVertexIdx();

    pObject << pBoxComponents.FTR ;
    pObject.VName.push_back("FTR");
    unsigned int wFTRIdx=(unsigned int) pObject.lastVertexIdx();

    pObject << pBoxComponents.FLL ;
    pObject.VName.push_back("FLL");
    unsigned int wFLLIdx= (unsigned int)pObject.lastVertexIdx();

    pObject << pBoxComponents.FLR ;
    pObject.VName.push_back("FLR");
    unsigned int wFLRIdx= (unsigned int)pObject.lastVertexIdx();

    /* back face clock wise */
    pObject << pBoxComponents.BTL ;
    pObject.VName.push_back("BTL");
    unsigned int wBTLIdx= (unsigned int)pObject.lastVertexIdx();

    pObject << pBoxComponents.BTR ;
    pObject.VName.push_back("BTR");
    unsigned int wBTRIdx= (unsigned int)pObject.lastVertexIdx();

    pObject << pBoxComponents.BLL ;
    pObject.VName.push_back("BLL");
    GLuint wBLLIdx= (GLuint)pObject.lastVertexIdx();

    pObject << pBoxComponents.BLR ;
    pObject.VName.push_back("BLR");
    unsigned int wBLRIdx= (unsigned int)pObject.lastVertexIdx();

    /* indices */
    /* front face */
    pObject.addIndice( wFLRIdx);
    pObject.addIndice( wFTRIdx);
    pObject.addIndice( wFTLIdx);


    pObject.VNormalDir.push_back( ZObject::Front);

    pObject.addIndice( wFTLIdx);
    pObject.addIndice( wFLLIdx);
    pObject.addIndice( wFLRIdx);

    pObject.VNormalDir.push_back( ZObject::Front);

    /* Backward face  */
    pObject.addIndice(wBLRIdx);
    pObject.addIndice(wBTRIdx);
    pObject.addIndice(wBTLIdx);

    pObject.VNormalDir.push_back( ZObject::Back);

    pObject.addIndice(wBTLIdx);
    pObject.addIndice(wBLLIdx);
    pObject.addIndice(wBLRIdx);

    pObject.VNormalDir.push_back( ZObject::Back);

    /* Down face  */
    pObject.addIndice(wFLRIdx);
    pObject.addIndice(wBLRIdx);
    pObject.addIndice(wBLLIdx);

    pObject.VNormalDir.push_back( ZObject::Bottom);

    pObject.addIndice(wBLLIdx);
    pObject.addIndice(wFLLIdx);
    pObject.addIndice(wFLRIdx);

    pObject.VNormalDir.push_back( ZObject::Bottom);

    /* Top face  */
    pObject.addIndice(wFTRIdx);
    pObject.addIndice(wBTRIdx);
    pObject.addIndice(wBTLIdx);

    pObject.VNormalDir.push_back( ZObject::Top);

    pObject.addIndice(wBTLIdx);
    pObject.addIndice(wFTLIdx);
    pObject.addIndice(wFTRIdx);

    pObject.VNormalDir.push_back( ZObject::Top);

    /* Left face  */

    pObject.addIndice(wFTLIdx);
    pObject.addIndice(wBTLIdx);
    pObject.addIndice(wBLLIdx);

    pObject.VNormalDir.push_back( ZObject::Left);

    pObject.addIndice(wBLLIdx);
    pObject.addIndice(wFLLIdx);
    pObject.addIndice(wFTLIdx);

    pObject.VNormalDir.push_back( ZObject::Left);;

    /* Right face  */

    pObject.addIndice(wFTRIdx);
    pObject.addIndice(wBTRIdx);
    pObject.addIndice(wBLRIdx);

    pObject.VNormalDir.push_back( ZObject::Right);

    pObject.addIndice(wBLRIdx);
    pObject.addIndice(wFLRIdx);
    pObject.addIndice(wFTRIdx);

    pObject.VNormalDir.push_back( ZObject::Right);

    /* OK */

/* end indices */
    pObject.setDrawFigure(GL_TRIANGLES);
    return pObject;
}//boxIndexSetup

ZObject boxSetup_old (const float pHigh,
                  const float pWidth,
                  const float pDepth,
                  ZBoxComponents& pComponents,
                  const char*pName)
{
ZObject pObject(pName);
/* remark : coords must remain positive -> to be addressed */

    pComponents.setup (pHigh,pWidth,pDepth);

    float wTopY = (pHigh /2.0f) ;
    float wLowY  =  - (pHigh /2.0f) ;

    float wLeftX = - (pWidth / 2.0f);
    float wRightX = (pWidth / 2.0f);

    float wFrontz =  (pDepth / 2.0f);
    float wBackz = - (pDepth / 2.0f);

    /* front face */

    Vertice_type wFTL (wLeftX,wTopY,wFrontz);/* front Top Left corner */
    Vertice_type wFTR (wRightX,wTopY,wFrontz);/* front Top Right corner */
    Vertice_type wFLL(wLeftX,wLowY,wFrontz);/* front Low Left corner */
    Vertice_type wFLR(wRightX,wLowY,wFrontz);/* front Low Right corner */

    /* mids */

    Vertice_type wFLMid (wLeftX,0.0,wFrontz); /* Front left mid point : y axis is centered on origin */
    Vertice_type wFRMid (wRightX,0.0,wFrontz); /* Front right mid point */

    /* backward face */

    Vertice_type wBTL(wLeftX,wTopY,wBackz);/* Backward Top Left corner */
    Vertice_type wBTR(wRightX,wTopY,wBackz);/* Backward Top Right corner */
    Vertice_type wBLL(wLeftX,wLowY,wBackz);/* Backward Low Left corner */
    Vertice_type wBLR(wRightX,wLowY,wBackz);/* Backward Low Right corner */

    /* mids */

    Vertice_type wBLMid (wLeftX,0.0,wBackz); /* Back left mid point */
    Vertice_type wBRMid (wRightX,0.0,wBackz); /* Back right mid point */
/*
    if (pMids)
        {
        pMids->push_back(wFLMid);
        pMids->push_back(wBLMid);
        pMids->push_back(wFRMid);
        pMids->push_back(wBRMid);
        }
        */
    /* front face */
    pObject.addVertice( wFTL);
    pObject.ShapeIndices << (GLuint)pObject.lastVertexIdx();
    pObject.addVertice( wFTR);
    pObject.ShapeIndices << (GLuint)pObject.lastVertexIdx();
    pObject.addVertice( wFLR);
    pObject.ShapeIndices << (GLuint)pObject.lastVertexIdx();

    pObject.addVertice( wFLR); /* skip it for shape line drawing index */
    pObject.addVertice( wFLL);
    pObject.ShapeIndices << (GLuint)pObject.lastVertexIdx();
    pObject.addVertice( wFTL);
    pObject.ShapeIndices << (GLuint)pObject.lastVertexIdx();

    pObject.VNormalDir.push_back( ZObject::Front);pObject.ShapeIndices << (GLuint)pObject.lastVertexIdx();

    pObject.VNormalDir.push_back( ZObject::Front);

    /* Backward face  */
    pObject.addVertice(wBTL);
    pObject.ShapeIndices << (GLuint)pObject.lastVertexIdx();
    pObject.addVertice(wBTR);
    pObject.ShapeIndices << (GLuint)pObject.lastVertexIdx();
    pObject.addVertice(wBLR);
    pObject.ShapeIndices << (GLuint)pObject.lastVertexIdx();

    pObject.addVertice(wBLR);/* skip it for shape line drawing index */
    pObject.addVertice(wBLL);
    pObject.ShapeIndices << (GLuint)pObject.lastVertexIdx();
    pObject.addVertice(wBTL);
    pObject.ShapeIndices << (GLuint)pObject.lastVertexIdx();

    pObject.VNormalDir.push_back( ZObject::Back);

    pObject.VNormalDir.push_back( ZObject::Back);

    /* Down face  */
    pObject.addVertice(wBLL);
    pObject.ShapeIndices << (GLuint)pObject.lastVertexIdx();
    pObject.addVertice(wBLR);
    pObject.ShapeIndices << (GLuint)pObject.lastVertexIdx();
    pObject.addVertice(wFLR);

    pObject.addVertice(wFLR);
    pObject.ShapeIndices << (GLuint)pObject.lastVertexIdx();
    pObject.addVertice(wFLL);
    pObject.ShapeIndices << (GLuint)pObject.lastVertexIdx();
    pObject.addVertice(wBLL);

    pObject.VNormalDir.push_back( ZObject::Bottom);

    pObject.VNormalDir.push_back( ZObject::Bottom);

    /* Top face  */
    pObject.addVertice(wBTL);
    pObject.ShapeIndices << (GLuint)pObject.lastVertexIdx();
    pObject.addVertice(wBTR);
    pObject.ShapeIndices << (GLuint)pObject.lastVertexIdx();
    pObject.addVertice(wFTR);

    pObject.addVertice(wFTR);
    pObject.ShapeIndices << (GLuint)pObject.lastVertexIdx();
    pObject.addVertice(wFTL);
    pObject.ShapeIndices << (GLuint)pObject.lastVertexIdx();
    pObject.addVertice(wBTL);

    pObject.VNormalDir.push_back( ZObject::Top);

    pObject.VNormalDir.push_back( ZObject::Top);

    /* Left face  */

    pObject.addVertice(wFTL);
    pObject.addVertice(wBTL);
    pObject.addVertice(wBLL);

    pObject.addVertice(wBLL);
    pObject.addVertice(wFLL);
    pObject.addVertice(wFTL);

    pObject.VNormalDir.push_back( ZObject::Left);

    pObject.VNormalDir.push_back( ZObject::Left);;

    /* Right face  */

    pObject.addVertice(wFTR);
    pObject.addVertice(wBTR);
    pObject.addVertice(wBLR);

    pObject.addVertice(wBLR);
    pObject.addVertice(wFLR);
    pObject.addVertice(wFTR);

    pObject.VNormalDir.push_back( ZObject::Right);

    pObject.VNormalDir.push_back( ZObject::Right);


    /* OK */
    pObject.setDrawFigure(GL_TRIANGLES);
/* end indices */

    return pObject;
}//boxSetup_old

ZObject boxSetup (const float pHigh,
                       const float pWidth,
                       const float pDepth,
                       const char* pName)
{
    ZBoxComponents pBoxComponents;
    return boxSetup(pHigh,pWidth,pDepth,pBoxComponents,pName);
}
ZObject boxSetup (const float pHigh,
                  const float pWidth,
                  const float pDepth,
                  ZBoxComponents& pComponents,
                  const char*pName)
{
ZObject pObject(pName);
/* remark : coords must remain positive -> to be addressed */

    pComponents.setup (pHigh,pWidth,pDepth);

    /* front face counter-clockwise */
    pObject.addVertice( pComponents.FLR,"FLR");
    pComponents.FLRIdx= (GLuint)pObject.lastVertexIdx();

    pObject.addVertice( pComponents.FTR,"FTR");
    pComponents.FTRIdx= (GLuint)pObject.lastVertexIdx();

    pObject.addVertice( pComponents.FTL,"FTL");
    pComponents.FTLIdx= (GLuint)pObject.lastVertexIdx();

    pObject.VNormalDir.push_back( ZObject::Front);

    pObject.addVertice( pComponents.FTL,"FTL");
    pObject.addVertice( pComponents.FLL,"FLL");
    pComponents.FLLIdx= (GLuint)pObject.lastVertexIdx();
    pObject.addVertice( pComponents.FLR,"FLR");


    pObject.VNormalDir.push_back( ZObject::Front);


    /* Backward face  counter-clockwise*/

    pObject.addVertice(pComponents.BLR,"BLR");
    pComponents.BLRIdx= (GLuint)pObject.lastVertexIdx();
    pObject.addVertice(pComponents.BTR,"BTR");
    pComponents.BTRIdx= (GLuint)pObject.lastVertexIdx();
    pObject.addVertice(pComponents.BTL,"BTL");
    pComponents.BTLIdx= (GLuint)pObject.lastVertexIdx();

    pObject.VNormalDir.push_back( ZObject::Back); /* one per triangle */

    pObject.addVertice(pComponents.BTL,"BTL");/* skip it for shape line drawing index */
    pObject.addVertice(pComponents.BLL,"BLL");
    pComponents.BLLIdx= (GLuint)pObject.lastVertexIdx();
    pObject.addVertice(pComponents.BLR,"BLR");


    pObject.VNormalDir.push_back( ZObject::Back); /* one per triangle */

    /* Down face  - Low face counter-clockwise*/

    pObject.addVertice(pComponents.FLR,"FLR");
    pObject.addVertice(pComponents.BLR,"BLR");
    pObject.addVertice(pComponents.BLL,"BLL");

    pObject.VNormalDir.push_back( ZObject::Bottom);/* one per triangle */

    pObject.addVertice(pComponents.BLL,"BLL");
    pObject.addVertice(pComponents.FLL,"FLL");
    pObject.addVertice(pComponents.FLR,"FLR");

    pObject.VNormalDir.push_back( ZObject::Bottom);/* one per triangle */

    /* Top face  counter-clockwise */

    pObject.addVertice(pComponents.FTR,"FTR");
    pObject.addVertice(pComponents.BTR,"BTR");
    pObject.addVertice(pComponents.BTL,"BTL");

    pObject.VNormalDir.push_back( ZObject::Top);

    pObject.addVertice(pComponents.BTL,"BTL");
    pObject.addVertice(pComponents.FTL,"FTL");
    pObject.addVertice(pComponents.FTR,"FTR");

    pObject.VNormalDir.push_back( ZObject::Top);

    /* Left face  */
    pObject.addVertice(pComponents.FTL,"FTL");
    pObject.addVertice(pComponents.BTL,"BTL");
    pObject.addVertice(pComponents.BLL,"BLL");

    pObject.VNormalDir.push_back( ZObject::Left);

    pObject.addVertice(pComponents.BLL,"BTL");
    pObject.addVertice(pComponents.FLL,"FLL");
    pObject.addVertice(pComponents.FTL,"FTL");

    pObject.VNormalDir.push_back( ZObject::Left);

    /* Right face  */
    pObject.addVertice(pComponents.FTR,"FTR");
    pObject.addVertice(pComponents.BTR,"BTR");
    pObject.addVertice(pComponents.BLR,"BLR");

    pObject.VNormalDir.push_back( ZObject::Right);

    pObject.addVertice(pComponents.BLR,"BLR");
    pObject.addVertice(pComponents.FLR,"FLR");
    pObject.addVertice(pComponents.FTR,"FTR");

    pObject.VNormalDir.push_back( ZObject::Right);


/* shape lines */

/* front face counter-clockwise */

/* for GL_LINE_LOOP */
//#ifdef __COMMENT__
    pObject.ShapeIndices << pComponents.FTRIdx;
    pObject.ShapeIndices << pComponents.FTLIdx;
    pObject.ShapeIndices << pComponents.FLLIdx;
    pObject.ShapeIndices << pComponents.FLRIdx;
    pObject.ShapeIndices << pComponents.FTRIdx;

   /* closing the figure back to FTR point */
//#endif
/* Top face counter-clockwise  starting from wFTRIdx*/

 //   pObject.ShapeIndices << pComponents.FTRIdx;
    pObject.ShapeIndices << pComponents.BTRIdx;
    pObject.ShapeIndices << pComponents.BTLIdx;
    pObject.ShapeIndices << pComponents.FTLIdx;
    pObject.ShapeIndices << pComponents.FTRIdx;
    /* closing the figure back to FTRpoint */

/* Low Face */
    pObject.ShapeIndices << pComponents.FLRIdx;
    pObject.ShapeIndices << pComponents.FLLIdx;
    pObject.ShapeIndices << pComponents.BLLIdx;
    pObject.ShapeIndices << pComponents.BLRIdx;
    pObject.ShapeIndices << pComponents.FLRIdx;

/* Back Face */
    pObject.ShapeIndices << pComponents.BLRIdx;
    pObject.ShapeIndices << pComponents.BTRIdx;
    pObject.ShapeIndices << pComponents.BTLIdx;
    pObject.ShapeIndices << pComponents.BLLIdx;
    pObject.ShapeIndices << pComponents.BLRIdx;

    pObject.ShapeIndices << pComponents.BTRIdx; /* closing the volume loop */
    pObject.ShapeIndices << pComponents.FTRIdx;

    /* closing the figure back to BTR point */

    /* OK */

    pObject.setDrawFigure(GL_TRIANGLES);

    return pObject;
}//boxSetup




ZObject openboxSetup (const float pHigh,
                       const float pWidth,
                       const float pDepth,
                      ZBoxComponents& pComponents,
                      const char *pName)
{

ZObject pObject(pName);

//    ZBoxComponents pComponents(pHigh,pWidth,pDepth);

    pComponents.setup(pHigh,pWidth,pDepth);
/* remark : coords must remain positive -> to be addressed */


    /* front face counter-clockwise */
    pObject.addVertice( pComponents.FLR,"FLR");
    pComponents.FLRIdx= (GLuint)pObject.lastVertexIdx();

    pObject.addVertice( pComponents.FTR,"FTR");
    pComponents.FTRIdx= (GLuint)pObject.lastVertexIdx();

    pObject.addVertice( pComponents.FTL,"FTL");
    pComponents.FTLIdx= (GLuint)pObject.lastVertexIdx();

    pObject.VNormalDir.push_back( ZObject::Front);

    pObject.addVertice( pComponents.FTL,"FTL");
    pObject.addVertice( pComponents.FLL,"FLL");
    pComponents.FLLIdx= (GLuint)pObject.lastVertexIdx();
    pObject.addVertice( pComponents.FLR,"FLR");


    pObject.VNormalDir.push_back( ZObject::Front);


    /* Backward face  counter-clockwise*/

    pObject.addVertice(pComponents.BLR,"BLR");
    pComponents.BLRIdx= (GLuint)pObject.lastVertexIdx();
    pObject.addVertice(pComponents.BTR,"BTR");
    pComponents.BTRIdx= (GLuint)pObject.lastVertexIdx();
    pObject.addVertice(pComponents.BTL,"BTL");
    pComponents.BTLIdx= (GLuint)pObject.lastVertexIdx();

    pObject.VNormalDir.push_back( ZObject::Back); /* one per triangle */

    pObject.addVertice(pComponents.BTL,"BTL");/* skip it for shape line drawing index */
    pObject.addVertice(pComponents.BLL,"BLL");
    pComponents.BLLIdx= (GLuint)pObject.lastVertexIdx();
    pObject.addVertice(pComponents.BLR,"BLR");


    pObject.VNormalDir.push_back( ZObject::Back); /* one per triangle */

    /* Down face  - Low face counter-clockwise*/

    pObject.addVertice(pComponents.FLR,"FLR");
    pObject.addVertice(pComponents.BLR,"BLR");
    pObject.addVertice(pComponents.BLL,"BLL");

    pObject.VNormalDir.push_back( ZObject::Bottom);/* one per triangle */

    pObject.addVertice(pComponents.BLL,"BLL");
    pObject.addVertice(pComponents.FLL,"FLL");
    pObject.addVertice(pComponents.FLR,"FLR");

    pObject.VNormalDir.push_back( ZObject::Bottom);/* one per triangle */

    /* Top face  counter-clockwise */

    pObject.addVertice(pComponents.FTR,"FTR");
    pObject.addVertice(pComponents.BTR,"BTR");
    pObject.addVertice(pComponents.BTL,"BTL");

    pObject.VNormalDir.push_back( ZObject::Top);

    pObject.addVertice(pComponents.BTL,"BTL");
    pObject.addVertice(pComponents.FTL,"FTL");
    pObject.addVertice(pComponents.FTR,"FTR");

    pObject.VNormalDir.push_back( ZObject::Top);

    /* no right no Left face  */
#ifdef __COMMENT__
/* shape lines */

/* front face counter-clockwise */

/* for GL_LINE_LOOP */
//#ifdef __COMMENT__
    pObject.ShapeIndices << pComponents.FTRIdx;
    pObject.ShapeIndices << pComponents.FTLIdx;
    pObject.ShapeIndices << pComponents.FLLIdx;
    pObject.ShapeIndices << pComponents.FLRIdx;
    pObject.ShapeIndices << pComponents.FTRIdx;

   /* closing the figure back to FTR point */
//#endif
/* Top face counter-clockwise  starting from wFTRIdx*/

    pObject.ShapeIndices << pComponents.FTRIdx;
    pObject.ShapeIndices << pComponents.BTRIdx;
    pObject.ShapeIndices << pComponents.BTLIdx;
    pObject.ShapeIndices << pComponents.FTLIdx;
    pObject.ShapeIndices << pComponents.FTRIdx;
    /* closing the figure back to FTRpoint */

/* Low Face */
    pObject.ShapeIndices << pComponents.FLRIdx;
    pObject.ShapeIndices << pComponents.FLLIdx;
    pObject.ShapeIndices << pComponents.BLLIdx;
    pObject.ShapeIndices << pComponents.BLRIdx;
    pObject.ShapeIndices << pComponents.FLRIdx;

/* Back Face */
    pObject.ShapeIndices << pComponents.BLRIdx;
    pObject.ShapeIndices << pComponents.BTRIdx;
    pObject.ShapeIndices << pComponents.BTLIdx;
    pObject.ShapeIndices << pComponents.BLLIdx;
    pObject.ShapeIndices << pComponents.BLRIdx;

    pObject.ShapeIndices << pComponents.BTRIdx; /* closing the volume loop */
    pObject.ShapeIndices << pComponents.FTRIdx;

    /* closing the figure back to BTR point */
#endif // __COMMENT__
    /* OK */

    pObject.setDrawFigure(GL_TRIANGLES);

/* end indices */

    return pObject;
}//openboxSetup
ZObject openboxIndexedSetup (const float pHigh,
                             const float pWidth,
                             const float pDepth,
                            ZBoxComponents& pComponents,
                            const char *pName)
{

ZObject pObject(pName);

//    ZBoxComponents pComponents(pHigh,pWidth,pDepth);

    pComponents.setup(pHigh,pWidth,pDepth);
/* remark : coords must remain positive -> to be addressed */

 /*   pObject.addVertice( pComponents.FLR,"FLR");
    GLuint wFLRIdx= (GLuint)pObject.lastVertexIdx();

    pObject.addVertice( pComponents.FTR,"FTR");
    GLuint wFTRIdx= (GLuint)pObject.lastVertexIdx();

    pObject.addVertice( pComponents.FTL,"FTL");
    GLuint wFTLIdx= (GLuint)pObject.lastVertexIdx();

    pObject.addVertice( pComponents.FLL,"FLL");
    GLuint wFLLIdx= (GLuint)pObject.lastVertexIdx();


    pObject.addVertice( pComponents.BLR,"BLR");
    GLuint wBLRIdx= (GLuint)pObject.lastVertexIdx();

    pObject.addVertice( pComponents.BTR,"BTR");
    GLuint wBTRIdx= (GLuint)pObject.lastVertexIdx();

    pObject.addVertice( pComponents.BTL,"BTL");
    GLuint wBTLIdx= (GLuint)pObject.lastVertexIdx();

    pObject.addVertice( pComponents.BLL,"BLL");
    GLuint wBLLIdx= (GLuint)pObject.lastVertexIdx();
*/
    pComponents.setupRawVertices(pObject);

/* front face counter-clockwise */
    pObject.Indices << pComponents.FLRIdx;
    pObject.Indices << pComponents.FTRIdx;
    pObject.Indices << pComponents.FTLIdx;

    pObject.VNormalDir.push_back( ZObject::Front);

    pObject.Indices << pComponents.FTLIdx;
    pObject.Indices << pComponents.FLLIdx;
    pObject.Indices << pComponents.FLRIdx;

    pObject.VNormalDir.push_back( ZObject::Front);

/* Backward face  counter-clockwise*/
    pObject.Indices << pComponents.BLRIdx;
    pObject.Indices << pComponents.BTRIdx;
    pObject.Indices << pComponents.BTLIdx;

    pObject.VNormalDir.push_back( ZObject::Back);

    pObject.Indices << pComponents.BTLIdx;
    pObject.Indices << pComponents.BLLIdx;
    pObject.Indices << pComponents.BLRIdx;

    pObject.VNormalDir.push_back( ZObject::Back);/* one per triangle */

 /* Down face  - Low face counter-clockwise*/

    pObject.Indices << pComponents.FLRIdx;
    pObject.Indices << pComponents.BLRIdx;
    pObject.Indices << pComponents.BLLIdx;

    pObject.VNormalDir.push_back( ZObject::Bottom);

    pObject.Indices << pComponents.BLLIdx;
    pObject.Indices << pComponents.FLLIdx;
    pObject.Indices << pComponents.FLRIdx;

    pObject.VNormalDir.push_back( ZObject::Bottom);/* one per triangle */


    /* Top face  counter-clockwise */

    pObject.Indices << pComponents.FTRIdx;
    pObject.Indices << pComponents.BTRIdx;
    pObject.Indices << pComponents.BTLIdx;

    pObject.VNormalDir.push_back( ZObject::Top);

    pObject.Indices << pComponents.BTLIdx;
    pObject.Indices << pComponents.FTLIdx;
    pObject.Indices << pComponents.FTRIdx;

    pObject.VNormalDir.push_back( ZObject::Top);/* one per triangle */

    /* no right no Left face  */

/* shape lines */

/* front face counter-clockwise */

/* for GL_LINE_LOOP */
//#ifdef __COMMENT__
    pObject.ShapeIndices << pComponents.FTRIdx;
    pObject.ShapeIndices << pComponents.FTLIdx;
    pObject.ShapeIndices << pComponents.FLLIdx;
    pObject.ShapeIndices << pComponents.FLRIdx;
    pObject.ShapeIndices << pComponents.FTRIdx;

   /* closing the figure back to FTR point */
//#endif
/* Top face counter-clockwise  starting from wFTRIdx*/

    pObject.ShapeIndices << pComponents.FTRIdx;
    pObject.ShapeIndices << pComponents.BTRIdx;
    pObject.ShapeIndices << pComponents.BTLIdx;
    pObject.ShapeIndices << pComponents.FTLIdx;
    pObject.ShapeIndices << pComponents.FTRIdx;
    /* closing the figure back to FTRpoint */

/* Low Face */
    pObject.ShapeIndices << pComponents.FLRIdx;
    pObject.ShapeIndices << pComponents.FLLIdx;
    pObject.ShapeIndices << pComponents.BLLIdx;
    pObject.ShapeIndices << pComponents.BLRIdx;
    pObject.ShapeIndices << pComponents.FLRIdx;

/* Back Face */
    pObject.ShapeIndices << pComponents.BLRIdx;
    pObject.ShapeIndices << pComponents.BTRIdx;
    pObject.ShapeIndices << pComponents.BTLIdx;
    pObject.ShapeIndices << pComponents.BLLIdx;
    pObject.ShapeIndices << pComponents.BLRIdx;

    pObject.ShapeIndices << pComponents.BTRIdx; /* closing the volume loop */
    pObject.ShapeIndices << pComponents.FTRIdx;

    /* closing the figure back to BTR point */

    /* OK */

    pObject.setDrawFigure(GL_TRIANGLES);

/* end indices */

    return pObject;
}//openboxIndexedSetup

//#ifdef __COMMENT__
//ZMetaObject
ZCandy
generateCandy (const float pHigh,
            const float pWidth,
            const float pDepth,
            const char *pName)
{

ZCandy wCandy(pName);
//char wName[50];
zbs::ZArray<Vertice_type> wMids;
float wRadius= pHigh/2.0f;
ZBoxComponents pComponents;

 //   strcpy (wName,pName);
 //   strcat (wName,"_OpenBox");
    ZObject wOpenBox = openboxSetup(pHigh,pWidth,pDepth,pComponents,"OpenBox");
//    wOpenBox.setDrawFigure(GL_TRIANGLES);
    wCandy.add(wOpenBox);

//    strcpy (wName,pName);
//    strcat (wName,"_ArcFL");
    ZObject wArcFL = generate_Arc(pComponents.FLMid,wRadius,10,ZObject::DirLeft,ZObject::Front,"ArcFL");
//    wArcFL.setDrawFigure(GL_TRIANGLE_FAN);
    wCandy.add(wArcFL);
//    strcpy (wName,pName);
//    strcat (wName,"ArcBL");
    ZObject wArcBL = generate_Arc(pComponents.BLMid,wRadius,10,ZObject::DirLeft,ZObject::Back,"ArcBL");/* generate arc reverse (left) */
//    wArcBL.setDrawFigure(GL_TRIANGLE_FAN);
    wCandy.add(wArcBL);
//    strcpy (wName,pName);
//    strcat (wName,"ArcFR");
    ZObject wArcFR = generate_Arc(pComponents.FRMid,wRadius,10,ZObject::DirRight,ZObject::Front,"ArcFR");/* generate front arc forward (right) */
//    wArcFR.setDrawFigure(GL_TRIANGLE_FAN);
    wCandy.add(wArcFR);
//    strcpy (wName,pName);
//    strcat (wName,"ArcBR");
    ZObject wArcBR = generate_Arc(pComponents.BRMid,wRadius,10,ZObject::DirRight,ZObject::Back,"ArcBR");/* generate bottom arc forward (right) */
//    wArcBR.setDrawFigure(GL_TRIANGLE_FAN);
    wCandy.add(wArcBR);

//    strcpy (wName,pName);
//    strcat (wName,"wArcStripsLeft");
    ZObject wArcStripsLeft = generate_ArcStripsLeft(wArcFL,wArcBL,"ArcStripLeft");
//    wArcStripsLeft.setDrawFigure(GL_TRIANGLES);
    wCandy.add(wArcStripsLeft);
//    strcpy (wName,pName);
//    strcat (wName,"wArcStripsRight");
    ZObject wArcStripsRight = generate_ArcStripsRight(wArcFR,wArcBR,"ArcStripRight");
//    wArcStripsLeft.setDrawFigure(GL_TRIANGLES);
    wCandy.add(wArcStripsRight);

/*=================ZCandy Line Shape vertices setup =====================*/

/* front face */
    wCandy.FrontShape.push_back(pComponents.FTR);
    wCandy.FrontShape.push_back(pComponents.FTL);

    for (long wi=0;wi<wArcFL.vertices.count();wi+=3)
        {
        /* skip first which is center of arc */
        wCandy.FrontShape.push_back(wArcFL.vertices[wi+1].point);
        wCandy.FrontShape.push_back(wArcFL.vertices[wi+2].point);
        }
    wCandy.FrontShape.push_back(pComponents.FLL);
    wCandy.FrontShape.push_back(pComponents.FLR);

    for (long wi=wArcFR.vertices.count()-1;wi>1;wi-=3) /* start by end and skip first which is center of arc */
        {
        wCandy.FrontShape.push_back(wArcFR.vertices[wi].point);
        wCandy.FrontShape.push_back(wArcFR.vertices[wi-1].point);
        }
    wCandy.FrontShape.push_back(pComponents.FTR);

/* backward face */

    wCandy.BackShape.push_back(pComponents.BTR);
    wCandy.BackShape.push_back(pComponents.BTL);
    for (long wi=0;wi<wArcBL.vertices.count();wi+=3)
        {
        /* skip first which is center of arc */
        wCandy.FrontShape.push_back(wArcBL.vertices[wi+1].point);
        wCandy.FrontShape.push_back(wArcBL.vertices[wi+2].point);
        }
    wCandy.BackShape.push_back(pComponents.BLL);
    wCandy.BackShape.push_back(pComponents.BLR);

    for (long wi=wArcBR.vertices.count()-1;wi>1;wi-=3) /* start by end and skip first which is center of arc */
        {
        wCandy.BackShape.push_back(wArcBR.vertices[wi].point);
        wCandy.BackShape.push_back(wArcBR.vertices[wi-1].point);
        }

    wCandy.BackShape.push_back(pComponents.BTR);
    /* OK */

/* end indices */

    return wCandy;
}//Candyboxsetup

//#endif // __COMMENT__


ZObject
generateCylinder(Color_type pColor,
                 Vertice_type pBeginCenter,
                 float pBeginRadius,
                 Vertice_type pTargetCenter,
                 float pTargetRadius,
                 int pNumber,
                 ZObject::CircleMade pBeginCircleState,
                 ZObject::CircleMade pTargetCircleState,
                 const char* pName)
{
    ZObject wBegin= generate_Circle(pColor,pBeginCenter,pBeginRadius,pNumber,pBeginCircleState);
    ZObject wTarget=generate_Circle(pColor,pTargetCenter,pTargetRadius,pNumber,pTargetCircleState);
    ZObject wPipe(pName);
    wPipe.setDefaultColor(pColor);

    wPipe << wBegin.vertices[0].point;
    wPipe << wBegin.vertices[wBegin.vertices.count()-1].point;
    wPipe << wTarget.vertices[0].point;
    wPipe.VNormalDir.push_back(ZObject::Compute);

    wPipe << wTarget.vertices[0].point;
    wPipe << wTarget.vertices[wBegin.vertices.count()-1].point;
    wPipe << wBegin.vertices[wBegin.vertices.count()-1].point;
    wPipe.VNormalDir.push_back(ZObject::Compute);



    long wi=0;

    for (; wi < (wBegin.vertices.count()-1) ;wi++)
    {
        wPipe << wBegin.vertices[wi+1].point;
        wPipe << wBegin.vertices[wi].point;
        wPipe << wTarget.vertices[wi+1].point;
        wPipe.VNormalDir.push_back(ZObject::Compute);

        wPipe << wTarget.vertices[wi].point;
        wPipe << wTarget.vertices[wi+1].point;
        wPipe << wBegin.vertices[wi].point;
        wPipe.VNormalDir.push_back(ZObject::Compute);
    }
    if ((wBegin.verticeCount()<2)||(wTarget.verticeCount()<2))
                return wPipe;

    long wj=wBegin.vertices.count()-1;
    wPipe << wBegin.vertices[0].point;
    wPipe << wBegin.vertices[wj].point;
    wPipe << wTarget.vertices[wj].point;
    wPipe.VNormalDir.push_back(ZObject::Compute);

    wPipe << wTarget.vertices[wj].point;
    wPipe << wTarget.vertices[0].point;
    wPipe << wBegin.vertices[0].point;
    wPipe.VNormalDir.push_back(ZObject::Compute);

    return wPipe;
}//generateRegularPipe

ZObject
generateRegularCylinder(Color_type pColor,
                        Vertice_type pBeginCenter,
                        Vertice_type pTargetCenter,
                        float pRadius,
                        int pNumber,
                        ZObject::CircleMade pCircleState,
                        const char *pName)
{
    return generateCylinder(pColor,
                            pBeginCenter,
                            pRadius,
                            pTargetCenter,
                            pRadius,
                            pNumber,
                            pCircleState,
                            pCircleState,
                            pName);

}

ZObject
generateDirectionCylinder(Color_type pColor,
                          Vertice_type pBeginCenter,
                          Vertice_type pDirection,
                          float pRadius,
                          int pNumber,
                          ZObject::CircleMade pCircleState,
                          const char*pName=nullptr)
{
    Vertice_type wTargetCenter = pBeginCenter + pDirection;
    return generateCylinder(pColor,
                            pBeginCenter,
                            pRadius,
                            wTargetCenter,
                            pRadius,
                            pNumber,
                            pCircleState,
                            pCircleState,
                            pName);

}

ZObject
generateSphere(Color_type pColor,
               Vertice_type pBeginCenter,
               float pRadius,
               int pNumber,
               const char*pName=nullptr)
{
   ZObject wSphere(pName);

}

