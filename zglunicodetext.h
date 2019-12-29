#ifndef ZGLUNICODETEXT_H
#define ZGLUNICODETEXT_H
/**

program must use GLResources to load fonts and make them available to application

  +-------------+
  | GLResources |   . gets font file path
  +-------------+   . loads font definition
        |           . manages list of available fonts (substitute font if not found)
        |
        |
       +--
       | Table of loaded       +-----------+
       | Fonts                 | ZFont     |     . generic loaded font definition (stored within GLResources)
       +-                      +-----------+     . generates UnicodeFont
       |
       |
      operational font Face, font size
       |
       |
       |                       +----------------+ . manages a group of text (UnicodeText)
       |              USER ->  | GLTextWriter   | . generates UnicodeText
       |                       +----------------+ . manages shader, matrices for all dependant fonts
       |                +------------
   +-------------+      | Table of Texts (UnicodeText) objects
   | UnicodeFont |      +
   +-------------+      /
       \               /
        \             /
         \           /
          \         /
       +-------------+
USER-> | UnicodeText |   . application text with usable font sample : here are mentionned font dimensions
       +-------------+
                |
            +------
            | table of UnicodeChar -> freetype glyphs
            +-




 Usage Example :
 ---------------

    wRet=GLResources->addFont("FreeSans.ttf","FreeSans",false);
    GLFontWriter wUWriter(GL_TEXTURE0);
    GLUnicodeText* wUText=wUWriter.newText();
    wRet=wUText->setText((utf32_t*)U"Жди меня, и я вернусь.","FreeSans",24); //specify here font height in pixels
    if (wRet<0)
            fprintf (stderr," Font <%s> is not loaded \n","FreeSans");


 */
#ifdef __USE_GLAD__
#include <glad/glad.h>
#else
#include <GL/glew.h>
#endif

#include <glm/glm.hpp>
#include <ztoolset/zcharset.h> // for utfxx_t

#include <zglconstants.h>

#include <ft2build.h>

#include FT_FREETYPE_H

// Maximum texture width
#define __TEX_MAXWIDTH__ 1024
// Maximum size for an internal font name
#define __FONTNAME_MAX__ 1024


/* documentation : https://en.wikibooks.org/wiki/OpenGL_Programming/Modern_OpenGL_Tutorial_Text_Rendering_01 */

#include <ztextbox.h>



class ZTexture;
class ZTextTexture;
class ZGLTextWriter;
class ZGLUnicodeChar;
class ZGLUnicodeFont;

class ZMatCtx;

#ifndef __TEXTPOINT__
#define __TEXTPOINT__

struct textPoint
{
    GLfloat x;
    GLfloat y;
    GLfloat s;
    GLfloat t;
};
#endif




struct RefLine {
    RefLine() {clear();}
#ifdef __COMMENT__
    RefLine(long pIdx,long pEndIdx,float pStartX,float pStartY,float pMaxHeight, bool pTrunc)
    {
        StartIdx=pIdx;      /* idx for first UTexChar of the line */
        EndIdx=pIdx;        /* idx for last UTexChar of the line */
        StartPosX=pStartX;  /* relative position since box left */
        StartPosY=pStartY;  /* relative position since box top */
        MaxHeight=pMaxHeight; /* maximum character height for the line */
        Truncated= pTrunc;
    }
#endif
    RefLine (RefLine& pLine) {memmove(this,&pLine, sizeof(RefLine));}
    RefLine& operator =  (RefLine& pLine) {memmove(this,&pLine, sizeof(RefLine));}

    void clear() {memset(this,0,sizeof(RefLine));}

    long  StartIdx;       /* index of TextUChar the line is starting with */
    long  EndIdx;         /* index of TextUChar the line is Ending with */
    float TextWidthSize;  /* sum of characters horizontal advance for the text*/
    float TextHeightSize; /* sum of characters vertical advance for the text */
    float StartPosX;      /* position : 0 most othe cases */
    float StartPosY;      /* height position for the line */
    float MaxCharHeight;  /* maximum character Height for the line to be printed */
    float MaxCharWidth;   /* maximum character Height for the line to be printed */
    float MaxAdvanceY;    /* maximum Advance position height for the line to be printed : horizontal display */
    float MaxAdvanceX;    /* maximum Advance position width for the line to be printed : vertical display */
    bool  Truncated;      /* is line being truncated or not */
} ;


