/*******************************************************************
** This code is part of Breakout.
**
** Breakout is free software: you can redistribute it and/or modify
** it under the terms of the CC BY 4.0 license as published by
** Creative Commons, either version 4 of the License, or (at your
** option) any later version.
******************************************************************/
#ifndef ZGLUNICODE_H
#define ZGLUNICODE_H
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



//#include <map>

#ifdef __USE_GLAD__
#include <glad/glad.h>
#else
#include <GL/glew.h>
#endif

#include <glm/glm.hpp>

#include <zglconstants.h>

#include <ztexture.h>
#include <zshader.h>

#include <stdlib.h>  // for malloc, memset
#include <stdint.h>  // for uintxx_t


#include <ztoolset/utfutils.h>

#include <ft2build.h>
#include FT_FREETYPE_H


// Maximum texture width
#define __TEX_MAXWIDTH__ 1024
// Maximum size for an internal font name
#define __FONTNAME_MAX__ 1024


/* documentation : https://en.wikibooks.org/wiki/OpenGL_Programming/Modern_OpenGL_Tutorial_Text_Rendering_01 */

class ZTexture;
/// Holds all state information relevant to a character as loaded using FreeType

#ifndef __BMSTRUCT__
#define __BMSTRUCT__
struct bmstruct{
    float width;    // bitmap width
    float height;   // bitmap height
    float left;     // bearing left : to draw character must position bearing <top,left>
    float top;      // bearing top
};

#endif

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


class GLUnicodeChar
{
private:
    void _cloneFrom(GLUnicodeChar& pIn)
    {
        clear();
        if (pIn.bitmapBuffer!=nullptr)
                    bitmapBuffer=(uint8_t*)malloc(pIn.bitmapBufferSize);

        bitmapBufferSize=pIn.bitmapBufferSize;

        bitmap=pIn.bitmap;
        Advance=pIn.Advance;
        Coef=pIn.Coef;

        xMin=pIn.xMin;
        yMin=pIn.yMin;
        xMax=pIn.xMax;
        yMax=pIn.yMax;

        texX=pIn.texX;
        texY=pIn.texY;
        texSize=pIn.texSize;
    }
public:
    GLUnicodeChar()
    {
        clear();
    }

    GLUnicodeChar(GLUnicodeChar& pIn)
    {
    _cloneFrom(pIn);
    }
    GLUnicodeChar(GLUnicodeChar* pIn)
    {
    _cloneFrom(*pIn);
    }
    GLUnicodeChar& operator = (GLUnicodeChar& pIn) {_cloneFrom(pIn); return *this;}

    ~GLUnicodeChar()
    {
        if (bitmapBuffer!=nullptr)
                free(bitmapBuffer);
    }

    void clear()
    {
        if (bitmapBuffer!=nullptr)
                free(bitmapBuffer);
        memset(this,0,sizeof(GLUnicodeChar));
    }

/*    inline bool isCuttingChar(const utf32_t* pCuttingCharList)
    {
        return (utfStrchr<utf32_t>(pCuttingCharList,Codepoint)!=nullptr);
    }
*/
//    float advanceX;	// advance.x
//    float advanceY;	// advance.y
/* bitmap information :
 * width,height : bitmap size
 * left,top : bearing : Offset from baseline to left/top of glyph
 */
    bmstruct bitmap;

    glm::ivec2 Advance; // Horizontal (x) or Vertical (y) offset to advance to next glyph

    double     Coef;    // conversion ratio from generic glyph measurement to bitmap metrics
                        // is equal to (double)wFace->glyph->advance.x/(double)wFace->glyph->linearHoriAdvance
    long     xMin, yMin, xMax ,yMax ; // BBox

    float texX;	// x offset of glyph in texture coordinates
    float texY;	// y offset of glyph in texture coordinates
    float texSize; // bitmap size within texture

    size_t      bitmapBufferSize=0;
    uint8_t*    bitmapBuffer=nullptr;

    bool        isCuttingChar=false;
    bool        isNewLineChar=false;

