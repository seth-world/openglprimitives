#ifndef ZTEXTBOX_H
#define ZTEXTBOX_H
#ifdef __USE_GLAD__
#include <glad/glad.h>
#else
#include <GL/glew.h>
#endif

#include <glm/glm.hpp>
#include <ztoolset/zcharset.h> // for utfxx_t

#include <zglconstants.h>
//#include <zmatrices.h>



class ZTexture;
class ZShader;
class ZShaderContext;
class ZGLTextProfile;
class ZGLText;


struct TextAttributes
{
    glm::vec3   TextColor;
    glm::vec3   BoxBorderColor;

    glm::vec3*  BoxFillColor=nullptr;
    ZTexture*   BoxFillTexture=nullptr;
    float       BoxBorderWidth;
    float RightMargin    = 0.0;
    float LeftMargin     = 0.0;
    float TopMargin      =0.0;
    float BottomMargin   = 0.0;
    uint32_t Flag=RBP_Default;
    float Width=-1.0;
    float Height=-1.0;
};

class ZTextBox
{
public:
    ZTextBox()=delete;
    ZTextBox(ZGLText* pFather);
    ~ZTextBox();

    void setPixelDimensions (int pBoxWidth, int pBoxHeight);
    void setGLDimensions (float pBoxWidth, float pBoxHeight);
    void setMargins  (int pLeftMargin, int pRightMargin,int pTopMargin,int pBottomMargin)
        {
        RightMargin=pRightMargin;
        LeftMargin=pLeftMargin;
        TopMargin=pTopMargin;
        BottomMargin=pBottomMargin;
        }
    void setMarginsAll(int pMargin)
        {
        RightMargin=pMargin;
        LeftMargin=pMargin;
        TopMargin=pMargin;
        BottomMargin=pMargin;
        }


    /* fill on/off */
    void setFill(bool pFill)
        {
        if (pFill)
            Flag |= RBP_BoxFill ;
        else
            Flag &= (uint16_t)~RBP_BoxFill;
        }

    /* view border line on/off */
    void setBorder(bool pBorder)
    {
        if (pBorder)
            Flag |= RBP_BoxShape ;
        else
            Flag &= (uint16_t)~RBP_BoxShape;
    }


    /* view box on/off */
    void setVisible (bool pVisible)
        {
        if (pVisible)
            Flag |= RBP_BoxVisible ;
        else
            Flag &= (uint16_t)~RBP_BoxVisible;
        }

    /* force box flag value */
    void setFlag(uint32_t pFlag) {Flag=pFlag;}

/* Shape context -- Border */
    void setBorderWidth (float pWidth) ;
    void setBorderColor(glm::vec3 pColor) ;
    void setBorderAlpha(float pAlpha);
/*----*/


/* Fill context - (Draw)  ---*/
    void setFillColor(glm::vec3 pColor);
    void setFillAlpha(float pAlpha);

/*    void setAlpha(float pAlpha) {   Alpha= pAlpha; }*/

    int setTextureByRank (const long pIdx);
    int setTextureByName (const char* pIntName);

    void setupGLShape   ();
    void setupGLFill ();

    void drawBox();
    void _drawShape ();
    void _drawBackground ();

    void setupGL ();

    /* change / replace shader context  with a new specific one (shader context is inherited from ZGLTextWriter */

    int changeBoxShaderByName(const int pCtx,const char* pIntName);
    int replaceBoxShaderByName(const int pCtx,const char* pIntName);
    int changeBoxShaderByRank(const int pCtx,const long pRank);
    int replaceBoxShaderByRank(const int pCtx,const long pRank);

/* Text Box */
    // Render state
    GLuint BoxVAOShape=0, BoxVBOShape=0 , BoxVAOFill=0, BoxVBOFill=0 ,BoxVBOTexture=0,BoxVBONormal=0 ;
//    ZTexture*   Texture=nullptr;

    ZGLText* Father=nullptr;

    float Alpha=1.0f;

/* End Text Box */

/* text box */
    uint32_t Flag=RBP_Default;
    float Width=-1.0;
    float Height=-1.0;
    float RightMargin    = 0.0;
    float LeftMargin     = 0.0;
    float TopMargin      =0.0;
    float BottomMargin   = 0.0;

    int IWidth=-1;
    int IHeight=-1;

    float LineSize=-1.0;

//    float* TextBoxcoords;

    glm::vec3 LineColor=ZBlueColor;
    glm::vec3 FillColor=ZBlueColor;

    glm::mat4* Model=nullptr;
    glm::mat4* View=nullptr;
    glm::mat4* Projection=nullptr;
    glm::mat4* Normal=nullptr;

    ZShaderContext* ShaderContext[2]={nullptr,nullptr};

/* end text box */

};


#endif // ZTEXTBOX_H
