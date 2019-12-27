#include <zobjectfunctions.h>
#include <stb_image.h>
#include <zglresource.h>
#include <zboxcomponents.h>
#include <zcandy.h>

/* compute a triangle's surface normal vec3 vector */

//Vertice_type CalculateSurfaceNormal (Vertice_type* pTriangle, ZObject::NormalDirection pNormDir)
glm::vec3 calculateSurfaceNormal (Vertice_type* pTriangle)
{

    Vertice_type wNormal= glm::normalize(glm::cross(pTriangle[2] - pTriangle[0], pTriangle[1] - pTriangle[0]));

    return wNormal;

}//CalculateSurfaceNormal

const glm::vec3 calculateCenter(glm::vec3* pTriangle)
{
    Vertice_type wCenter;

    wCenter.x=(pTriangle[0].x+pTriangle[1].x+pTriangle[2].x)/3.0f;
    wCenter.y=(pTriangle[0].y+pTriangle[1].y+pTriangle[2].y)/3.0f;
    wCenter.z=(pTriangle[0].z+pTriangle[1].z+pTriangle[2].z)/3.0f;
    return wCenter;
}//calculateCenter


/*--------------------------------------------
 *                  PI/2
 *
 *
 *      PI                          0-2*PI
 *
 *
 *
 *
 *                  3PI/2
 * ---------------------------------------------
 */


/*  PI/2 --> 0
 *
 *           2*PI ---> 3*PI/2
 */
zbs::ZArray<Vertice_type> perfect_arc_right_Front(Vertice_type pCenter, double pRadiusLen,  int pNumber)
{

//#define NUMBER_OF_VERTICES 16

double wRadius = pRadiusLen;
zbs::ZArray<Vertice_type> vertexBuffer;


Vertice_type wPoint;

/* right Front from  3*PI/2  -> (2*PI) 0 -> PI/2
*/

    vertexBuffer.push_back( pCenter);

double wLimit = (double)M_PI;

double wT=3.0*(double)M_PI/2.0;
double wIncrement =  (double)M_PI /(double)pNumber ;

    wLimit = 2.0*(double)M_PI;

    while (wT <= wLimit)
    {
        wPoint.x=pCenter.x + (cos(wT) * wRadius);
        wPoint.y=pCenter.y + (sin(wT) * wRadius);
        wPoint.z=pCenter.z;
        vertexBuffer.push_back( wPoint);
        wT+=wIncrement;
    }

    wT=0.0;
    wLimit = double(M_PI)/2.0 ;
    while (wT <= wLimit)
    {
        wPoint.x=pCenter.x + (cos(wT) * wRadius);
        wPoint.y=pCenter.y + (sin(wT) * wRadius);
        wPoint.z=pCenter.z;
        vertexBuffer.push_back( wPoint);
        wT+=wIncrement;
    }

/* left from PI / 2  (top- 90°) -> PI ->  to 3*PI / 2   (bottom - 270 °) */
    return vertexBuffer;
}//perfect_arc_right_Front

zbs::ZArray<Vertice_type> perfect_arc_right_Back(Vertice_type pCenter, double pRadiusLen,  int pNumber)
{
/* right Back from  PI/2  -> 0 -> 3*PI/2
*/

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
    while (wT>=0)
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

    return vertexBuffer;
}//perfect_arc_right_Back


zbs::ZArray<Vertice_type> perfect_arc_left_Front(Vertice_type pCenter, double pRadiusLen,  int pNumber)
{

//#define NUMBER_OF_VERTICES 16

double wRadius = pRadiusLen;
zbs::ZArray<Vertice_type> vertexBuffer;


Vertice_type wPoint;
double wIncrement =  (double)M_PI /(double)pNumber ;
//double wLimit = - 2.0 * (double)M_PI;

/* left from PI / 2  -> PI ->  to 3*PI / 2  */

    vertexBuffer.push_back(pCenter); /* center is always first element */

    double wT= double(M_PI)/2.0;
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
}//perfect_arc_left_Front

zbs::ZArray<Vertice_type> perfect_arc_left_Back(Vertice_type pCenter,double pRadiusLen,  int pNumber)
{

//#define NUMBER_OF_VERTICES 16

double wRadius = double(pRadiusLen);
zbs::ZArray<Vertice_type> vertexBuffer;


Vertice_type wPoint;
double wIncrement =  double(M_PI) /double(pNumber) ;
//double wLimit = - 2.0 * (double)M_PI;

/* left from  3*PI / 2   -> PI ->  to  PI / 2 */

    vertexBuffer.push_back(pCenter); /* center is always first element */

    double wT= 3.0*double(M_PI)/2.0;
    double wLimit = double(M_PI) / 2.0 ;


    while (wT >= wLimit)
    {
        wPoint.x=pCenter.x+(cos(wT) * wRadius);
        wPoint.y=pCenter.y+(sin(wT) * wRadius);
        wPoint.z=pCenter.z;
        vertexBuffer.push_back( wPoint);
        wT-= wIncrement;
    }

    return vertexBuffer;
}//perfect_arc_left_Front



zbs::ZArray<glm::vec3> face_circle(glm::vec3 pCenter,double pRadiusLen,  int pNumber)
{

//#define NUMBER_OF_VERTICES 16

double wRadius = pRadiusLen;
zbs::ZArray<Vertice_type> vertexBuffer;

const double wM_PI= (double)M_PI;

Vertice_type wPoint;
double wIncrement =  (2.0*wM_PI) /(double)pNumber ;

    vertexBuffer.push_back(pCenter); /* center is always first element */

    double wT= 0;
    double wLimit = 2.0 *wM_PI;


    while (wT <= wLimit)
    {
        wPoint.x=pCenter.x+(cos(wT) * wRadius);
        wPoint.y=pCenter.y+(sin(wT) * wRadius);
        wPoint.z=pCenter.z;
        vertexBuffer.push_back( wPoint);
        wT+= wIncrement;
    }

    return vertexBuffer;
}//facing_circle
zbs::ZArray<glm::vec3> back_circle(glm::vec3 pCenter,double pRadiusLen,  int pNumber)
{

double wRadius = pRadiusLen;
zbs::ZArray<Vertice_type> vertexBuffer;

const double wM_PI= double(M_PI);

glm::vec3 wPoint;
double wIncrement =  (2.0*wM_PI) /double(pNumber) ;

    vertexBuffer.push_back(pCenter); /* center is always first element */

    double wT= 2.0 *wM_PI;

    while (wT >= 0.0)
    {
        wPoint.x=pCenter.x+(cos(wT) * wRadius);
        wPoint.y=pCenter.y+(sin(wT) * wRadius);
        wPoint.z=pCenter.z;
        vertexBuffer.push_back( wPoint);
        wT-= wIncrement;
    }

    return vertexBuffer;
}//back_circle
zbs::ZArray<Vertice_type> sequent_circle(Vertice_type pCenter,double pRadiusLen,  int pNumber)
{

//#define NUMBER_OF_VERTICES 16

double wRadius = pRadiusLen;
zbs::ZArray<Vertice_type> vertexBuffer;

const double wM_PI= double(M_PI);

glm::vec3 wPoint;
double wIncrement =  (2.0*wM_PI) /double(pNumber) ;
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
}//sequent_circle

zbs::ZArray<Vertice_type> sequent_back_circle(glm::vec3 pCenter,double pRadiusLen,  int pNumber)
{

//#define NUMBER_OF_VERTICES 16

double wRadius = pRadiusLen;
zbs::ZArray<Vertice_type> vertexBuffer;

const double wM_PI= double(M_PI);

glm::vec3 wPoint;
double wIncrement =  (2.0*wM_PI) /double(pNumber) ;
//double wLimit = - 2.0 * (double)M_PI;

/* left from PI / 2  (top- 90°) -> PI ->  to 3*PI / 2   (bottom - 270 °) */

/* rigth from  PI/2 (top-90°) -> 0 -> 3*PI/2 */

    vertexBuffer.push_back(pCenter); /* center is always first element */

    double wT= 2.0 *wM_PI;

    while (wT >= 0.0)
    {
        wPoint.x=pCenter.x;
        wPoint.y=pCenter.y+(sin(wT) * wRadius);
        wPoint.z=pCenter.z+(cos(wT) * wRadius);
        vertexBuffer.push_back( wPoint);
        wT-= wIncrement;
    }

    return vertexBuffer;
}//sequent_back_circle

zbs::ZArray<glm::vec3> flat_back_circle(glm::vec3 pCenter,double pRadiusLen,  int pNumber)
{

//#define NUMBER_OF_VERTICES 16

double wRadius = pRadiusLen;
zbs::ZArray<Vertice_type> vertexBuffer;

const double wM_PI= double(M_PI);

glm::vec3 wPoint;
double wIncrement =  (2.0*wM_PI) /double(pNumber) ;
//double wLimit = - 2.0 * (double)M_PI;

/* left from PI / 2  (top- 90°) -> PI ->  to 3*PI / 2   (bottom - 270 °) */

/* rigth from  PI/2 (top-90°) -> 0 -> 3*PI/2 */

    vertexBuffer.push_back(pCenter); /* center is always first element */

    double wT= 2.0 *wM_PI;

    while (wT >= 0.0)
    {
        wPoint.x=pCenter.x+(cos(wT) * wRadius);
        wPoint.y=pCenter.y;
        wPoint.z=pCenter.z+(sin(wT) * wRadius);
        vertexBuffer.push_back( wPoint);
        wT-= wIncrement;
    }

    return vertexBuffer;
}//flat_back_circle