    utf32_t     Codepoint;
} ;//GLUnicodeChar




class GLUnicodeText;

// A renderer class for rendering text displayed by a font loaded using the 
// FreeType library. A single font is loaded, processed into a list of Character
// items for later rendering.

/** For all dependent GLTextWriter generated, this object holds
 *   shader (generated)
 *   texture engine
 *   RFFU : matrices (view, model, projection)
 *
 *
 *  holds GLUnicodeText objects list and deletes it when GLFontWriter object is destroyed.
 *
 */
class GLTextWriter
{
public:


    // Constructor
    GLTextWriter(GLenum pTexture=GL_TEXTURE0);

    ~GLTextWriter();

    GLUnicodeText* newText();

    // Shader used for text rendering
    ZShader* TextShader=nullptr;
    /* shader used for text boxes rendering */
    ZShader* BoxShader=nullptr;

    void setModel(glm::mat4 pModel) {Model=pModel;}
    glm::mat4  getModel() {return Model;}

    void setView(glm::mat4 pView) {View=pView;}
    glm::mat4  getView() {return View;}

    void setProjection(glm::mat4 pProjection) {Projection=pProjection;}
    glm::mat4  getProjection() {return Projection;}

    int updateShader()
    {

    }

    ZShader* getBoxShader()
    {
        if (BoxShader==nullptr)
                    _newBoxShader();
        return BoxShader;
    }

    void _newBoxShader()
    {
        BoxShader=new  ZShader("ztextbox.vs", "ztextbox.fs", "BoxShader");
    }
    glm::mat4 Model;
    glm::mat4 View;
    glm::mat4 Projection;

private:
    GLenum      TextureEngine=GL_TEXTURE0;

    zbs::ZArray<GLUnicodeText*> TextList;

}; // GLTextWriter


struct RefLine {
    RefLine() {clear();}
    RefLine(long pIdx,long pEndIdx,float pStartX,float pStartY,float pMaxHeight, bool pTrunc)
    {
        StartIdx=pIdx;      /* idx for first UTexChar of the line */
        EndIdx=pIdx;        /* idx for last UTexChar of the line */
        StartPosX=pStartX;  /* relative position since box left */
        StartPosY=pStartY;  /* relative position since box top */
        MaxHeight=pMaxHeight; /* maximum character height for the line */
        Truncated= pTrunc;
    }

    RefLine (RefLine& pLine) {memmove(this,&pLine, sizeof(RefLine));}
    RefLine& operator =  (RefLine& pLine) {memmove(this,&pLine, sizeof(RefLine));}

    void clear() {memset(this,0,sizeof(RefLine));}

    long  StartIdx;     /* index of TextUChar the line is starting with */
    long  EndIdx;       /* index of TextUChar the line is Ending with */
    float TextSize;     /* sum of characters horizontal advance for the text */
    float StartPosX;    /* position : 0 most othe cases */
    float StartPosY;    /* height position for the line */
    float MaxHeight;    /* maximum character Height for the line to be printed */
    float MaxAdvanceY;   /* maximum Advance position height for the line to be printed */
    bool  Truncated;    /* is line being truncated or not */
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
    void _cloneFrom(GLUnicodeText&pIn)
    {
//        TexCode=pIn.TexCode;
        if (Texture!=nullptr)
                delete Texture;
        Texture = new ZTexture(pIn.Texture);
        TexSumWidth=pIn.TexSumWidth;
        TexSumHeight=pIn.TexSumHeight;
        Font=pIn.Font;
        Writer=pIn.Writer;
        for (long wi=0;wi<pIn.UTexChar.count();wi++)
            UTexChar.push_back(new GLUnicodeChar(*pIn.UTexChar[wi]));

        FontHeight=pIn.FontHeight;
        MaxBearingH=pIn.MaxBearingH;
        MaxBearingW=pIn.MaxBearingW;
        MaxWidth=pIn.MaxWidth;
        MaxHeight=pIn.MaxHeight;
    }

public:
    GLUnicodeText()=delete;
 //   GLUnicodeText(const utf32_t* pText,const char* pFont, unsigned int pFontSize, GLenum pTextureEngine);
    GLUnicodeText(GLUnicodeText&& pIn) {_cloneFrom(pIn);}

