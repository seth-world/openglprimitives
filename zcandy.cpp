#include "zcandy.h"
#ifdef __USE_GLAD__
#include <glad/glad.h>
#else
#include <GL/glew.h>
#endif


void ZCandy::computeShapeOp()
{
    glm::vec3 wSCoords;
    ShapeOp.clear();
    for (long wi=0; wi < FrontShape.count(); wi++)
            {
            wSCoords=FrontShape[wi].point + (FrontShape[wi].normal * ShapeDistance);
            ShapeOp.push_back(wSCoords);
            }
    for (long wi=0; wi < BackShape.count(); wi++)
            {
            wSCoords=BackShape[wi].point - (BackShape[wi].normal * ShapeDistance);
            ShapeOp.push_back(wSCoords);
            }

    BackStart = FrontShape.count();
    BackCount = BackShape.count();
    return;
}

/* method overriding metaobject setupGLShape() method

    NB: Only one Vertex buffer for both front and back shape design : offsets & sizes are positionned accordingly
*/
void ZCandy::setupGLShape(CSO_type pAction)
{
    if (ShaderContext[Shape]==nullptr)
            {
            fprintf(stderr,"ZCandy::setupGLShape-F-SHADNULL Object <%s> : Shape shader has not been defined while setting up GL context for shape.\n",
                    Name);
            exit (EXIT_FAILURE);
            }
    ShaderContext[Shape]->bindShader(); /* shape shader is defined within underlying meta object */

    if(!MatCtx)  /* idem for matrices : NB As shader contexts, matrices are defined at ZMetaObject level */
        {
        fprintf(stderr,"ZCandy::setupGLShape-F-MATNULL Object <%s> : Matrix context not been defined while setting up GL context for shape.\n",
                Name);
        exit (EXIT_FAILURE);
        }

    if(!FrontShapeDesc)
            FrontShapeDesc = new ZGLObjDescriptor;

/* concatenate Front and back Shapes : Not in first approach */

    BackStart= FrontShape.count(); /* first indice of BackShape after being concatenated  */
    BackCount = BackShape.count(); /* count of vertices for drawing back shape */

    computeShapeOp();

//    FrontShape += BackShape;                    /* concatenate front and back to one unique set of vertices */
 /* total number of elements to draw */
/* Front shape setup */
/* GL buffers setup */
    glGenVertexArrays(1, &FrontShapeDesc->VAO);
    glGenBuffers(1, &FrontShapeDesc->VBO);

    glBindVertexArray(FrontShapeDesc->VAO);
    glBindBuffer(GL_ARRAY_BUFFER, FrontShapeDesc->VBO);
//    glBufferData(GL_ARRAY_BUFFER, FrontShape.usedSize(), FrontShape.data(), GL_STATIC_DRAW);
    glBufferData(GL_ARRAY_BUFFER, ShapeOp.usedSize(), ShapeOp.data(), GL_STATIC_DRAW);

/* point position */
    GLuint wPos= ShaderContext[Shape]->getPositionAttribute();

//    glVertexAttribPointer(wPos, 3, GL_FLOAT, GL_FALSE, (GLsizei)sizeof(ZVertice), (void*)nullptr);

    glVertexAttribPointer(wPos,
                          3,
                          GL_FLOAT,
                          GL_FALSE,
                          (GLsizei)sizeof(glm::vec3),
                          (void*)nullptr);
    glEnableVertexAttribArray(wPos);
#ifdef __COMMENT__
/* Back shape setup */
/* GL buffers setup */

    glGenVertexArrays(1, &BackShapeDesc->VAO);
    glGenBuffers(1, &BackShapeDesc->VBO);

    glBindVertexArray(BackShapeDesc->VAO);
    glBindBuffer(GL_ARRAY_BUFFER, BackShapeDesc->VBO);
    glBufferData(GL_ARRAY_BUFFER, BackShape.usedSize(), BackShape.data(), GL_STATIC_DRAW);

/* point position */
    glVertexAttribPointer(BackShapeDesc->getPositionAttribute(pShader), 3, GL_FLOAT, GL_FALSE, (GLsizei)sizeof(Vertice_type), (void*)0);
    glEnableVertexAttribArray(BackShapeDesc->PositionAttribArray);
#endif // __COMMENT__
// You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
// VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
    // unbind
// note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
    glBindBuffer(GL_ARRAY_BUFFER, 0);

// unbind
    glBindVertexArray(0);
    ShaderContext[Shape]->releaseShader();

    if (pAction & CSO_DeleteDataAfter)
            {
            FrontShape.clear();
            BackShape.clear();
            ShapeOp.clear();
            }
} //