zbs::ZArray<glm::vec3> flat_circle(glm::vec3 pCenter,double pRadiusLen,  int pNumber)
{

//#define NUMBER_OF_VERTICES 16

double wRadius = pRadiusLen;
zbs::ZArray<Vertice_type> vertexBuffer;

const double wM_PI= (double)M_PI;

glm::vec3 wPoint;
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


ZObject* generate_Arc(Vertice_type pCenter,
                     float pRadiusLen,
                     int pNumber,
                     ZObject::Direction pDirection,
                     NormalDirection pNormDir,
                     bool pGenerateShape,
                     const char*pName)
{

ZObject* wArc=new ZObject(pName,ZObject::Arc);
zbs::ZArray<Vertice_type> vertexBuffer;

    wArc->createVertexOnly(Draw);

    if (pGenerateShape)
            {
            wArc->createVertexAndIndex(Shape);
            wArc->setDrawFigure(Shape,GL_LINE_LOOP);
            }
    if (pDirection<0)
        {
//     pCenter.x -= 0.25 ;
        vertexBuffer = perfect_arc_left_Front(pCenter,pRadiusLen,pNumber);
        }
    else
        {
//    pCenter.x += 0.25 ;
        vertexBuffer = perfect_arc_right_Front(pCenter,pRadiusLen,pNumber);
        }
    for (int wi=2;wi<vertexBuffer.size();wi++)
        {
        wArc->addVertex(Draw,vertexBuffer[0]);
        wArc->addVertex(Draw,vertexBuffer[wi-1]);
        wArc->addVertex(Draw,vertexBuffer[wi]);
        wArc->addNormalDir(Draw, pNormDir); /* one direction per triangle */
        if (pGenerateShape)
                {
                wArc->addVertex(Shape,vertexBuffer[0]);     /* copy again vertex data to Shape context */
                wArc->addVertex(Shape,vertexBuffer[wi-1]);  /* NB has to be optimized with a Vertex array = nullptr for Shape */
                wArc->addVertex(Shape,vertexBuffer[wi]);

                wArc->addIndice(Shape,wArc->lastVertexIdx()-1);
                wArc->addIndice(Shape,wArc->lastVertexIdx());
//                wArc->ShapeIndices << (GLuint)wArc->lastVertexIdx()-1;
//                wArc->ShapeIndices << (GLuint)wArc->lastVertexIdx();
                }
        }
    wArc->setDrawFigure(Draw,GL_TRIANGLE_FAN);

    return wArc;
}//generate_Arc

ZObject* generate_ArcFrontBack(Vertice_type pCenter,
                               float pRadiusLen,
                               int pNumber,
                               int pFrontBack,          /* == 1 Front ; == 0 Back */
                               int pLeftRight,          /* == 1 Left ; == 0 Right */
                               bool pGenerateShape,
                               const char*pName)
{

ZObject* wArc=new ZObject(pName,ZObject::Arc);
zbs::ZArray<Vertice_type> vertexBuffer;

    wArc->createVertexOnly(Draw);

    if (pGenerateShape)
            {
            wArc->createVertexAndIndex(Shape);
            wArc->setDrawFigure(Shape,GL_LINE_LOOP);
            }
    if (pFrontBack) /* == 1 Front ; == 0 Back */
        {
//     pCenter.x -= 0.25 ;
        if (pLeftRight) /* == 1 Left ; == 0 Right */
            vertexBuffer = perfect_arc_left_Front(pCenter,pRadiusLen,pNumber);
        else
            vertexBuffer = perfect_arc_right_Front(pCenter,pRadiusLen,pNumber);
        }
    else
        {
        if (pLeftRight)
            vertexBuffer = perfect_arc_left_Back(pCenter,pRadiusLen,pNumber);
        else
            vertexBuffer = perfect_arc_right_Back(pCenter,pRadiusLen,pNumber);
        }

    for (int wi=2;wi<vertexBuffer.size();wi++)
        {
        wArc->addVertex(Draw,vertexBuffer[0]);
        wArc->addVertex(Draw,vertexBuffer[wi-1]);
        wArc->addVertex(Draw,vertexBuffer[wi]);

        if (pGenerateShape)
                {
                wArc->addVertex(Shape,vertexBuffer[0]);     /* copy again vertex data to Shape context */
                wArc->addVertex(Shape,vertexBuffer[wi-1]);  /* NB has to be optimized with a Vertex array = nullptr for Shape */
                wArc->addVertex(Shape,vertexBuffer[wi]);

                wArc->addIndice(Shape,wArc->lastVertexIdx()-1);
                wArc->addIndice(Shape,wArc->lastVertexIdx());
//                wArc->ShapeIndices << (GLuint)wArc->lastVertexIdx()-1;
//                wArc->ShapeIndices << (GLuint)wArc->lastVertexIdx();
                }
        }
    wArc->setDrawFigure(Draw,GL_TRIANGLE_FAN);

    return wArc;
}//generate_Arc

ZObject*
generate_ArcStripsRight(ZObject &pArcFront,
                        ZObject &pArcBack,
                        const char *pName)
{
ZObject* wArcStrips=new ZObject(pName,ZObject::ArcStrip);

    wArcStrips->createVertexOnly(Draw);

    /* Nota Bene : no ShapeIndices for this object */

        /* start skipping arc center (first) */
        for (long wi=1; (wi < pArcFront.verticeCount())&&(wi<pArcBack.verticeCount());wi+=2) /* each 2 and skip arc center*/
            {
            /* first triangle counter-clockwise*/
            wArcStrips->addVertex(Draw,pArcFront[wi+1]);
            wArcStrips->addVertex(Draw,pArcFront[wi]);
            wArcStrips->addVertex(Draw,pArcBack[wi]);

/* Compute normals is the default when VNormalDir does not exist */
//            wArcStrips->VNormalDir.push_back( ZObject::Compute); /* one normal direction per triangle */

            /* second adjacent triangle counter-clockwise*/
            wArcStrips->addVertex(Draw,pArcBack[wi]);
            wArcStrips->addVertex(Draw,pArcBack[wi+1]);
            wArcStrips->addVertex(Draw,pArcFront[wi+1]);


//            wArcStrips->VNormalDir.push_back( ZObject::Compute); /* one normal direction per triangle */
            }
    wArcStrips->setDrawFigure(Draw,GL_TRIANGLES);
    return wArcStrips;
}//generate_ArcStrips


ZObject* generate_ArcStripsLeft(ZObject &pArcFront,
                               ZObject &pArcBack,
                               const char *pName)
{
ZObject* wArcStrips=new ZObject(pName,ZObject::ArcStrip);


    wArcStrips->createVertexOnly(Draw);

    /* start skipping arc center (first) */
    for (long wi=1; (wi < pArcFront.verticeCount())&&(wi<pArcBack.verticeCount());wi+=2) /* each 2 and skip arc center*/
        {
        /* first triangle counter-clockwise*/
        wArcStrips->addVertex(Draw,pArcFront[wi]);
        wArcStrips->addVertex(Draw,pArcBack[wi]);
        wArcStrips->addVertex(Draw,pArcBack[wi+1]);


        /* compute normal dir is default option when VNormalDir does not exist for the context */
//        wArcStrips->VNormalDir.push_back( ZObject::Compute); /* one normal direction per triangle */

        /* second adjacent triangle counter-clockwise*/
        wArcStrips->addVertex(Draw,pArcBack[wi+1]);
        wArcStrips->addVertex(Draw,pArcFront[wi+1]);
        wArcStrips->addVertex(Draw,pArcFront[wi]);

//        wArcStrips->VNormalDir.push_back( ZObject::Compute); /* one normal direction per triangle */
        }//for

    wArcStrips->setDrawFigure(Draw,GL_TRIANGLES);

    return wArcStrips;
}//generate_ArcStripsLeft


ZObject* generate_Circle (Vertice_type pCenter,
                         float pRadiusLen,
                         int pNumber,
                         ZObject::CircleMade pCircleDir,
                         NormalDirection pNormDir,
                         const char*pName)
{
ZObject* wCircle=new ZObject(pName,ZObject::Circle);
zbs::ZArray<Vertice_type> vertexBuffer;


    wCircle->createVertexOnly();

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
        wCircle->addVertex(Draw,vertexBuffer[0]);
        wCircle->addVertex(Draw,vertexBuffer[wi-1]);
        wCircle->addVertex(Draw,vertexBuffer[wi]);
        wCircle->addNormalDir(Draw, pNormDir);/* one normal direction per triangle */

        }
    wCircle->setDrawFigure(Draw,GL_TRIANGLE_FAN);
    return wCircle;
}//generate_Circle

ZObject* boxIndexSetup (const float pHigh,
                       const float pWidth,
                       const float pDepth,
                       const char* pName)
{
    ZBoxComponents pBoxComponents;
    return boxIndexSetup(pHigh,pWidth,pDepth,pBoxComponents,pName);
}