/**
 * The atlas struct holds a texture that contains the visible UNICODE characters
 * of a certain font rendered with a certain character height.
 * It also contains an array that contains all the information necessary to
 * generate the appropriate vertex and texture coordinates for each character.
 *
 * After the constructor is run, you don't need to use any FreeType functions anymore.
 */
class ZGLUnicodeText
{
private:
    void _cloneFrom(ZGLUnicodeText&pIn);


public:
    ZGLUnicodeText()=delete;
 //   GLUnicodeText(const utf32_t* pText,const char* pFont, unsigned int pFontSize, GLenum pTextureEngine);
    ZGLUnicodeText(ZGLUnicodeText&& pIn) {_cloneFrom(pIn);}

    ZGLUnicodeText(ZGLTextWriter* pWriter, GLenum pTextureEngine);
    ZGLUnicodeText& operator = (ZGLUnicodeText& pIn) {_cloneFrom(pIn); return *this;}

    ~ZGLUnicodeText();


    void clear ();

    void setBox (int pBoxWidth,
                 int pBoxHeight,
                 glm::vec3 pColor=glm::vec3(0.0f),
                 uint16_t pBoxFlag=RBP_Default,
                 bool pVisible=true,
                 float pLineSize=1.0,
                 int pRightMargin=1.0,
                 int pLeftMargin=1.0f,
                 int pTopMargin= 1.0f,
                 int pBottomMargin=1.0f); /* 1.0 is a minimum margin */




    void setPosition(float pX,float pY,float pZ) {Position=glm::vec3(pX,pY,pZ);}
    void setPosition(glm::vec3 pPosition) {Position=pPosition;}

    void setBoxBorderColor(glm::vec3 pColor) {Box->setBorderColor(pColor);}
    void setBoxBorderWidth (float pLineSize) {Box->setBorderWidth(pLineSize);}
    void setBoxGLDimensions (float pBoxWidth, float pBoxHeight) {Box->setGLDimensions(pBoxWidth,pBoxHeight);}
    void setBoxPixelDimensions (int pBoxWidth, int pBoxHeight) {Box->setPixelDimensions(pBoxWidth,pBoxHeight);}

    void setBoxMargins  (int pLeftMargin, int pRightMargin,int pTopMargin,int pBottomMargin)
                { Box->setMargins(pLeftMargin,pRightMargin,pTopMargin,pBottomMargin); }
    void setBoxMarginsAll(int pMargin) {Box->setMarginsAll(pMargin);}

    void setBoxVisible (bool pVisible) { Box->setVisible(pVisible);}

    void setBoxFillColor(glm::vec3 pColor) { Box->setFillColor(pColor);}
    void setBoxFill(bool pFill) {   Box->setFill(pFill);}
    void setBoxBorder(bool pBorder) { Box->setBorder(pBorder);}

    void setBoxFillAlpha(float pAlpha) {Box->setFillAlpha(pAlpha);}
    void setBoxBorderAlpha(float pAlpha) {Box->setBorderAlpha(pAlpha);}

    void printBoxShaderContext(const int pCtx,FILE*pOutput=stdout);

    void setTextFlag (uint32_t pFlag) {TextFlag=  pFlag;}
    void setBoxFlag (uint32_t pFlag) {Box->setFlag(pFlag);}

    uint16_t getBoxFlag() {return Box->Flag | TextFlag ;}

