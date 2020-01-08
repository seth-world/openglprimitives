#include <ztextbox.h>
#include <zglresource.h>
#include <zshader.h>
#include <ztexture.h>
#include <zglconstants.h>

#include <zshadercontext.h>
#include <zglunicodetext.h>

ZTextBox::ZTextBox(ZGLText *pFather)
{
    Father=pFather;

    Model=&Father->Model;
    Projection=&Father->Projection;
    View=&Father->View;
    Normal=&Father->Normal;
}

ZTextBox::~ZTextBox()
{
    if (BoxVBOShape)
            glDeleteBuffers(1 ,&BoxVBOShape);
    if (BoxVAOShape)
            glDeleteVertexArrays(1 ,&BoxVAOShape);


    if (BoxVBOFill)
            glDeleteBuffers(1 ,&BoxVBOFill);
    if (BoxVBOTexture)
            glDeleteBuffers(1 ,&BoxVBOTexture);
    if (BoxVBONormal)
            glDeleteBuffers(1 ,&BoxVBONormal);

    if (BoxVAOFill)
            glDeleteVertexArrays(1 ,&BoxVAOFill);
    //   if (Texture!=nullptr)
    //               delete Texture;

    if (ShaderContext[0])
            delete ShaderContext[0];
    if (ShaderContext[1])
            delete ShaderContext[1];
}

void ZTextBox::setPixelDimensions (int pBoxWidth, int pBoxHeight)
{
    glm::vec2 wSCDim=GLResources->getGLWindowSize();
    IWidth = pBoxWidth;
    IHeight = pBoxHeight;
    Width= (float)pBoxWidth*2.0f/wSCDim.x;
    Height=(float)pBoxHeight*2.0f/wSCDim.y;
}
void ZTextBox::setGLDimensions (float pBoxWidth, float pBoxHeight)
{
    Width=pBoxWidth;
    Height=pBoxHeight;
    glm::vec2 wSCDim=GLResources->getGLWindowSize();
    IWidth = int((pBoxWidth*wSCDim.x)/2.0f);
    IHeight = int((pBoxHeight*wSCDim.y)/2.0f);
}

int ZTextBox::changeBoxShaderByName(const int pCtx,const char* pIntName)
{
    ZShader* wSh=GLResources->getShaderByNamePtr(pIntName);
    if (!wSh)
    {
        fprintf (stderr,"ZTextBox::changeBoxShaderByName-E-SHNTFND Shader named <%s> not found in registrated shaders. Shader is not changed.\n ",
                 pIntName);
        return -1;
    }
    ShaderContext[pCtx]->setShader(wSh);
    return 0;
}
int ZTextBox::replaceBoxShaderByName(const int pCtx,const char* pIntName)
{
    ZShader* wSh=GLResources->getShaderByNamePtr(pIntName);
    if (!wSh)
    {
        fprintf (stderr,"ZTextBox::replaceBoxShaderByName-E-SHNTFND Shader named <%s> not found in registrated shaders.\n"
                        " Shader <%s> is not replaced for shader context <%s>.\n ",
                 pIntName,
                 ShaderContext[pCtx]->getShaderName(),
                 decodeShdCtx(pCtx));
        return -1;
    }
    if (ShaderContext[pCtx])
        fprintf (stderr,"ZTextBox::replaceBoxShaderByName-I Shader context for shader <%s> is to be replaced with shader <%s>.\n",
                 ShaderContext[pCtx]->getShaderName(),
                 wSh->getName());
    if (ShaderContext[pCtx]->count())
        {
        fprintf (stderr,"ZTextBox::replaceBoxShaderByName-W-LOSTRULES Shader <%s> : <%ld> rules will be lost after shader replacement.\n",
                 ShaderContext[pCtx]->getShaderName(),
                 ShaderContext[pCtx]->count());
        }
    if (ShaderContext[pCtx])
            delete ShaderContext[pCtx];
    ShaderContext[pCtx]=new ZShaderContext(wSh);
    return 0;
}
int ZTextBox::changeBoxShaderByRank(const int pCtx,const long pRank)
{
    ZShader* wSh=GLResources->getShaderByRankPtr(pRank);
    if (!wSh)
    {
        fprintf (stderr,"ZTextBox::changeBoxShaderByName-E-SHNTFND Shader of rank <%ld> not found in registrated shaders. Shader is not changed.\n ",
                 pRank);
        return -1;
    }
    ShaderContext[pCtx]->setShader(wSh);
    return 0;
}
int ZTextBox::replaceBoxShaderByRank(const int pCtx,const long pRank)
{
    ZShader* wSh=GLResources->getShaderByRankPtr(pRank);
    if (!wSh)
    {
        fprintf (stderr,"ZTextBox::replaceBoxShaderByName-E-SHNTFND Shader of rank <%ld> not found in registrated shaders.\n"
                        " Shader <%s> is not replaced for shader context <%s>.\n ",
                 pRank,
                 ShaderContext[pCtx]->getShaderName(),
                 decodeShdCtx(pCtx));
        return -1;
    }
    if (ShaderContext[pCtx])
        fprintf (stderr,"ZTextBox::replaceBoxShaderByName-I Shader context for shader rank <%s> is to be replaced with shader <%s>.\n",
                 ShaderContext[pCtx]->getShaderName(),
                 wSh->getName());
    if (ShaderContext[pCtx]->count())
        {
        fprintf (stderr,"ZTextBox::replaceBoxShaderByName-W-LOSTRULES Shader <%s> : <%ld> rules will be lost after shader replacement.\n",
                 ShaderContext[pCtx]->getShaderName(),
                 ShaderContext[pCtx]->count());
        }
    if (ShaderContext[pCtx])
            delete ShaderContext[pCtx];
    ShaderContext[pCtx]=new ZShaderContext(wSh);
    return 0;
}