ZObject* boxIndexSetup (const float pHigh,
                       const float pWidth,
                       const float pDepth,
                       ZBoxComponents& pBoxComponents,
                       const char* pName)
{
ZObject* pObject=new ZObject(pName,ZObject::Box);


    pBoxComponents.setup(pHigh,pWidth,pDepth);

    /* clock wise */

    pObject->createVertexAndIndex(Draw);

    /* front face clock wise */
    pObject->addVec3(Draw,pBoxComponents.FTL,"FTL");

/*    *pObject << pBoxComponents.FTL ;
    pObject->VName.push_back("FTL");
    */
    unsigned int wFTLIdx= (unsigned int)pObject->lastVertexIdx();

    pObject->addVec3(Draw,pBoxComponents.FTR,"FTR");
/*    *pObject << pBoxComponents.FTR ;
    pObject->VName.push_back("FTR");*/
    unsigned int wFTRIdx=(unsigned int) pObject->lastVertexIdx();

    pObject->addVec3(Draw,pBoxComponents.FLL,"FLL");
 /*   *pObject << pBoxComponents.FLL ;
    pObject->VName.push_back("FLL");*/
    unsigned int wFLLIdx= (unsigned int)pObject->lastVertexIdx();

    pObject->addVec3(Draw,pBoxComponents.FLR,"FLR");
/*    *pObject << pBoxComponents.FLR ;
    pObject->VName.push_back("FLR");*/
    unsigned int wFLRIdx= (unsigned int)pObject->lastVertexIdx();

    /* back face clock wise */
    pObject->addVec3(Draw,pBoxComponents.BTL,"BTL");
/*    *pObject << pBoxComponents.BTL ;
    pObject->VName.push_back("BTL");*/
    unsigned int wBTLIdx= (unsigned int)pObject->lastVertexIdx();

     pObject->addVec3(Draw,pBoxComponents.BTR,"BTR");
/*    *pObject << pBoxComponents.BTR ;
    pObject->VName.push_back("BTR");*/
    unsigned int wBTRIdx= (unsigned int)pObject->lastVertexIdx();

    pObject->addVec3(Draw,pBoxComponents.BLL,"BLL");
    /*
    *pObject << pBoxComponents.BLL ;
    pObject->VName.push_back("BLL");*/
    GLuint wBLLIdx= (GLuint)pObject->lastVertexIdx();

    pObject->addVec3(Draw,pBoxComponents.BLR,"BLR");
/*    *pObject << pBoxComponents.BLR ;
    pObject->VName.push_back("BLR");*/
    unsigned int wBLRIdx= (unsigned int)pObject->lastVertexIdx();

    /* indices */
    /* front face */
    pObject->addIndice( Draw,wFLRIdx);
    pObject->addIndice( Draw,wFTRIdx);
    pObject->addIndice( Draw,wFTLIdx);


    pObject->addNormalDir(Draw, Front);

    pObject->addIndice( Draw,wFTLIdx);
    pObject->addIndice( Draw,wFLLIdx);
    pObject->addIndice( Draw,wFLRIdx);

    pObject->addNormalDir(Draw, Front);

    /* Backward face  */
    pObject->addIndice(Draw,wBLRIdx);
    pObject->addIndice(Draw,wBTRIdx);
    pObject->addIndice(Draw,wBTLIdx);

    pObject->addNormalDir(Draw,Back);

    pObject->addIndice(Draw,wBTLIdx);
    pObject->addIndice(Draw,wBLLIdx);
    pObject->addIndice(Draw,wBLRIdx);

    pObject->addNormalDir(Draw,Back);

    /* Down face  */
    pObject->addIndice(Draw,wFLRIdx);
    pObject->addIndice(Draw,wBLRIdx);
    pObject->addIndice(Draw,wBLLIdx);

    pObject->addNormalDir(Draw, Bottom);

    pObject->addIndice(Draw,wBLLIdx);
    pObject->addIndice(Draw,wFLLIdx);
    pObject->addIndice(Draw,wFLRIdx);

    pObject->addNormalDir(Draw,Bottom);

    /* Top face  */
    pObject->addIndice(Draw,wFTRIdx);
    pObject->addIndice(Draw,wBTRIdx);
    pObject->addIndice(Draw,wBTLIdx);

    pObject->addNormalDir(Draw,Top);

    pObject->addIndice(Draw,wBTLIdx);
    pObject->addIndice(Draw,wFTLIdx);
    pObject->addIndice(Draw,wFTRIdx);

    pObject->addNormalDir(Draw,Top);

    /* Left face  */

    pObject->addIndice(Draw,wFTLIdx);
    pObject->addIndice(Draw,wBTLIdx);
    pObject->addIndice(Draw,wBLLIdx);

    pObject->addNormalDir(Draw,Left);

    pObject->addIndice(Draw,wBLLIdx);
    pObject->addIndice(Draw,wFLLIdx);
    pObject->addIndice(Draw,wFTLIdx);

    pObject->addNormalDir(Draw,Left);

    /* Right face  */

    pObject->addIndice(Draw,wFTRIdx);
    pObject->addIndice(Draw,wBTRIdx);
    pObject->addIndice(Draw,wBLRIdx);

    pObject->addNormalDir(Draw,Right);

    pObject->addIndice(Draw,wBLRIdx);
    pObject->addIndice(Draw,wFLRIdx);
    pObject->addIndice(Draw,wFTRIdx);

    pObject->addNormalDir(Draw,Right);

    /* OK */

/* end indices */
    pObject->setDrawFigure(Draw,GL_TRIANGLES);
    return pObject;
}//boxIndexSetup
#ifdef __COMMENT__
ZObject* boxSetup_old (const float pHigh,
                  const float pWidth,
                  const float pDepth,
                  ZBoxComponents& pComponents,
                  const char*pName)
{
ZObject* pObject=new ZObject(pName,ZObject::Box);
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
    pObject->addVec3( wFTL);
    pObject->ShapeIndices << (GLuint)pObject->lastVertexIdx();
    pObject->addVec3( wFTR);
    pObject->ShapeIndices << (GLuint)pObject->lastVertexIdx();
    pObject->addVec3( wFLR);
    pObject->ShapeIndices << (GLuint)pObject->lastVertexIdx();

    pObject->addVec3( wFLR); /* skip it for shape line drawing index */
    pObject->addVec3( wFLL);
    pObject->ShapeIndices << (GLuint)pObject->lastVertexIdx();
    pObject->addVec3( wFTL);
    pObject->ShapeIndices << (GLuint)pObject->lastVertexIdx();

    pObject->VNormalDir.push_back( ZObject::Front);pObject->ShapeIndices << (GLuint)pObject->lastVertexIdx();

    pObject->VNormalDir.push_back( ZObject::Front);

    /* Backward face  */
    pObject->addVec3(wBTL);
    pObject->ShapeIndices << (GLuint)pObject->lastVertexIdx();
    pObject->addVec3(wBTR);
    pObject->ShapeIndices << (GLuint)pObject->lastVertexIdx();
    pObject->addVec3(wBLR);
    pObject->ShapeIndices << (GLuint)pObject->lastVertexIdx();

    pObject->addVec3(wBLR);/* skip it for shape line drawing index */
    pObject->addVec3(wBLL);
    pObject->ShapeIndices << (GLuint)pObject->lastVertexIdx();
    pObject->addVec3(wBTL);
    pObject->ShapeIndices << (GLuint)pObject->lastVertexIdx();

    pObject->VNormalDir.push_back( ZObject::Back);

    pObject->VNormalDir.push_back( ZObject::Back);

    /* Down face  */
    pObject->addVec3(wBLL);
    pObject->ShapeIndices << (GLuint)pObject->lastVertexIdx();
    pObject->addVec3(wBLR);
    pObject->ShapeIndices << (GLuint)pObject->lastVertexIdx();
    pObject->addVec3(wFLR);

    pObject->addVec3(wFLR);
    pObject->ShapeIndices << (GLuint)pObject->lastVertexIdx();
    pObject->addVec3(wFLL);
    pObject->ShapeIndices << (GLuint)pObject->lastVertexIdx();
    pObject->addVec3(wBLL);

    pObject->VNormalDir.push_back( ZObject::Bottom);

    pObject->VNormalDir.push_back( ZObject::Bottom);

    /* Top face  */
    pObject->addVec3(wBTL);
    pObject->ShapeIndices << (GLuint)pObject->lastVertexIdx();
    pObject->addVec3(wBTR);
    pObject->ShapeIndices << (GLuint)pObject->lastVertexIdx();
    pObject->addVec3(wFTR);

    pObject->addVec3(wFTR);
    pObject->ShapeIndices << (GLuint)pObject->lastVertexIdx();
    pObject->addVec3(wFTL);
    pObject->ShapeIndices << (GLuint)pObject->lastVertexIdx();
    pObject->addVec3(wBTL);

    pObject->VNormalDir.push_back( ZObject::Top);

    pObject->VNormalDir.push_back( ZObject::Top);

    /* Left face  */

    pObject->addVec3(wFTL);
    pObject->addVec3(wBTL);
    pObject->addVec3(wBLL);

    pObject->addVec3(wBLL);
    pObject->addVec3(wFLL);
    pObject->addVec3(wFTL);

    pObject->VNormalDir.push_back( ZObject::Left);

    pObject->VNormalDir.push_back( ZObject::Left);;

    /* Right face  */

    pObject->addVec3(wFTR);
    pObject->addVec3(wBTR);
    pObject->addVec3(wBLR);

    pObject->addVec3(wBLR);
    pObject->addVec3(wFLR);
    pObject->addVec3(wFTR);

    pObject->VNormalDir.push_back( ZObject::Right);

    pObject->VNormalDir.push_back( ZObject::Right);


    /* OK */
    pObject->setDrawFigure(GL_TRIANGLES);
/* end indices */

    return pObject;
}//boxSetup_old
#endif // __COMMENT__

ZObject* boxSetup (const float pHigh,
                       const float pWidth,
                       const float pDepth,
                       const char* pName)
{
    ZBoxComponents pBoxComponents;
    return boxSetup(pHigh,pWidth,pDepth,pBoxComponents,pName);
}

