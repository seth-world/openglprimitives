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



class ZTexture;
class ZGLTextWriter;
class ZGLUnicodeChar;
class ZGLUnicodeFont;



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


enum RBoxPos : uint16_t
{
    RBP_Nothing         = 0,
    RBP_HorizCenter     = 0x01, /* horizontally centered */
    RBP_VertCenter      = 0x02, /* vertically centered */
    RBP_LeftJust        = 0x04, /* text is horizontally left justified (default) */
    RBP_RightJust       = 0x08, /* text is horizontally right justified */
    RBP_TopJust         = 0x10, /* text is vertically display starting at top of box (default) */
    RBP_BotJust         = 0x20, /* text is vertically displayed to box bottom */

    RBP_LineWrap        = 0x40, /* Text is wrapped horizontally if it does not fit into box boundary (default)*/
    RBP_WordWrap        = 0x80, /* Text is wrapped horizontally if it does not fit into box boundary (default)*/
    RBP_TruncChar       = 0x0100, /* Displays a truncate sign at the end of the truncated line */
//    RBP_FitVertical     = 0x80,          /* Text should fit into vertical box boundary  */

    RBP_AdjustFSize     = 0x0200,/* Text should fit as it is adjusting font size if necessary :
                                    This option must be set only if RBP_Wrap is not set
                                    to make text with fit into box maximum width */

//    RBP_AdjustBestTry   = 0x0200,       /* Text size is being adjusted if it does not fit vertically after being cut (default) */

    RBP_Default         = RBP_LeftJust | RBP_TopJust | RBP_LineWrap
};



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
class GLUnicodeText
{
private:
    void _cloneFrom(GLUnicodeText&pIn);


public:
    GLUnicodeText()=delete;
 //   GLUnicodeText(const utf32_t* pText,const char* pFont, unsigned int pFontSize, GLenum pTextureEngine);
    GLUnicodeText(GLUnicodeText&& pIn) {_cloneFrom(pIn);}

    GLUnicodeText(ZGLTextWriter* pWriter, GLenum pTextureEngine);
    GLUnicodeText& operator = (GLUnicodeText& pIn) {_cloneFrom(pIn); return *this;}

    ~GLUnicodeText();


    void clear ();

    void setBox (float pBoxWidth,
                 float pBoxHeight,
                 glm::vec3 pColor=glm::vec3(0.0f),
                 uint16_t pBoxFlag=RBP_Default,
                 bool pVisible=false,
                 float pLineSize=-1.0,
                 float pRightMargin=1.0,
                 float pLeftMargin=1.0f,
                 float pTopMargin= 1.0f,
                 float pBottomMargin=1.0f); /* 1.0 is a minimum margin */


    void setPosition(float pX,float pY,float pZ) {Position=glm::vec3(pX,pY,pZ);}
    void setPosition(glm::vec3 pPosition) {Position=pPosition;}

    void setBoxColor(glm::vec3 pColor) {BoxColor=pColor;}
    void setBoxLineSize (float pLineSize) {BoxLineSize=pLineSize;}

    void _boxSetupGL ();

    void _setupMatrices ();

    void _drawBox ();


    inline int loadChar(FT_ULong pChar);

    int setText(const utf32_t* pUtf32Text, const char* pFontName, size_t pFontSize);

    int setText(const utf32_t* pUtf32Text,const int pFontIdx);

    void render(glm::vec3 pPosition,
                glm::vec3 pColor);

    void renderVertical(glm::vec3 pPosition,
                        glm::vec3 pColor);

    void renderToBox(glm::vec3 pTextColor);
    void renderToBoxVertical(glm::vec3 pTextColor);



    void _render(glm::vec3 pPosition,
                 glm::vec3 pColor,
                 float pSx, float pSy);

    void _renderToBox(glm::vec3 pBoxPosition,
                      float     pBoxWidth,
                      float     pBoxHeight,
                      glm::vec3 pColor,
                      float pSx, float pSy,
                      uint16_t pFlag=RBP_Default); /* flag : how to position text within the box */

    void _renderToBoxVertical(glm::vec3 pBoxPosition,
                              float     pBoxWidth,
                              float     pBoxHeight,
                              glm::vec3 pColor,
                              float pSx, float pSy,
                              uint16_t pFlag=RBP_Default); /* flag : how to position text within the box */


    void _renderVertical(glm::vec3 pPosition,
                         glm::vec3 pColor,
                         float pSx, float pSy);



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

    ZGLTextWriter* Writer=nullptr;

    unsigned int TexSumWidth=0;	// texture width in pixels
    unsigned int TexSumHeight=0;	// texture height in pixels

    long FontHeight=0;
    FT_Int MaxBearingH=0;   // Maximum bearing high (to top of the glyph)
    FT_Int MaxBearingW=0;   // Maximum bearing width (to left of the glyph)

    FT_Int MaxWidth=0;      // Maximum width for a font character
    FT_Int MaxHeight=0;     // Maximum height to a font character


    zbs::ZArray<ZGLUnicodeChar*> UTexChar;

    const utf32_t* OriginText;

    // Render state
    GLuint VAO=0, VBO=0;
    ZTexture* Texture=nullptr;  /* One texture object for all text          */

/* Text Box */
    // Render state
    GLuint BoxVAO=0, BoxVBO=0;

    ZTexture* BoxTexture=nullptr;  /* unused for the time being        */

/* End Text Box */

    size_t TextLen=0;           /* number of characters composing text      */

    size_t TextSumWidth =0;     /* sum of glyphs raw width for the whole text   */

    ZGLUnicodeFont* Font=nullptr;

    glm::vec3 Position=glm::vec3(0.0,0.0,0.0);

    glm::mat4   Model; /* model matrix for text and text box if any */
    glm::mat4   Projection; /* model matrix for text and text box if any */
    glm::mat4   View; /* model matrix for text and text box if any */

private:
/* text box */
    uint16_t BoxFlag=RBP_Default;
    float BoxWidth=-1.0;
    float BoxHeight=-1.0;
    float BoxRightMargin    = 0.0;
    float BoxLeftMargin     = 0.0;
    float BoxTopMargin      =0.0;
    float BoxBottomMargin   = 0.0;

    int IBoxWidth=-1;
    int IBoxHeight=-1;

    bool  BoxVisible=false;
    float BoxLineSize=-1.0;
    glm::vec3 BoxColor=ZBlueColor;
/* end text box */

    inline void _setUpGLState(glm::vec3 pColor);

    inline void _postGL();

    GLboolean   BlendEnabled=false;
    int         TextCoordsAttLocation;

    float StdMaxAdvanceY=0;    /* standard vertical advance */
    float StdMaxAdvanceX=0;    /* standard minimal horizontal advance */
    float StdMinAdvanceY=0;    /* standard vertical advance */
    float StdMinAdvanceX=0;    /* standard minimal horizontal advance */
    //zbs::ZArray<glm::vec3>* TextBoxCoords=nullptr;
    float TextBoxcoords[];

};//GLUnicodeText


#endif // ZGLUNICODETEXT_H