    int setBoxTextureByName(const char* pIntName)   {return Box->setTextureByName(pIntName);}
    int setBoxTextureByRank (const long pIdx)       {return Box->setTextureByRank(pIdx);}
    int changeBoxShaderByName(const int pCtx, const char* pIntName);
    int replaceBoxShaderByName(const int pCtx, const char* pIntName);
    int changeBoxShaderByRank(const int pCtx,const long pRank) ;
    int replaceBoxShaderByRank(const int pCtx,const long pRank) ;
    void setupGL();



/* text & text box rotation */
    void setModelRotation (float pAngle,glm::vec3 pAxis) {RotationAngle=pAngle;RotationAxis=pAxis;}

    void rotate90 () {RotationAngle = glm::radians(90.0f); RotationAxis= glm::vec3(0.0,0.0,1.0);}
    void rotate270 () {RotationAngle = glm::radians(270.0f); RotationAxis= glm::vec3(0.0,0.0,1.0);}

/* text setup and rendering */
    /**
     * @brief GLUnicodeText::setText prepares a unicode text (expressed in unicode codepoints) ready to be rendered by render() or renderVertical().
     *  This routine formats a texture whose texture engine has been set within UnicodeWriter object according glyphs from font given by pFontName.
     *  pFonName must point to a font that must have been loaded within object UnicodeWriter.
     *
     * @param pUtf32Text text to be rendered as UTF32 codepoint string
     * @param pFontName  internal registrated name for font
     * @param pFontSize  self explainatory : size of the font 12, 24, etc.
     */
    int setText(const utf32_t* pUtf32Text, const char* pFontName, size_t pFontSize);
    /**
     * @brief GLUnicodeText::setText prepares a unicode text (expressed in unicode codepoints) ready to be rendered by render() or renderVertical().
     *  This routine formats a texture whose texture engine has been set within UnicodeWriter object according glyphs from font given by pFontName.
     *  pFonName must point to a font that must have been loaded within object UnicodeWriter.
     *
     * @param pUtf32Text text to be rendered as UTF32 codepoint string
     * @param pFontId    internal registrated id for font (quicker than search by internal name)
     * @param pFontSize  self explainatory : size of the font 12, 24, etc.
     */
    int setText(const utf32_t* pUtf32Text,const int pFontId, size_t pFontSize);




    void render(glm::vec3 pPosition,
                glm::vec3 pColor);

    void renderVertical(glm::vec3 pPosition,
                        glm::vec3 pColor);

    void renderToBox(glm::vec3 pTextColor);
    void renderToBoxVertical(glm::vec3 pTextColor);


    void printLastError(FILE* pOuput=stderr){fprintf (pOuput,"%s\n",LastError);}
    char* getLastError () {return LastError;}
    void clearLastError() {if (LastError)
                                free(LastError);
                            LastError=nullptr;}


private:

    int _setText(const utf32_t* pUtf32Text);

/*
    void _boxSetupGLShape   ();
    void _boxSetupGLFill ();
    void _boxSetupGLFill3D ();
*/
    void _setupMatrices ();

    void _drawBoxShape ();
    void _drawBoxBackground ();



    void _render(glm::vec3 pPosition,
                 glm::vec3 pColor,
                 float pSx, float pSy);

    void _renderToBox(glm::vec3 pBoxPosition,
                      float     pBoxWidth,
                      float     pBoxHeight,
                      glm::vec3 pColor,
                      float pSx, float pSy,
                      uint32_t pTextFlag=RBP_Default); /* flag : text appearance and positionning */

    void _renderToBoxVertical(glm::vec3 pTextColor, float pSx, float pSy, uint16_t pTextFlag=RBP_Default);



    void _renderVertical(glm::vec3 pPosition,
                         glm::vec3 pColor,
                         float pSx, float pSy);

/* per character */

    inline int loadChar(FT_ULong pChar);

    int _storeOneChar(utf32_t pChar,
                      int &offsetX,int &offsetY,
                      unsigned int &wCurrentWidth,unsigned int &wCurrentHeight);

    void _setupOneChar(float &wStartPosX,                /* starting position x updated to next character position */
                       float wStartPosY,                 /* starting position y : not updated */
                       float wSx,
                       float wSy,
                       ZGLUnicodeChar* pChar,              /* character data content */
                       zbs::ZArray<textPoint>& wCoords);  /* array point coords table to draw characters */