ZObject* boxSetup (const float pHigh,
                  const float pWidth,
                  const float pDepth,
                  ZBoxComponents& pComponents,
                  const char*pName)
{
ZObject* pObject=new ZObject(pName,ZObject::Box);
/* remark : coords must remain positive -> to be addressed */


    pObject->createVertexAndIndex();

    pComponents.setup (pHigh,pWidth,pDepth);

    /* front face counter-clockwise */
    pObject->addVec3( Draw,pComponents.FLR,"FLR");
    pComponents.FLRIdx= (GLuint)pObject->lastVertexIdx();

    pObject->addVec3( Draw,pComponents.FTR,"FTR");
    pComponents.FTRIdx= (GLuint)pObject->lastVertexIdx();

    pObject->addVec3( Draw,pComponents.FTL,"FTL");
    pComponents.FTLIdx= (GLuint)pObject->lastVertexIdx();

    pObject->addNormalDir(Draw, Front);

    pObject->addVec3(Draw, pComponents.FTL,"FTL");
    pObject->addVec3(Draw, pComponents.FLL,"FLL");
    pComponents.FLLIdx= (GLuint)pObject->lastVertexIdx();
    pObject->addVec3( Draw,pComponents.FLR,"FLR");


    pObject->addNormalDir(Draw, Front);


    /* Backward face  counter-clockwise*/

    pObject->addVec3(Draw,pComponents.BLR,"BLR");
    pComponents.BLRIdx= (GLuint)pObject->lastVertexIdx();
    pObject->addVec3(Draw,pComponents.BTR,"BTR");
    pComponents.BTRIdx= (GLuint)pObject->lastVertexIdx();
    pObject->addVec3(Draw,pComponents.BTL,"BTL");
    pComponents.BTLIdx= (GLuint)pObject->lastVertexIdx();

    pObject->addNormalDir(Draw,Back); /* one per triangle */

    pObject->addVec3(Draw,pComponents.BTL,"BTL");/* skip it for shape line drawing index */
    pObject->addVec3(Draw,pComponents.BLL,"BLL");
    pComponents.BLLIdx= (GLuint)pObject->lastVertexIdx();
    pObject->addVec3(Draw,pComponents.BLR,"BLR");


    pObject->addNormalDir(Draw,Back); /* one per triangle */

    /* Down face  - Low face counter-clockwise*/

    pObject->addVec3(Draw,pComponents.FLR,"FLR");
    pObject->addVec3(Draw,pComponents.BLR,"BLR");
    pObject->addVec3(Draw,pComponents.BLL,"BLL");

    pObject->addNormalDir(Draw,Bottom);/* one per triangle */

    pObject->addVec3(Draw,pComponents.BLL,"BLL");
    pObject->addVec3(Draw,pComponents.FLL,"FLL");
    pObject->addVec3(Draw,pComponents.FLR,"FLR");

    pObject->addNormalDir(Draw,Bottom);/* one per triangle */

    /* Top face  counter-clockwise */

    pObject->addVec3(Draw,pComponents.FTR,"FTR");
    pObject->addVec3(Draw,pComponents.BTR,"BTR");
    pObject->addVec3(Draw,pComponents.BTL,"BTL");

    pObject->addNormalDir(Draw,Top);

    pObject->addVec3(Draw,pComponents.BTL,"BTL");
    pObject->addVec3(Draw,pComponents.FTL,"FTL");
    pObject->addVec3(Draw,pComponents.FTR,"FTR");

    pObject->addNormalDir(Draw,Top);

    /* Left face  */
    pObject->addVec3(Draw,pComponents.FTL,"FTL");
    pObject->addVec3(Draw,pComponents.BTL,"BTL");
    pObject->addVec3(Draw,pComponents.BLL,"BLL");

    pObject->addNormalDir(Draw,Left);

    pObject->addVec3(Draw,pComponents.BLL,"BTL");
    pObject->addVec3(Draw,pComponents.FLL,"FLL");
    pObject->addVec3(Draw,pComponents.FTL,"FTL");

    pObject->addNormalDir(Draw,Left);

    /* Right face  */
    pObject->addVec3(Draw,pComponents.FTR,"FTR");
    pObject->addVec3(Draw,pComponents.BTR,"BTR");
    pObject->addVec3(Draw,pComponents.BLR,"BLR");

    pObject->addNormalDir(Draw,Right);

    pObject->addVec3(Draw,pComponents.BLR,"BLR");
    pObject->addVec3(Draw,pComponents.FLR,"FLR");
    pObject->addVec3(Draw,pComponents.FTR,"FTR");

    pObject->addNormalDir(Draw,Right);


/* shape lines */

/* front face counter-clockwise */

/* for GL_LINE_LOOP */
#ifdef __COMMENT__
    if (pGenerateShape)
    {
    pObject->ShapeIndices << pComponents.FTRIdx;
    pObject->ShapeIndices << pComponents.FTLIdx;
    pObject->ShapeIndices << pComponents.FLLIdx;
    pObject->ShapeIndices << pComponents.FLRIdx;
    pObject->ShapeIndices << pComponents.FTRIdx;

   /* closing the figure back to FTR point */
//#endif
/* Top face counter-clockwise  starting from wFTRIdx*/

 //   pObject->ShapeIndices << pComponents.FTRIdx;
    pObject->ShapeIndices << pComponents.BTRIdx;
    pObject->ShapeIndices << pComponents.BTLIdx;
    pObject->ShapeIndices << pComponents.FTLIdx;
    pObject->ShapeIndices << pComponents.FTRIdx;
    /* closing the figure back to FTRpoint */

/* Low Face */
    pObject->ShapeIndices << pComponents.FLRIdx;
    pObject->ShapeIndices << pComponents.FLLIdx;
    pObject->ShapeIndices << pComponents.BLLIdx;
    pObject->ShapeIndices << pComponents.BLRIdx;
    pObject->ShapeIndices << pComponents.FLRIdx;

/* Back Face */
    pObject->ShapeIndices << pComponents.BLRIdx;
    pObject->ShapeIndices << pComponents.BTRIdx;
    pObject->ShapeIndices << pComponents.BTLIdx;
    pObject->ShapeIndices << pComponents.BLLIdx;
    pObject->ShapeIndices << pComponents.BLRIdx;

    pObject->ShapeIndices << pComponents.BTRIdx; /* closing the volume loop */
    pObject->ShapeIndices << pComponents.FTRIdx;

    /* closing the figure back to BTR point */
    }// pGenerateShape
#endif //__COMMENT__
    /* OK */

    pObject->setDrawFigure(Draw,GL_TRIANGLES);

    return pObject;
}//boxSetup
/* remark: no shape is generated by this function */
ZObject* openboxSetup (const float pHigh,
                       const float pWidth,
                       const float pDepth,
                       const char* pName)
{
    ZBoxComponents pBoxComponents;
    return openboxSetup(pHigh,pWidth,pDepth,pBoxComponents,pName);
}