void ZCandy::drawGLShape()
{
    if (ShaderContext[Shape]==nullptr)
            {
            fprintf(stderr,"ZCandy::setupGLShape-F-SHADNULL Object <%s> : Shape shader has not been defined while setting up GL context for shape.\n",
                    Name);
            exit (EXIT_FAILURE);
            }
/*    float wLineWidth;  // done within ShaderContext::applyRules
    glGetFloatv(GL_LINE_WIDTH,&wLineWidth);

 //   ShaderContext[Shape]->bindShader();
*/
    ShaderContext[Shape]->applyRules();
    ZShader* wShader=GLResources->getActiveShaderPtr();
    if (wShader==nullptr)
            {
            fprintf(stderr,"ZObject::drawGL-E-NOSHD Object <%s> : No active shader while drawing object.No action made.\n",
                    Name);
            return;
            }

    _preprocessGLMatrices(wShader);

//    glLineWidth(2.0);     // must be done within ShaderContext::applyRules()
//    glBindBuffer(GL_ARRAY_BUFFER, FrontShapeDesc->VBO); // not necessary
    glBindVertexArray(FrontShapeDesc->VAO);
    /* draw front shape up until BackStart */
    glDrawArrays(DrawFigure[Shape], 0 , BackStart);
    /* draw back shape from BackStart till end */
//    glBindVertexArray(FrontShapeDesc->VAO);
    glDrawArrays(DrawFigure[Shape], (GLsizei)BackStart, BackCount);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    ShaderContext[Shape]->postProcess();

    delete wShader;

/*    glLineWidth(wLineWidth);// done within ShaderContext::postProces()
    ShaderContext[Shape]->releaseShader();
    */
    return;
}


void ZCandy::setupGLByContext(const DrawContext_type pCtx,CSO_type pAction)
{
    if (pCtx==Shape)
        {
            setupGLShape(pAction); // shader binding is made within
            return;
            }
    for (long wi=0;wi < count();wi++)
        {
        if (ShaderContext[pCtx])
                ShaderContext[pCtx]->bindShader();
        Tab[wi]->setupGL(pCtx,pAction);
        }
}//setupGLByContext



void ZCandy::drawGLByContext(const DrawContext_type pCtx)
{
    if (pCtx==Shape)
            {
            drawGLShape();
            return;
            }

    for (long wi=0;wi < count();wi++)
        {
        ZShader* wSh=nullptr;
        if (ShaderContext[pCtx])
            {
            ShaderContext[pCtx]->applyRules();
            wSh=GLResources->getActiveShaderPtr();
            if (MatCtx)
                MatCtx->_preprocessGL(wSh);         /* need to reapply global matrices (if exist) before each object draw */
            }
        Tab[wi]->drawGLByContext(pCtx);
        if (wSh)
            delete wSh;
        }
}//drawGLByContext

#include <zboxcomponents.h>