    GLUnicodeText(GLTextWriter* pWriter, GLenum pTextureEngine);


    GLUnicodeText& operator = (GLUnicodeText& pIn) {_cloneFrom(pIn); return *this;}

    ~GLUnicodeText()
        {
//        FT_Done_Face(Face);
        delete Texture;
        while (UTexChar.count())
                delete UTexChar.popR();

        if (VBO)
                glDeleteBuffers(1 ,&VBO);
        if (VAO)
                glDeleteVertexArrays(1 ,&VAO);

        if (BoxVBO)
                glDeleteBuffers(1 ,&BoxVBO);
        if (BoxVAO)
                glDeleteVertexArrays(1 ,&BoxVAO);
        }

    void clear ()
    {
//        TexCode=0;
        if (Texture!=nullptr)
                delete Texture;
        Texture=nullptr;

        TexSumWidth=0;      // total width for texture including all face characters
        TexSumHeight=0;     // total height for texture including all face characters
        Font=nullptr;
        Writer=nullptr;
        FontHeight=0;
        MaxBearingH=0;
        MaxBearingW=0;
        MaxWidth=0;
        MaxHeight=0;

        while (UTexChar.count())
                delete UTexChar.popR();
    }

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

    void _render(glm::vec3 pPosition,
                 glm::vec3 pColor,
                 float pSx, float pSy);

    void _renderToBox(glm::vec3 pBoxPosition,
                      float     pBoxWidth,
                      float     pBoxHeight,
                      glm::vec3 pColor,
                      float pSx, float pSy,
                      uint16_t pFlag=RBP_Default); /* flag : how to position text within the box */

    void _renderVertical(glm::vec3 pPosition,
                         glm::vec3 pColor,
                         float pSx, float pSy);

    void _renderVerticalToBox(glm::vec3 pBoxPosition,
                      int       pBoxWidth,
                      int       pBoxHeight,
                      glm::vec3 pColor,
                      float pSx, float pSy);


    void _setupOneChar(float &wStartPosX,                 /* starting position x updated to next character position */
                       float wStartPosY,                 /* starting position y : not updated */
                       float wSx,
                       float wSy,
                       GLUnicodeChar* pChar,              /* character data content */
                       zbs::ZArray<textPoint>& wCoords);  /* array point coords table to draw characters */

    void _setupOneChar_1(float &wStartPosX,                 /* starting position x updated to next character position */
                       float wStartPosY,                 /* starting position y : not updated */
                       float wSx,
                       float wSy,
                       GLUnicodeChar* pChar,              /* character data content */
                       zbs::ZArray<textPoint>& wCoords);  /* array point coords table to draw characters */

    GLTextWriter* Writer=nullptr;

    unsigned int TexSumWidth=0;	// texture width in pixels
    unsigned int TexSumHeight=0;	// texture height in pixels

    long FontHeight=0;
    FT_Int MaxBearingH=0;   // Maximum bearing high (to top of the glyph)
    FT_Int MaxBearingW=0;   // Maximum bearing width (to left of the glyph)

    FT_Int MaxWidth=0;      // Maximum width for a font character
    FT_Int MaxHeight=0;     // Maximum height to a font character


    zbs::ZArray<GLUnicodeChar*> UTexChar;

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

    UnicodeFont* Font=nullptr;

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

    inline void _setUpGLState(glm::vec3 pPosition,
                       glm::vec3 pColor);

    inline void _postGL();

    GLboolean   BlendEnabled=false;
    int         TextCoordsAttLocation;

    float StdLineAdvanceY=0;    /* standard vertical advance */
    float StdLineAdvanceX=0;    /* standard minimal horizontal advance */

    //zbs::ZArray<glm::vec3>* TextBoxCoords=nullptr;
    float TextBoxcoords[];

};//GLUnicodeText


#endif //ZGLUNICODE_H