ZObject* openboxSetup (const float pHigh,
                       const float pWidth,
                       const float pDepth,
                      ZBoxComponents& pComponents,
                      const char *pName)
{

ZObject* pObject=new ZObject(pName,ZObject::Openbox);


    pObject->createVertexAndIndex();
//    ZBoxComponents pComponents(pHigh,pWidth,pDepth);

    pComponents.setup(pHigh,pWidth,pDepth);
/* remark : coords must remain positive -> to be addressed */


    /* front face counter-clockwise */
    pObject->addVec3(Draw, pComponents.FLR,"FLR");
    pComponents.FLRIdx= (GLuint)pObject->lastVertexIdx();

    pObject->addVec3( Draw,pComponents.FTR,"FTR");
    pComponents.FTRIdx= (GLuint)pObject->lastVertexIdx();

    pObject->addVec3( Draw,pComponents.FTL,"FTL");
    pComponents.FTLIdx= (GLuint)pObject->lastVertexIdx();

//    pObject->addNormalDir(Draw,Front);


    pObject->addVec3(Draw, pComponents.FTL,"FTL");
    pObject->addVec3( Draw,pComponents.FLL,"FLL");
    pComponents.FLLIdx= (GLuint)pObject->lastVertexIdx();
    pObject->addVec3(Draw, pComponents.FLR,"FLR");


//    pObject->addNormalDir( Draw,Front);


    /* Backward face  counter-clockwise*/

    pObject->addVec3(Draw,pComponents.BLR,"BLR");
    pComponents.BLRIdx= (GLuint)pObject->lastVertexIdx();
    pObject->addVec3(Draw,pComponents.BTR,"BTR");
    pComponents.BTRIdx= (GLuint)pObject->lastVertexIdx();
    pObject->addVec3(Draw,pComponents.BTL,"BTL");
    pComponents.BTLIdx= (GLuint)pObject->lastVertexIdx();

//    pObject->addNormalDir(Draw, Back); /* one per triangle */

    pObject->addVec3(Draw,pComponents.BTL,"BTL");/* skip it for shape line drawing index */
    pObject->addVec3(Draw,pComponents.BLL,"BLL");
    pComponents.BLLIdx= (GLuint)pObject->lastVertexIdx();
    pObject->addVec3(Draw,pComponents.BLR,"BLR");


//    pObject->addNormalDir(Draw,Back); /* one per triangle */
#ifdef __COMMENT__
    /* Down face  - Low face counter-clockwise */

    pObject->addVec3(Draw,pComponents.FLR,"FLR");
    pObject->addVec3(Draw,pComponents.BLR,"BLR");
    pObject->addVec3(Draw,pComponents.BLL,"BLL");

//    pObject->addNormalDir(Draw,Bottom);/* one per triangle */

    pObject->addVec3(Draw,pComponents.BLL,"BLL");
    pObject->addVec3(Draw,pComponents.FLL,"FLL");
    pObject->addVec3(Draw,pComponents.FLR,"FLR");

//    pObject->addNormalDir(Draw,Bottom);/* one per triangle */
#endif // __COMMENT__

    /* Down face  - Low face clockwise :this face must be clockwise */

    pObject->addVec3(Draw,pComponents.BLL,"BLL");
    pObject->addVec3(Draw,pComponents.BLR,"BLR");
    pObject->addVec3(Draw,pComponents.FLR,"FLR");

//    pObject->addNormalDir(Draw,Bottom);/* one per triangle */

    pObject->addVec3(Draw,pComponents.FLR,"FLR");
    pObject->addVec3(Draw,pComponents.FLL,"FLL");
    pObject->addVec3(Draw,pComponents.BLL,"BLL");

//    pObject->addNormalDir(Draw,Bottom);/* one per triangle */

    /* Top face  counter-clockwise */

    pObject->addVec3(Draw,pComponents.FTR,"FTR");
    pObject->addVec3(Draw,pComponents.BTR,"BTR");
    pObject->addVec3(Draw,pComponents.BTL,"BTL");

//    pObject->addNormalDir(Draw,Top);

    pObject->addVec3(Draw,pComponents.BTL,"BTL");
    pObject->addVec3(Draw,pComponents.FTL,"FTL");
    pObject->addVec3(Draw,pComponents.FTR,"FTR");

    if (pObject->GLDesc[Draw]->VNormalDir)
                delete pObject->GLDesc[Draw]->VNormalDir;

//    pObject->addNormalDir(Draw,Top);

// compute normal and texture coordinates

    pObject->computeNormals(pObject->GLDesc[Draw]->VertexData,nullptr);
    pObject->computeTexCoords(pObject->GLDesc[Draw]->VertexData);

    /* no right no Left face  */
#ifdef __COMMENT__
/* shape lines */

/* front face counter-clockwise */

/* for GL_LINE_LOOP */
//#ifdef __COMMENT__
    pObject->ShapeIndices << pComponents.FTRIdx;
    pObject->ShapeIndices << pComponents.FTLIdx;
    pObject->ShapeIndices << pComponents.FLLIdx;
    pObject->ShapeIndices << pComponents.FLRIdx;
    pObject->ShapeIndices << pComponents.FTRIdx;

   /* closing the figure back to FTR point */
//#endif
/* Top face counter-clockwise  starting from wFTRIdx*/

    pObject->ShapeIndices << pComponents.FTRIdx;
    pObject->ShapeIndices << pComponents.BTRIdx;
    pObject->ShapeIndices << pComponents.BTLIdx;
    pObject->ShapeIndices << pComponents.FTLIdx;
    pObject->ShapeIndices << pComponents.FTRIdx;
    /* closing the figure back to FTRpoint */

/* Low Face */
    pObject->ShapeIndices << pComponents.FLRIdx;
    pObject->ShapeIndices << pComponents.FLLIdx;
    pObject->ShapeIndices << pComponents.BLLIdx;
    pObject->ShapeIndices << pComponents.BLRIdx;
    pObject->ShapeIndices << pComponents.FLRIdx;

/* Back Face */
    pObject->ShapeIndices << pComponents.BLRIdx;
    pObject->ShapeIndices << pComponents.BTRIdx;
    pObject->ShapeIndices << pComponents.BTLIdx;
    pObject->ShapeIndices << pComponents.BLLIdx;
    pObject->ShapeIndices << pComponents.BLRIdx;

    pObject->ShapeIndices << pComponents.BTRIdx; /* closing the volume loop */
    pObject->ShapeIndices << pComponents.FTRIdx;

    /* closing the figure back to BTR point */
#endif // __COMMENT__
    /* OK */

    pObject->setDrawFigure(Draw,GL_TRIANGLES);

/* end indices */

    return pObject;
}//openboxSetup
ZObject* openboxIndexedSetup (const float pHigh,
                             const float pWidth,
                             const float pDepth,
                            ZBoxComponents& pComponents,
                              bool pGenerateShape,
                            const char *pName)
{

ZObject* pObject=new ZObject(pName,ZObject::Openbox);


//    ZBoxComponents pComponents(pHigh,pWidth,pDepth);

    pComponents.setup(pHigh,pWidth,pDepth);
/* remark : coords must remain positive -> to be addressed */

 /*   pObject->addVec3( pComponents.FLR,"FLR");
    GLuint wFLRIdx= (GLuint)pObject->lastVertexIdx();

    pObject->addVec3( pComponents.FTR,"FTR");
    GLuint wFTRIdx= (GLuint)pObject->lastVertexIdx();

    pObject->addVec3( pComponents.FTL,"FTL");
    GLuint wFTLIdx= (GLuint)pObject->lastVertexIdx();

    pObject->addVec3( pComponents.FLL,"FLL");
    GLuint wFLLIdx= (GLuint)pObject->lastVertexIdx();


    pObject->addVec3( pComponents.BLR,"BLR");
    GLuint wBLRIdx= (GLuint)pObject->lastVertexIdx();

    pObject->addVec3( pComponents.BTR,"BTR");
    GLuint wBTRIdx= (GLuint)pObject->lastVertexIdx();

    pObject->addVec3( pComponents.BTL,"BTL");
    GLuint wBTLIdx= (GLuint)pObject->lastVertexIdx();

    pObject->addVec3( pComponents.BLL,"BLL");
    GLuint wBLLIdx= (GLuint)pObject->lastVertexIdx();
*/
    pComponents.setupRawVertices(*pObject);

/* front face counter-clockwise */
    pObject->addIndice(Draw, pComponents.FLRIdx);
    pObject->addIndice(Draw,pComponents.FTRIdx);
    pObject->addIndice(Draw,pComponents.FTLIdx);

    pObject->addNormalDir(Draw,Front);

    pObject->addIndice(Draw,pComponents.FTLIdx);
    pObject->addIndice(Draw,pComponents.FLLIdx);
    pObject->addIndice(Draw,pComponents.FLRIdx);

    pObject->addNormalDir(Draw, Front);

/* Backward face  counter-clockwise*/
    pObject->addIndice(Draw,pComponents.BLRIdx);
    pObject->addIndice(Draw,pComponents.BTRIdx);
    pObject->addIndice(Draw,pComponents.BTLIdx);

    pObject->addNormalDir(Draw,Back);

    pObject->addIndice(Draw, pComponents.BTLIdx);
    pObject->addIndice(Draw,pComponents.BLLIdx);
    pObject->addIndice(Draw,pComponents.BLRIdx);

    pObject->addNormalDir(Draw,Back);/* one per triangle */

 /* Down face  - Low face counter-clockwise*/

    pObject->addIndice(Draw, pComponents.FLRIdx);
    pObject->addIndice(Draw, pComponents.BLRIdx);
    pObject->addIndice(Draw,pComponents.BLLIdx);

    pObject->addNormalDir(Draw,Bottom);

    pObject->addIndice(Draw,pComponents.BLLIdx);
    pObject->addIndice(Draw, pComponents.FLLIdx);
    pObject->addIndice(Draw, pComponents.FLRIdx);

    pObject->addNormalDir(Draw,Bottom);/* one per triangle */


    /* Top face  counter-clockwise */

    pObject->addIndice(Draw,pComponents.FTRIdx);
    pObject->addIndice(Draw, pComponents.BTRIdx);
    pObject->addIndice(Draw, pComponents.BTLIdx);

    pObject->addNormalDir(Draw,Top);

    pObject->addIndice(Draw, pComponents.BTLIdx);
    pObject->addIndice(Draw,pComponents.FTLIdx);
    pObject->addIndice(Draw, pComponents.FTRIdx);

    pObject->addNormalDir(Draw,Top);/* one per triangle */

    /* no right no Left face  */

/* shape lines */

/* front face counter-clockwise */

/* for GL_LINE_LOOP */
//#ifdef __COMMENT__

    if (!pGenerateShape)
                return pObject;
    pObject->createVertexAndIndex(Shape);
    pObject->addIndice(Shape,pComponents.FTRIdx);
    pObject->addIndice(Shape,pComponents.FTLIdx);
    pObject->addIndice(Shape,pComponents.FLLIdx);
    pObject->addIndice(Shape, pComponents.FLRIdx);
    pObject->addIndice(Shape, pComponents.FTRIdx);

   /* closing the figure back to FTR point */
//#endif
/* Top face counter-clockwise  starting from wFTRIdx*/

    pObject->addIndice(Shape, pComponents.FTRIdx);
    pObject->addIndice(Shape, pComponents.BTRIdx);
    pObject->addIndice(Shape, pComponents.BTLIdx);
    pObject->addIndice(Shape,pComponents.FTLIdx);
    pObject->addIndice(Shape,pComponents.FTRIdx);
    /* closing the figure back to FTRpoint */

/* Low Face */
    pObject->addIndice(Shape,pComponents.FLRIdx);
    pObject->addIndice(Shape, pComponents.FLLIdx);
    pObject->addIndice(Shape, pComponents.BLLIdx);
    pObject->addIndice(Shape,pComponents.BLRIdx);
    pObject->addIndice(Shape,pComponents.FLRIdx);

/* Back Face */
    pObject->addIndice(Shape, pComponents.BLRIdx);
    pObject->addIndice(Shape,pComponents.BTRIdx);
    pObject->addIndice(Shape, pComponents.BTLIdx);
    pObject->addIndice(Shape,pComponents.BLLIdx);
    pObject->addIndice(Shape, pComponents.BLRIdx);

    pObject->addIndice(Shape, pComponents.BTRIdx); /* closing the volume loop */
    pObject->addIndice(Shape,pComponents.FTRIdx);

    /* closing the figure back to BTR point */

    /* OK */

    pObject->setDrawFigure(Draw,GL_TRIANGLES);

/* end indices */

    return pObject;
}//openboxIndexedSetup