int ZTextBox::setTextureByName (const char* pIntName)
{
    Flag |= (uint16_t)RBP_BoxTexture;
    return ShaderContext[Draw]->setTextureByName(pIntName);
}// setBoxTextureByName

int ZTextBox::setTextureByRank (const long pIdx)
{
    Flag |= (uint16_t)RBP_BoxTexture;
    return ShaderContext[Draw]->setTextureByRank(pIdx);
}// setTextureByRank


void ZTextBox::setBorderWidth (float pWidth)
{
    return ShaderContext[Shape]->setLineWidth(pWidth);
}
void ZTextBox::setBorderColor(glm::vec3 pColor)
{
    LineColor=pColor;
    Flag |= RBP_BoxShape ;
    return ShaderContext[Shape]->addVec3(__SHD_DEFAULTCOLOR_UN__,&LineColor);
}
void ZTextBox::setBorderAlpha(float pAlpha)
{
    return ShaderContext[Shape]->addFloat(__SHD_ALPHA_UN__,pAlpha);
}

void ZTextBox::setFillColor(glm::vec3 pColor)
    {
    FillColor = pColor;
    return ShaderContext[Shape]->addVec3(__SHD_DEFAULTCOLOR_UN__,&FillColor);
    Flag |= RBP_BoxFill ;
    }
void ZTextBox::setFillAlpha(float pAlpha)
    {
    return ShaderContext[Shape]->addFloat(__SHD_ALPHA_UN__,pAlpha);
    Flag |= RBP_BoxFill ;
    }

void ZTextBox::setupGL ()
{
   if (Flag&(RBP_BoxFill|RBP_BoxTexture))
                           setupGLFill();
   if (Flag&RBP_BoxShape)
                           setupGLShape();
}//_boxSetupGL