    void _setupOneCharVertical(float wStartPosX,         /* starting position x : not updated  */
                       float& wStartPosY,                /* starting position y updated to next character position  */
                       float wSx,
                       float wSy,
                       ZGLUnicodeChar* pChar,              /* character data content */
                       zbs::ZArray<textPoint>& wCoords);  /* array point coords table to draw characters */

    void _setupOneCharV90(float &wStartPosX,                 /* starting position x updated to next character position */
                       float wStartPosY,                 /* starting position y : not updated */
                       float wSx,
                       float wSy,
                       ZGLUnicodeChar* pChar,              /* character data content */
                       zbs::ZArray<textPoint>& wCoords);  /* array point coords table to draw characters */

    inline void _textsetUpGLState(glm::vec3 pColor);

    inline void _textpostGL();


    ZGLTextWriter* Writer=nullptr;

    unsigned int TexSumWidth=0;     // texture width in pixels
    unsigned int TexSumHeight=0;	// texture height in pixels

    long FontHeight=0;

    FT_Int MaxWidth=0;      // Maximum width for a font character
    FT_Int MaxHeight=0;     // Maximum height to a font character


    zbs::ZArray<ZGLUnicodeChar*> UTexChar;

    const utf32_t* OriginText;

    // Render state
    GLuint VAO=0, VBO=0;
    ZTextTexture* TextTexture=nullptr;  /* One texture object for all text          */

/* Text Box */
    // Render state
 //   GLuint BoxVAOShape=0, BoxVBOShape=0 , BoxVAOFill=0, BoxVBOFill=0 ,BoxVBOTexture=0 ;
 //   ZTexture* BoxTexture=nullptr;  /* unused for the time being        */
 //   float BoxAlpha=1.0f;

/* End Text Box */

    size_t TextLen=0;           /* number of characters composing text      */

    size_t TextSumWidth =0;     /* sum of glyphs raw width for the whole text   */

    ZGLUnicodeFont* Font=nullptr;

    glm::vec3 Position=glm::vec3(0.0,0.0,0.0);
public:
    glm::mat4   Model; /* model matrix for text and text box if any */
    glm::mat4   Projection; /* Projection matrix for text and text box if any */
    glm::mat4   View; /* View matrix for text and text box if any */
    glm::mat4   Normal; /* Normal matrix for text and text box if any */
private:
    float RotationAngle=0.0;
    glm::vec3 RotationAxis=glm::vec3(0.0);


private:
    char*   LastError=nullptr;

/* text box */

    ZTextBox* Box=nullptr;
/*    uint16_t BoxFlag=RBP_Default;
    float BoxWidth=-1.0;
    float BoxHeight=-1.0;
    float BoxRightMargin    = 0.0;
    float BoxLeftMargin     = 0.0;
    float BoxTopMargin      =0.0;
    float BoxBottomMargin   = 0.0;

    int IBoxWidth=-1;
    int IBoxHeight=-1;

    float BoxLineSize=-1.0;


    glm::vec3 BoxLineColor=ZBlueColor;
    glm::vec3 BoxFillColor=ZBlueColor;
    */
/* end text box */


    float TextAdvanceWhenFilled = 0.001f;

    GLboolean   BlendEnabled=false;
    int         TextCoordsAttLocation;

    float StdMaxAdvanceY=0;    /* standard vertical advance */
    float StdMaxAdvanceX=0;    /* standard minimal horizontal advance */
    float StdMinAdvanceY=0;    /* standard vertical advance */
    float StdMinAdvanceX=0;    /* standard minimal horizontal advance */
    //zbs::ZArray<glm::vec3>* TextBoxCoords=nullptr;
//    float TextBoxcoords[];

    uint32_t TextFlag;

};//GLUnicodeText

void _printTextBoxFlag (uint16_t pBoxFlag,FILE* pOutput=stdout);

#endif // ZGLUNICODETEXT_H