//#ifdef __COMMENT__
//ZMetaObject
ZCandy
generateCandy (const float pHigh,
               const float pWidth,
               const float pDepth,
               bool pGenerateShape,              /* if true : generate shape indices */
               const char *pName)
{

ZCandy wCandy(pName);
zbs::ZArray<Vertice_type> wMids;
float wRadius= pHigh/2.0f;
ZBoxComponents pComponents;
zbs::ZArray<glm::vec3> wCoords;

    ZObject* wOpenBox = openboxSetup(pHigh,pWidth,pDepth,pComponents,"OpenBox");
    wCandy.add(wOpenBox);

    zbs::ZArray<glm::vec3> wVFL = perfect_arc_left_Front(pComponents.FLMid,wRadius,20);
    ZObject* wArcFL=new ZObject("ArcFL",ZObject::Arc);
    wArcFL->createVertexOnly(Draw);
//    if (pGenerateShape)
//            wArcFL->createVertexOnly(Shape);

    for (int wi=2;wi < wVFL.size();wi++)
        {
        wArcFL->addVertex(Draw,wVFL[0]);
        wArcFL->addVertex(Draw,wVFL[wi-1]);
        wArcFL->addVertex(Draw,wVFL[wi]);

/*        if (pGenerateShape)
                {
                wArcFL->addVertex(Shape,wVFL[wi-1]);
                wArcFL->addVertex(Shape,wVFL[wi]);
                }*/
        }// for

    wArcFL->setDrawFigure(Draw,GL_TRIANGLE_FAN);

/* compute normal and texture coordinates */

    wArcFL->computeNormals(wArcFL->GLDesc[Draw]->VertexData,nullptr);
    wArcFL->computeTexCoords(wArcFL->GLDesc[Draw]->VertexData);

//    wArcFL->setDrawFigure(Shape,GL_LINE_LOOP);

//    ZObject* wArcFL = generate_ArcFrontBack(pComponents.FLMid,
//                                            wRadius,
//                                            20,
//                                            1, /* == 1 Front ; == 0 Back */
//                                            1,/* == 1 Left ; == 0 Right */
//                                            pGenerateShape,
//                                            "ArcFL");
    wCandy.add(wArcFL);

    zbs::ZArray<glm::vec3> wVBL = perfect_arc_left_Back(pComponents.BLMid,wRadius,20);
    ZObject* wArcBL=new ZObject("ArcBL",ZObject::Arc);
    wArcBL->createVertexOnly(Draw);
//    if (pGenerateShape)
//            wArcBL->createVertexOnly(Shape);
    for (int wi=2;wi<wVBL.size();wi++)
        {
        wArcBL->addVertex(Draw,wVBL[0]);
        wArcBL->addVertex(Draw,wVBL[wi]);
        wArcBL->addVertex(Draw,wVBL[wi-1]);

/*        if (pGenerateShape)
                {
                wArcBL->addVertex(Shape,wVBL[wi-1]);
                wArcBL->addVertex(Shape,wVBL[wi]);
                }*/
        }// for

/* compute normal and texture coordinates */

    wArcBL->computeNormals(wArcBL->GLDesc[Draw]->VertexData,nullptr);
    wArcBL->computeTexCoords(wArcBL->GLDesc[Draw]->VertexData);

    wArcBL->setDrawFigure(Draw,GL_TRIANGLE_FAN);
    wCandy.add(wArcBL);


    ZObject* wArcStripLeft=new ZObject("ArcStripLeft",ZObject::ArcStrip);


    wArcStripLeft->createVertexOnly(Draw);

    int wFi=1;
    int wBi=wVBL.count()-1;

    /* start skipping arc center (first) */

    while ((wBi>0)&&(wFi<wVFL.count()))
        {
        /* first triangle counter-clockwise*/
        wArcStripLeft->addVertex(Draw,wVFL[wFi]);
        wArcStripLeft->addVertex(Draw,wVBL[wBi]);
        wArcStripLeft->addVertex(Draw,wVBL[wBi-1]);

        /* second adjacent triangle counter-clockwise*/
        wArcStripLeft->addVertex(Draw,wVBL[wBi]);
        wArcStripLeft->addVertex(Draw,wVFL[wFi]);
        wArcStripLeft->addVertex(Draw,wVFL[wFi-1]);

        wFi++;
        wBi--;
//        wArcStrips->VNormalDir.push_back( ZObject::Compute); /* one normal direction per triangle */
        }//for

    wArcStripLeft->setDrawFigure(Draw,GL_TRIANGLES);

/* compute normal and texture coordinates */

    wArcStripLeft->computeNormals(wArcStripLeft->GLDesc[Draw]->VertexData,nullptr);
    wArcStripLeft->computeTexCoords(wArcStripLeft->GLDesc[Draw]->VertexData);

    wCandy.add(wArcStripLeft);


    zbs::ZArray<glm::vec3> wVFR = perfect_arc_right_Front(pComponents.FRMid,wRadius,20);
    ZObject* wArcFR=new ZObject("ArcFR",ZObject::Arc);
    wArcFR->createVertexOnly(Draw);
//    if (pGenerateShape)
//            wArcFL->createVertexOnly(Shape);

    for (int wi=2;wi < wVFR.size();wi++)
        {
        wArcFR->addVertex(Draw,wVFR[0]);
        wArcFR->addVertex(Draw,wVFR[wi-1]);
        wArcFR->addVertex(Draw,wVFR[wi]);

/*        if (pGenerateShape)
                {
                wArcFL->addVertex(Shape,wVFL[wi-1]);
                wArcFL->addVertex(Shape,wVFL[wi]);
                }*/
        }// for

    wArcFR->setDrawFigure(Draw,GL_TRIANGLE_FAN);

/* compute normal and texture coordinates */

    wArcFR->computeNormals(wArcFR->GLDesc[Draw]->VertexData,nullptr);
    wArcFR->computeTexCoords(wArcFR->GLDesc[Draw]->VertexData);

    wCandy.add(wArcFR);

    zbs::ZArray<glm::vec3> wVBR = perfect_arc_right_Back(pComponents.BRMid,wRadius,20);
    ZObject* wArcBR=new ZObject("ArcBR",ZObject::Arc);
    wArcBR->createVertexOnly(Draw);

    for (int wi=2;wi<wVBR.size();wi++)
        {
        wArcBR->addVertex(Draw,wVBR[0]);
        wArcBR->addVertex(Draw,wVBR[wi]);
        wArcBR->addVertex(Draw,wVBR[wi-1]);
        }// for

    wArcBR->setDrawFigure(Draw,GL_TRIANGLE_FAN);

/* compute normal and texture coordinates */

    wArcBR->computeNormals(wArcBR->GLDesc[Draw]->VertexData,nullptr);
    wArcBR->computeTexCoords(wArcBR->GLDesc[Draw]->VertexData);

    wCandy.add(wArcBR);

    ZObject* wArcStripRight=new ZObject("ArcStripRight",ZObject::ArcStrip);
    wArcStripRight->createVertexOnly(Draw);

    wFi=1;
    wBi=wVBR.count()-1;

    while ((wBi>0)&&(wFi<wVFR.count()))
        {
        /* first triangle counter-clockwise*/
        wArcStripRight->addVertex(Draw,wVFR[wFi]);
        wArcStripRight->addVertex(Draw,wVBR[wBi]);
        wArcStripRight->addVertex(Draw,wVBR[wBi-1]);

        /* compute normal dir is default option when VNormalDir does not exist for the context */
//        wArcStrips->VNormalDir.push_back( ZObject::Compute); /* one normal direction per triangle */

        /* second adjacent triangle counter-clockwise*/
        wArcStripRight->addVertex(Draw,wVBR[wBi]);
        wArcStripRight->addVertex(Draw,wVFR[wFi]);
        wArcStripRight->addVertex(Draw,wVFR[wFi-1]);

        wFi++;
        wBi--;
//        wArcStrips->VNormalDir.push_back( ZObject::Compute); /* one normal direction per triangle */
        }//for

    wArcStripRight->setDrawFigure(Draw,GL_TRIANGLES);

/* compute normal and texture coordinates */

    wArcStripRight->computeNormals(wArcStripRight->GLDesc[Draw]->VertexData,nullptr);
    wArcStripRight->computeTexCoords(wArcStripRight->GLDesc[Draw]->VertexData);

    wCandy.add(wArcStripRight);


#ifdef __COMMENT__

    ZObject* wArcFR = generate_ArcFrontBack(pComponents.FRMid,
                                            wRadius,
                                            20,
                                            1, /* == 1 Front ; == 0 Back */
                                            0,/* == 1 Left ; == 0 Right */
                                            pGenerateShape,
                                            "ArcFR");
    wCandy.add(wArcFR);

    ZObject* wArcBR = generate_ArcFrontBack(pComponents.BRMid,
                                            wRadius,
                                            20,
                                            0, /* == 1 Front ; == 0 Back */
                                            0,/* == 1 Left ; == 0 Right */
                                            pGenerateShape,
                                            "ArcBR");
    wCandy.add(wArcBR);

#endif // __COMMENT__

 //   ZObject* wArcStripsLeft = generate_ArcStripsLeft(*wArcFL,*wArcBL,"ArcStripLeft");
 //   wCandy.add(wArcStripsLeft);

//    ZObject* wArcStripsRight = generate_ArcStripsRight(*wArcFR,*wArcBR,"ArcStripRight");
//    wCandy.add(wArcStripsRight);


/*=================ZCandy Line Shape vertices setup =====================*/

    if (!pGenerateShape)
            return wCandy;

    zbs::ZArray<ZVertice>* wVt=wOpenBox->GLDesc[Draw]->VertexData;

 /* Shape face is slightly (ShapeP) out the main object's figure according normal direction
  *
  *  we must keep normal coordinates to compute shape vertices.
 */


/* front face */
    wCandy.FrontShape.push_back(wVt->Tab[pComponents.FTRIdx]);
    wCandy.FrontShape.push_back(wVt->Tab[pComponents.FTLIdx]);
    for (long wi=0;wi<wArcFL->verticeCount();wi+=3)
        {
        /* skip first of 3 vertices which is center of arc */
        wCandy.FrontShape.push_back((*wArcFL)[wi+1]);
        wCandy.FrontShape.push_back((*wArcFL)[wi+2]);
        }
    wCandy.FrontShape.push_back(wVt->Tab[pComponents.FLLIdx]);
    wCandy.FrontShape.push_back(wVt->Tab[pComponents.FLRIdx]);

    for (long wi=0;wi<wArcFR->verticeCount();wi+=3)
        {
        /* skip first of 3 vertices which is center of arc */
        wCandy.FrontShape.push_back((*wArcFR)[wi+1]);
        wCandy.FrontShape.push_back((*wArcFR)[wi+2]);
        }
/* closing the loop */
    wCandy.FrontShape.push_back(wVt->Tab[pComponents.FTRIdx]);

/* backward face */
    wCandy.BackShape.push_back(wVt->Tab[pComponents.BLLIdx]);
    wCandy.BackShape.push_back(wVt->Tab[pComponents.BLRIdx]);

    /* skip first of 3 vertices which is center of arc */
    for (long wi=0;wi<wArcBL->verticeCount();wi+=3)
        {
        wCandy.BackShape.push_back((*wArcBL)[wi+1]);
        wCandy.BackShape.push_back((*wArcBL)[wi+2]);
        }

    wCandy.BackShape.push_back(wVt->Tab[pComponents.BTRIdx]);
    wCandy.BackShape.push_back(wVt->Tab[pComponents.BTLIdx]);

    for (long wi=0;wi<wArcBR->verticeCount();wi+=3)
       {
        wCandy.BackShape.push_back((*wArcBR)[wi+1]);
        wCandy.BackShape.push_back((*wArcBR)[wi+2]);
       }

/* closing the loop */
   wCandy.BackShape.push_back(wVt->Tab[pComponents.BLLIdx]);

   wCandy.DrawFigure[Shape]=GL_LINE_LOOP;
/* OK */


#ifdef __COMMENT__
   /* front face */
       wCandy.FrontShape.push_back(pComponents.FTR);
       wCandy.FrontShape.push_back(pComponents.FTL);
       for (long wi=0;wi<wArcFL->verticeCount();wi+=3)
           {
           /* skip first which is center of arc */
           wCandy.FrontShape.push_back((*wArcFL)[wi+1].point);
           wCandy.FrontShape.push_back((*wArcFL)[wi+2].point);
           }
       wCandy.FrontShape.push_back(pComponents.FLL);
       wCandy.FrontShape.push_back(pComponents.FLR);

       for (long wi=wArcFR->lastVertexIdx();wi>0;wi-=3) /* start by end and skip first which is center of arc */
           {
           wCandy.FrontShape.push_back((*wArcFL)[wi].point);
           wCandy.FrontShape.push_back((*wArcFL)[wi-1].point);
           }

       wCandy.FrontShape.push_back(pComponents.FTR);

   /* backward face */

       wCandy.BackShape.push_back(pComponents.BTR);
       wCandy.BackShape.push_back(pComponents.BTL);
       for (long wi=0;wi<wArcBL->verticeCount();wi+=3)
           {
           /* skip first which is center of arc */
           wCandy.BackShape.push_back((*wArcBL)[wi+1].point);
           wCandy.BackShape.push_back((*wArcBL)[wi+2].point);
           }
       wCandy.BackShape.push_back(pComponents.BLL);
       wCandy.BackShape.push_back(pComponents.BLR);

       for (long wi=wArcBR->lastVertexIdx();wi>0;wi-=3) /* start by end and skip first which is center of arc */
           {
           wCandy.BackShape.push_back((*wArcBR)[wi].point);
           wCandy.BackShape.push_back((*wArcBR)[wi-1].point);
           }

      wCandy.BackShape.push_back(pComponents.BTR);
#endif // __COMMENT__

/* end indices */

    return wCandy;
}//Candyboxsetup