void ZTextBox::setupGLShape ()
{
   /* setup box coordinates */
    float wTextBoxShapecoords[15] = {
       0.0      ,        0.0   ,   0.0,
       Width ,   0.0        ,   0.0,
       Width ,   -Height  ,   0.0,
       0.0      ,   -Height  ,   0.0,
       0.0      ,   0.0        ,   0.0
       };

   /* setup GL context */
    ShaderContext[Shape]->useShader();
//   Writer->getBoxShaderShape();
//   Writer->BoxShader->use();

   if (BoxVAOShape==0)
       glGenVertexArrays(1, &this->BoxVAOShape);
   if (BoxVBOShape==0)
       glGenBuffers(1, &this->BoxVBOShape);

   glBindVertexArray(BoxVAOShape);

   glBindBuffer(GL_ARRAY_BUFFER, BoxVBOShape);
   glBufferData(GL_ARRAY_BUFFER, sizeof(wTextBoxShapecoords), wTextBoxShapecoords, GL_STATIC_DRAW);

   /* aPosition because ztextbox.vs shader */
   int wAttPosition=ShaderContext[Shape]->getPositionAttributeLocation(true); /* abort on error option set*/
   glVertexAttribPointer(wAttPosition,
                         3,        /* three items */
                         GL_FLOAT, /* type is float */
                         GL_FALSE, /* not to be normalized : use floating point directly */
                         0,        /* no stride */
                         (void*)0);
   glEnableVertexAttribArray(wAttPosition);

   ShaderContext[Shape]->addMat4(__SHD_MODEL__,Model);
   ShaderContext[Shape]->addMat4(__SHD_PROJECTION__,Projection);
   ShaderContext[Shape]->addMat4(__SHD_VIEW__,View);

  // unbind
   glBindVertexArray(0);
   glBindBuffer(GL_ARRAY_BUFFER,0);

   ShaderContext[Shape]->releaseShader();
}//_boxSetupGLShape



void ZTextBox::setupGLFill ()
{
    float wMinX , wMinY, wMaxX, wMaxY;
    float wLineSize=0.005f;
    if (Flag & RBP_BoxShape)
        {
        wMinX= wLineSize;
        wMinY= wLineSize - Height;
        wMaxX= Width - wLineSize;
        wMaxY= -wLineSize;
        }
    else
        {
        wMinX= 0.0f;
        wMinY= - Height;
        wMaxX= Width ;
        wMaxY= 0.0f;
        }
    /* setup box coordinates */
     float wTextBoxCoords [18] =
     {
         wMinX  , wMinY   , 0.0f,
         wMaxX  , wMinY   , 0.0f,
         wMaxX  , wMaxY   , 0.0f,
         wMaxX  , wMaxY   , 0.0f,
         wMinX  , wMaxY   , 0.0f,
         wMinX  , wMinY   , 0.0f
     };

     float wTextureBoxcoords[12] = {
        0.0f , 0.0f ,
        1.0f , 0.0f ,
        1.0f , 1.0f ,
        1.0f , 1.0f ,
        0.0f , 1.0f ,
        0.0f , 0.0f
        };
//     glm::vec3 wNormalBoxcoords[6];
     float wNormalBoxCoords [18] =
     {
         0.0f   , 0.0f   ,  1.0f,
         0.0f   , 0.0f    , 1.0f,
         0.0f   , 0.0f    , 1.0f,
         0.0f   , 0.0f    , 1.0f,
         0.0f   , 0.0f    , 1.0f,
         0.0f   , 0.0f    , 1.0f
     };


    /* setup GL context */
//    Writer->getBoxShaderFill();
//    Writer->BoxShader->use();

    if (!BoxVAOFill)
            glGenVertexArrays(1, &BoxVAOFill);

    if (!BoxVBOFill)
            glGenBuffers(1, &BoxVBOFill);

    if (!BoxVBONormal)
            glGenBuffers(1, &BoxVBONormal);

    if ((Flag&RBP_BoxTexture)&&(ShaderContext[Draw]->hasTexture()))
        {
        if (BoxVBOTexture==0)
                glGenBuffers(1, &BoxVBOTexture);
        }

    glBindVertexArray(BoxVAOFill);

    glBindBuffer(GL_ARRAY_BUFFER, BoxVBOFill);
    glBufferData(GL_ARRAY_BUFFER, sizeof(wTextBoxCoords), wTextBoxCoords, GL_STATIC_DRAW);

    /* aPosition because ztextbox.vs shader */
    int wAttPosition=ShaderContext[Fill]->getPositionAttributeLocation(true); /* abort on error option set*/
    glVertexAttribPointer(wAttPosition,
                          3,        /* three items <x,y,z>  */
                          GL_FLOAT, /* type is float */
                          GL_FALSE, /* not to be normalized : use floating point directly */
                          0,        /* no stride */
                          (void*)0);
    glEnableVertexAttribArray(wAttPosition);
    if ((Flag&RBP_BoxTexture)&&(ShaderContext[Fill]->hasTexture()))
        {
        glBindBuffer(GL_ARRAY_BUFFER, BoxVBOTexture);
        glBufferData(GL_ARRAY_BUFFER, sizeof(wTextureBoxcoords), wTextureBoxcoords, GL_STATIC_DRAW);
        /* aPosition because ztextbox.vs shader */
        wAttPosition=ShaderContext[Fill]->getTextureAttributeLocation(true); /* abort on error option is set*/
        if (wAttPosition >= 0)
            {
            glVertexAttribPointer(wAttPosition,
                                  2,        /* two items <s,t>*/
                                  GL_FLOAT, /* type is float */
                                  GL_FALSE, /* not to be normalized : use floating point directly */
                                  0,        /* no stride */
                                  (void*)0);
            glEnableVertexAttribArray(wAttPosition);
            }
        }//if (BoxFlag&RBP_Texture)

    glBindBuffer(GL_ARRAY_BUFFER, BoxVBONormal);
    glBufferData(GL_ARRAY_BUFFER, sizeof(wNormalBoxCoords), wNormalBoxCoords, GL_STATIC_DRAW);
    /* aPosition because ztextbox.vs shader */
    wAttPosition=ShaderContext[Fill]->getNormalAttributeLocation(false); /* abort on error option NOT set*/
    if (wAttPosition >= 0)
        {
        glVertexAttribPointer(wAttPosition,
                              3,        /* three items <nx,ny,nz>*/
                              GL_FLOAT, /* type is float */
                              GL_FALSE, /* not to be normalized : use floating point directly */
                              0,        /* no stride */
                              (void*)0);
        glEnableVertexAttribArray(wAttPosition);
        }
   // unbind

    ShaderContext[Fill]->addMat4(__SHD_MODEL__,Model);
    ShaderContext[Fill]->addMat4(__SHD_PROJECTION__,Projection);
    ShaderContext[Fill]->addMat4(__SHD_VIEW__,View);
    ShaderContext[Fill]->addMat4(__SHD_NORMAL__,Normal);

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER,0);
    ShaderContext[Fill]->releaseShader();
}//_boxSetupGLTexture

