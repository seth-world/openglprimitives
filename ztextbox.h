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

enum RBoxPos : uint32_t
{
    RBP_Nothing             = 0,
    /* for horizontal display only */
    RBP_Center              = 0x01, /* centered either horizontally or vertically according diplay mode (vertical or horizontal)*/
    RBP_LeftJust            = 0x04, /* text is horizontally left justified (default) */
    RBP_RightJust           = 0x08, /* text is horizontally right justified */
    /* for vertical text display only */
//    RBP_VertCenter      = 0x02, /* vertically centered */
    RBP_TopJust             = 0x10, /* text is vertically display starting at top of box (default) */
    RBP_BotJust             = 0x20, /* text is vertically displayed to box bottom */

    RBP_LineWrap            = 0x40, /* Text is cut where line/column ends without taking care of words*/
    RBP_WordWrap            = 0x80, /* Text is wrapped by word if it does not fit into box boundary (default)*/
    RBP_TruncChar           = 0x0100, /* Displays a truncate sign at the end of the truncated line */
//    RBP_FitVertical     = 0x80,          /* Text should fit into vertical box boundary  */

    RBP_AdjustFSize         = 0x0200,/* Text should fit as it is adjusting font size if necessary :
                                    This option must be set only if RBP_Wrap is not set
                                    to make text with fit into box maximum width */

    RBP_TextMask           =    0x00FFFF,

/* box drawing flag */
    RBP_BoxVisible         =    0x010000,
    RBP_BoxShape           =    0x020000,
    RBP_BoxFill            =    0x040000,
    RBP_BoxTexture         =    0x080000,

    RBP_BoxMask            =  0xFFFF0000,

//    RBP_AdjustBestTry   = 0x0200,       /* Text size is being adjusted if it does not fit vertically after being cut (default) */

    RBP_Default         = RBP_LeftJust | RBP_TopJust | RBP_LineWrap | RBP_BoxVisible | RBP_BoxShape,

};




class ZTexture;
class ZShader;
class ZShaderContext;
class ZGLTextWriter;
class ZGLUnicodeText;
class ZTextBox
{
public:
    ZTextBox()=delete;
    ZTextBox(ZGLUnicodeText* pFather);
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

    ZGLUnicodeText* Father=nullptr;

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