//#endif // __COMMENT__

zbs::ZArray<glm::vec3>
generateDisk(glm::vec3 pOrigin, double pRadius,glm::vec3 pDirection,int pNumber)
{
zbs::ZArray<glm::vec3> wC1;

    /* radius is perpendicular to direction */
    glm::vec3 wPerp = glm::normalize(glm::perp(pOrigin,pDirection))*pRadius;
    glm::vec3 wDNorm=glm::normalize(pDirection);
    /* absolute value see glm::abs() */
    if (wDNorm.x<0.0)
           wDNorm.x=-wDNorm.x;
    if (wDNorm.y<0.0)
           wDNorm.y=-wDNorm.y;
    if (wDNorm.z<0.0)
           wDNorm.z=-wDNorm.z;

    /* normalised direction(sign) per axis : from direction vector */
    glm::vec3 wVSign =glm::vec3(1.0f);
    if (pDirection.x<0.0)
           wVSign.x=-wVSign.x;
    if (pDirection.y<0.0)
           wVSign.y=-wVSign.y;
    if (pDirection.z<0.0)
           wVSign.z=-wVSign.z;

    double wT= 0;

    double wLimit = 2.0 *double (M_PI);
    double wIncrement =  (2.0*double(M_PI)) /double(pNumber) ;

    glm::vec3 wPoint;

    while ((wT <= wLimit)&&(wT>=0.0))
    {
        wPoint.x=pOrigin.x + ( cos(wT)* pRadius * (1.0-wDNorm.x) * wVSign.x );
        wPoint.y=pOrigin.y + ( sin(wT) * pRadius *(1.0-wDNorm.y) * wVSign.y );
        wPoint.z=pOrigin.z  +(((sin(wT)* pRadius *( wDNorm.y)) + ((cos(wT) * pRadius *(wDNorm.x)) ))*(1.0-wDNorm.z) * wVSign.z );
        wC1.push_back( wPoint);
        wT+= wIncrement;
    }

/*
    fprintf(stdout,
            " pOrigin           x:<%f> y:<%f> z:<%f>\n"
            " pTargetCenter     x:<%f> y:<%f> z:<%f>\n"
            " Direction         x:<%f> y:<%f> z:<%f>\n"
            " Dx sin <%f> cos <%f> \n"
            " Dy sin <%f> cos <%f> \n"
            " Dz sin <%f> cos <%f> \n"
            " Radius value      <%f>\n"
            " Perp              x:<%f> y:<%f> z:<%f>\n"
            " wV                x:<%f> y:<%f> z:<%f>\n"
            ,
            pOrigin.x, pOrigin.y, pOrigin.z,
            pTargetCenter.x, pTargetCenter.y, pTargetCenter.z,
            wDirection.x, wDirection.y, wDirection.z,
            sinf(wDirection.x),cosf(wDirection.x),
            sinf(wDirection.y),cosf(wDirection.y),
            sinf(wDirection.z),cosf(wDirection.z),
            pRadius,
            wPerp.x,wPerp.y,wPerp.z,
            wDNorm.x,wDNorm.y,wDNorm.z);
  */

    return wC1;
}//generateDisk

glm::vec3 getMainDirection(glm::vec3 pDirection)
{
    glm::vec3 wMainDir=glm::vec3(0.0f) , wDirAbs=glm::abs(pDirection);
    if ((abs(wDirAbs.x) > abs(wDirAbs.y))&&(abs(wDirAbs.x) > abs(wDirAbs.z)))
            wMainDir.x = pDirection.x;
    if ((abs(wDirAbs.y) > abs(wDirAbs.x))&&(abs(wDirAbs.y) > abs(wDirAbs.z)))
            wMainDir.y = pDirection.y;
    if ((abs(wDirAbs.z) > abs(wDirAbs.x))&&(abs(wDirAbs.z) > abs(wDirAbs.y)))
            wMainDir.z = pDirection.z;
    return wMainDir;
}


zbs::ZArray<glm::vec3>
generateDiskNorm(glm::vec3 pOrigin, double pRadius,glm::vec3 pDirection,int pNumber)
{
zbs::ZArray<glm::vec3> wC1;

/* define the main direction of vector pDirection */

    glm::vec3 wMainDir=getMainDirection(pDirection);

    /* radius is perpendicular to main direction */
    glm::vec3 wPerp = glm::normalize(glm::perp(pOrigin,wMainDir))*pRadius;
    glm::vec3 wDNorm=glm::abs(glm::normalize(wMainDir));

    /* normalised direction(sign) per axis : from direction vector */
    glm::vec3 wVSign =glm::vec3(1.0f);
    if (wMainDir.x<0.0)
           wVSign.x=-wVSign.x;
    if (wMainDir.y<0.0)
           wVSign.y=-wVSign.y;
    if (wMainDir.z<0.0)
           wVSign.z=-wVSign.z;

    double wT= 0;

    double wLimit = 2.0 *double (M_PI);
    double wIncrement =  (2.0*double(M_PI)) /double(pNumber) ;

    glm::vec3 wPoint;

    while ((wT <= wLimit)&&(wT>=0.0))
    {
        wPoint.x=pOrigin.x + ( cos(wT)* pRadius * (1.0-wDNorm.x) * wVSign.x );
        wPoint.y=pOrigin.y + ( sin(wT) * pRadius *(1.0-wDNorm.y) * wVSign.y );
        wPoint.z=pOrigin.z  +(((sin(wT)* pRadius *( wDNorm.y)) + ((cos(wT) * pRadius *(wDNorm.x)) ))*(1.0-wDNorm.z) * wVSign.z );
        wC1.push_back( wPoint);
        wT+= wIncrement;
    }

/*
    fprintf(stdout,
            " pOrigin           x:<%f> y:<%f> z:<%f>\n"
            " pTargetCenter     x:<%f> y:<%f> z:<%f>\n"
            " Direction         x:<%f> y:<%f> z:<%f>\n"
            " Dx sin <%f> cos <%f> \n"
            " Dy sin <%f> cos <%f> \n"
            " Dz sin <%f> cos <%f> \n"
            " Radius value      <%f>\n"
            " Perp              x:<%f> y:<%f> z:<%f>\n"
            " wV                x:<%f> y:<%f> z:<%f>\n"
            ,
            pOrigin.x, pOrigin.y, pOrigin.z,
            pTargetCenter.x, pTargetCenter.y, pTargetCenter.z,
            wDirection.x, wDirection.y, wDirection.z,
            sinf(wDirection.x),cosf(wDirection.x),
            sinf(wDirection.y),cosf(wDirection.y),
            sinf(wDirection.z),cosf(wDirection.z),
            pRadius,
            wPerp.x,wPerp.y,wPerp.z,
            wDNorm.x,wDNorm.y,wDNorm.z);
  */

    return wC1;
}//generateDisk