TextZone ZCandy::_getTextZone(double pDiv,const float pMargin)
{

    if ((pDiv < 2.0)||(pDiv > 10.0))
    {
        fprintf (stderr,"ZCandy::getTextZoneNormal-E-IVDIV <%f> is an invalid divisor for angle calculation. Must be [2.0,10.0] \n",
                 pDiv);
        pDiv=4.0;
    }

    TextZone wTZ;

    glm::vec3 wFTR=Tab[0]->BoxComponents->FTR;
    glm::vec3 wFTL=Tab[0]->BoxComponents->FTL;
    glm::vec3 wFLR=Tab[0]->BoxComponents->FLR;
    glm::vec3 wFLL=Tab[0]->BoxComponents->FLL;

    wTZ.Center =(Tab[0]->BoxComponents->FRMid - Tab[0]->BoxComponents->FLMid) / 2.0f;

    float wHeight = wFTR.y - wFLR.y ;
    float wWidth = wFTR.x - wFTL.x;
    float wRadius = wHeight / 2.0f ;

    glm::vec3 wTopLeft;
    glm::vec3 wLowLeft;
    glm::vec3 wTopRight;
    glm::vec3 wLowRight;

    wTZ.Center = Tab[0]->BoxComponents->getCenter(); /* hopefully 0.0,0.0,0.0 */

/*    fprintf (stdout,
             "----------------------------\n"
             " FLMid <x:%f y:%f z:%f>\n"
             " FRMid <x:%f y:%f z:%f>\n"
             " Center <x:%f y:%f z:%f>\n"
             " radius <%f>\n"
             ,
             Tab[0]->BoxComponents->FLMid.x, Tab[0]->BoxComponents->FLMid.y, Tab[0]->BoxComponents->FLMid.z,
             Tab[0]->BoxComponents->FRMid.x, Tab[0]->BoxComponents->FRMid.y,Tab[0]->BoxComponents->FRMid.z,
            wTZ.Center.x,wTZ.Center.y,wTZ.Center.z,
            wRadius
             );
*/
    double wAngle;
    double wDiv;
//    wDiv=4.0;
    wDiv=pDiv;
//    wAngle=double (M_PI)/ 4.0;
    wAngle=double (M_PI)/ wDiv;
    wTopRight.x = Tab[0]->BoxComponents->FRMid.x + (wRadius* cos(wAngle)) - pMargin;
    wTopRight.y = Tab[0]->BoxComponents->FRMid.y + (wRadius* sin(wAngle)) - pMargin;
    wTopRight.z = Tab[0]->BoxComponents->FRMid.z;

//     wAngle=double (M_PI)*3.0 / 4.0;
    wAngle=double (M_PI)*(wDiv-1.0)/ wDiv;
    wTopLeft.x = Tab[0]->BoxComponents->FLMid.x + (wRadius* cos(wAngle)) + pMargin;
    wTopLeft.y = Tab[0]->BoxComponents->FLMid.y + (wRadius* sin(wAngle)) - pMargin;
    wTopLeft.z = Tab[0]->BoxComponents->FLMid.z;

//    wAngle=double (M_PI)*5.0 / 4.0;
    wAngle=double (M_PI)*(wDiv+1.0)/ wDiv;

    wLowLeft.x = Tab[0]->BoxComponents->FLMid.x + (wRadius* cos(wAngle)) + pMargin;
    wLowLeft.y = Tab[0]->BoxComponents->FLMid.y + (wRadius* sin(wAngle)) + pMargin;
    wLowLeft.z = Tab[0]->BoxComponents->FLMid.z;


//    wAngle=double (M_PI)*7.0 / 4.0;
    wAngle=double (M_PI)*((2.0*wDiv)-1.0)/ wDiv;
    wLowRight.x = Tab[0]->BoxComponents->FRMid.x + (wRadius* cos(wAngle)) - pMargin;
    wLowRight.y = Tab[0]->BoxComponents->FRMid.y + (wRadius* sin(wAngle)) + pMargin;
    wLowRight.z = Tab[0]->BoxComponents->FRMid.z;


    wTZ.Height = float(wTopLeft.y - wLowLeft.y);
    wTZ.Width = float(wTopRight.x - wTopLeft.x);



    wTZ.ToTopLeft =   wTopLeft - wTZ.Center  ;


    fprintf (stdout,
             "----------------------------\n"
             " TopLeft <x:%f y:%f z:%f>\n"
             " LowLeft <x:%f y:%f z:%f>\n"
             " TopRight <x:%f y:%f z:%f>\n"
             " LowRight <x:%f y:%f z:%f>\n"
             ,
             wTopLeft.x, wTopLeft.y, wTopLeft.z,
             wLowLeft.x, wLowLeft.y, wLowLeft.z,
             wTopRight.x, wTopRight.y, wTopRight.z,
             wLowRight.x, wLowRight.y, wLowRight.z
             );

    return wTZ;
}