void ZTextBox::drawBox()
{
    if (Flag & RBP_BoxVisible)
    {
        if (Flag & (RBP_BoxTexture | RBP_BoxFill))
            {
            _drawBackground();
            }
    if (Flag & RBP_BoxShape)
            {
            _drawShape();
            }
    }

}//drawBox

void ZTextBox::_drawShape ()
{

    ShaderContext[Shape]->applyRules();

    glBindVertexArray(BoxVAOShape);

    glDrawArrays(GL_LINE_LOOP, 0 , 5);

    ShaderContext[Shape]->postProcess();

    glBindVertexArray(0);
    return;
}//_drawBoxShape


void ZTextBox::_drawBackground ()
{
    ShaderContext[Draw]->applyRules();

     // rules must be described within shader context : ShaderCtxFill
/*
    ShaderFill->use();
    ShaderFill->setFloat("Alpha",Alpha);

    if ( Texture && (Flag & RBP_BoxTexture))
        {
        Texture->bind();
        ShaderFill->setBool("UseTexture",true);
//        glBindBuffer(GL_ARRAY_BUFFER, BoxVBOTexture);
        ShaderFill->setupTexSampler(Texture);
        }
    else
        {
        ShaderFill->setBool("UseTexture",false);
        ShaderFill->setVec3("DefaultColor",BoxFillColor);
        }
    ShaderFill->setFloat(__BOXPOSZ__,0.0f);

*/

    glBindVertexArray(BoxVAOFill);

    glDrawArrays(GL_TRIANGLES, 0 , 6);

    ShaderContext[Draw]->postProcess();

    glBindVertexArray(0);

    return;
}//_drawBoxBackground