/* NB face (direction) is deduced from pOrigin - pTarget */
ZObject*
generateCylinderPointToPoint(glm::vec3 pOrigin,
                             glm::vec3 pTarget,
                             double pRadius,
                             int pNumber,
                             const char* pName)
{

    glm::vec3 wDirection = pTarget - pOrigin;
    zbs::ZArray<glm::vec3> wC1=generateDiskNorm(pOrigin,pRadius,wDirection,pNumber);
    zbs::ZArray<glm::vec3> wC2=generateDiskNorm(pTarget,pRadius,wDirection,pNumber);

    ZObject* wP=new ZObject(pName,ZObject::Pipe);

    wP->createVertexOnly(Draw);

    for (long wi=wC1.lastIdx();(wi > 0) ;wi--)
    {
        wP->addVertex(Draw,wC1[wi]);
        wP->addVertex(Draw,wC2[wi]);
        wP->addVertex(Draw,wC2[wi-1]);

        wP->addVertex(Draw,wC2[wi-1]);
        wP->addVertex(Draw,wC1[wi-1]);
        wP->addVertex(Draw,wC1[wi]);
    }

    /* closing the cylinder surface */

    wP->addVertex(Draw,wC1[0]);
    wP->addVertex(Draw,wC2[0]);
    wP->addVertex(Draw,wC2.last());

    wP->addVertex(Draw,wC2.last());
    wP->addVertex(Draw,wC1.last());
    wP->addVertex(Draw,wC1[0]);

    wP->computeNormals(wP->GLDesc[Draw]->VertexData,nullptr);
    wP->computeTexCoords(wP->GLDesc[Draw]->VertexData);

    wP->setDrawFigure(Draw,GL_TRIANGLES);


    wP->createVertexOnly(Shape);

    wP->addVerticeArray(Shape,&wC1);
    wP->addVertex(Shape,wC1[0]);
    wP->addVertex(Shape,wC2[0]);
    wP->addVerticeArray(Shape,&wC2);
    wP->addVertex(Shape,wC2.last());
    wP->addVertex(Shape,wC1.last());

    wP->setDrawFigure(Shape,GL_LINE_LOOP);

    return  wP;
}



ZObject*
generateCylinder(Vertice_type pBeginCenter,
                 double pBeginRadius,
                 Vertice_type pTargetCenter,
                 double pTargetRadius,
                 int pNumber,
                 ZObject::CircleMade pBeginCircleState,
                 ZObject::CircleMade pTargetCircleState,
                 const char* pName)
{
    ZObject* wBegin= generate_Circle(pBeginCenter,pBeginRadius,pNumber,pBeginCircleState);
    ZObject* wTarget=generate_Circle(pTargetCenter,pTargetRadius,pNumber,pTargetCircleState);
    ZObject* wPipe=new ZObject(pName,ZObject::Pipe);

    wPipe->createVertexOnly(Draw);

//    wPipe->setDefaultColor(pColor);

    wPipe->addVertex(Draw,(*wBegin)[0].point);
    wPipe->addVertex(Draw,(*wBegin)[wBegin->lastVertexIdx()].point);
    wPipe->addVertex(Draw,(*wTarget)[0].point);
//    wPipe->addNormalDir(Draw,Compute);

    wPipe->addVertex(Draw,(*wTarget)[0].point);
    wPipe->addVertex(Draw,(*wBegin)[wBegin->lastVertexIdx()].point);
    wPipe->addVertex(Draw,(*wBegin)[wBegin->lastVertexIdx()].point);
//    wPipe->addNormalDir(Draw,Compute);

    /* Nota bene : VNormalDir is nullptr by default -> force Compute when setting normals */

  /*  *wPipe << wBegin->vertices[0].point;
    *wPipe << wBegin->vertices[wBegin->vertices.count()-1].point;
    *wPipe << wTarget->vertices[0].point;
    wPipe->VNormalDir.push_back(ZObject::Compute);

    *wPipe << wTarget->vertices[0].point;
    *wPipe << wTarget->vertices[wBegin->vertices.count()-1].point;
    *wPipe << wBegin->vertices[wBegin->vertices.count()-1].point;
    wPipe->VNormalDir.push_back(ZObject::Compute);

*/

    long wi=0;

    for (; wi < (wBegin->lastVertexIdx(Draw)) ;wi++)
    {
        wPipe->addVertex(Draw,(*wBegin)[wi+1].point);
        wPipe->addVertex(Draw,(*wBegin)[wi].point);
        wPipe->addVertex(Draw,(*wTarget)[wi+1].point);
//        wPipe->addNormalDir(Draw,Compute);

        wPipe->addVertex(Draw,(*wTarget)[wi].point);
        wPipe->addVertex(Draw,(*wTarget)[wi+1].point);
        wPipe->addVertex(Draw,(*wBegin)[wi].point);
//        wPipe->addNormalDir(Draw,Compute);
    }

/*
    for (; wi < (wBegin->vertices.count()-1) ;wi++)
    {
        *wPipe << wBegin->vertices[wi+1].point;
        *wPipe << wBegin->vertices[wi].point;
        *wPipe << wTarget->vertices[wi+1].point;
        wPipe->VNormalDir.push_back(ZObject::Compute);

        *wPipe << wTarget->vertices[wi].point;
        *wPipe << wTarget->vertices[wi+1].point;
        *wPipe << wBegin->vertices[wi].point;
        wPipe->VNormalDir.push_back(ZObject::Compute);
    }
    */

    if ((wBegin->verticeCount()<2)||(wTarget->verticeCount()<2))
                return wPipe;

    long wj=wBegin->lastVertexIdx(Draw);


    wPipe->addVertex(Draw,(*wBegin)[0].point);
    wPipe->addVertex(Draw,(*wBegin)[wBegin->lastVertexIdx(Draw)].point);
    wPipe->addVertex(Draw,(*wTarget)[wTarget->lastVertexIdx(Draw)].point);
//    wPipe->addNormalDir(Draw,Compute);

    wPipe->addVertex(Draw,(*wTarget)[wTarget->lastVertexIdx(Draw)].point);
    wPipe->addVertex(Draw,(*wTarget)[0].point);
    wPipe->addVertex(Draw,(*wBegin)[0].point);
//    wPipe->addNormalDir(Draw,Compute);

    delete wTarget;
    delete wBegin;

    return wPipe;
}//generateRegularPipe

ZObject*
generateRegularCylinder(Vertice_type pBeginCenter,
                        Vertice_type pTargetCenter,
                        float pRadius,
                        int pNumber,
                        ZObject::CircleMade pCircleState,
                        const char *pName)
{
    return generateCylinder(pBeginCenter,
                            pRadius,
                            pTargetCenter,
                            pRadius,
                            pNumber,
                            pCircleState,
                            pCircleState,
                            pName);

}

ZObject*
generateDirectionCylinder(Vertice_type pBeginCenter,
                          Vertice_type pDirection,
                          float pRadius,
                          int pNumber,
                          ZObject::CircleMade pCircleState,
                          const char*pName=nullptr)
{
    Vertice_type wTargetCenter = pBeginCenter + pDirection;
    return generateCylinder(pBeginCenter,
                            pRadius,
                            wTargetCenter,
                            pRadius,
                            pNumber,
                            pCircleState,
                            pCircleState,
                            pName);

}

ZObject
generateSphere(Vertice_type pBeginCenter,
               float pRadius,
               int pNumber,
               const char*pName=nullptr)
{
   ZObject wSphere(pName,ZObject::Sphere);

}

/*----------------------------------------------------------------------------*/
// return face normal of a triangle v1-v2-v3
// if a triangle has no surface (normal length = 0), then return a zero vector
/*----------------------------------------------------------------------------*/


glm::vec3 computeFaceNormal(glm::vec3 pV1,   // v1
                            glm::vec3 pV2,   // v2
                            glm::vec3 pV3)   // v3

{
    const float EPSILON = 0.000001f;
    Vertice_type wNormal=Vertice_type(0.0f,0.0f,0.0f); // default return value (0,0,0)

//    zbs::ZArray<float> normal;
//    normal.addValues(3, 0.0f);     // default return value (0,0,0)
    float nx=0.0f, ny=0.0f, nz=0.0f;

    // find 2 edge vectors: v1-v2, v1-v3
    float ex1 = pV2.x - pV1.x;
    float ey1 = pV2.y - pV1.y;
    float ez1 = pV2.z - pV1.z;
    float ex2 = pV3.x - pV1.x;
    float ey2 = pV3.y - pV1.y;
    float ez2 = pV3.z - pV1.z;

    // cross product: e1 x e2
    nx = ey1 * ez2 - ez1 * ey2;
    ny = ez1 * ex2 - ex1 * ez2;
    nz = ex1 * ey2 - ey1 * ex2;

    // normalize only if the length is > 0
    float length = sqrtf(nx * nx + ny * ny + nz * nz);

    if(length > EPSILON)
    {
        // normalize
        float lengthInv = 1.0f / length;
        wNormal.x= nx * lengthInv;
        wNormal.y = ny * lengthInv;
        wNormal.z = nz * lengthInv;
 /*       normal[0] = nx * lengthInv;
        normal[1] = ny * lengthInv;
        normal[2] = nz * lengthInv;*/
    }

//    return normal;
    return wNormal;
} //computeFaceNormal


glm::vec3 computeFaceNormalFloat(float x1, float y1, float z1,  // v1
                                 float x2, float y2, float z2,  // v2
                                 float x3, float y3, float z3)  // v3
{
    const float EPSILON = 0.000001f;
    Vertice_type wNormal=Vertice_type(0.0f,0.0f,0.0f); // default return value (0,0,0)

//    zbs::ZArray<float> normal;
//    normal.addValues(3, 0.0f);     // default return value (0,0,0)
    float nx, ny, nz;

    // find 2 edge vectors: v1-v2, v1-v3
    float ex1 = x2 - x1;
    float ey1 = y2 - y1;
    float ez1 = z2 - z1;
    float ex2 = x3 - x1;
    float ey2 = y3 - y1;
    float ez2 = z3 - z1;

    // cross product: e1 x e2
    nx = ey1 * ez2 - ez1 * ey2;
    ny = ez1 * ex2 - ex1 * ez2;
    nz = ex1 * ey2 - ey1 * ex2;

    // normalize only if the length is > 0
    float length = sqrtf(nx * nx + ny * ny + nz * nz);


    if(length > EPSILON)
    {
        // normalize
        float lengthInv = 1.0f / length;
        wNormal.x= nx * lengthInv;
        wNormal.y = ny * lengthInv;
        wNormal.z = nz * lengthInv;
 /*       normal[0] = nx * lengthInv;
        normal[1] = ny * lengthInv;
        normal[2] = nz * lengthInv;*/
    }

//    return normal;
    return wNormal;
} //computeFaceNormalFloat
