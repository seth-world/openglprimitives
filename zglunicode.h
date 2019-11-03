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

#include <map>

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

class UnicodeFont
{
private:
    void _copyFrom(UnicodeFont&pIn)
    {
        Name=pIn.Name;
        Face=pIn.Face;
        FontSize=pIn.FontSize;
        MemResident=pIn.MemResident;
    }
public:
    UnicodeFont()=default;

    UnicodeFont(UnicodeFont&pIn) {}

    UnicodeFont& operator = (UnicodeFont& pIn) {_copyFrom(pIn); return *this;}

    UnicodeFont(const char* pFontPath,unsigned int pFontSize,const char* pName)
    {
        if (load(pFontPath,pFontSize,pName)<0)
            abort();
    }

    ~UnicodeFont() {FT_Done_Face(Face);}

    long   load(const char* pFontPath,unsigned int pFontSize,const char* pName);

const char* Name=nullptr; /*internal name :
                            fonts may be retreived eiher by this name or by its index within FontList */
FT_Face Face;
FT_UInt FontSize;
bool    MemResident=false;
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
} ;//GLUnicode_struct


#ifdef __COMMENT__

class GLUnicodeWriter
{
private:
    void _cloneFrom(GLUnicodeWriter&pIn)
    {

//        TexCode=pIn.TexCode;
        if (Texture!=nullptr)
                delete Texture;
        Texture = new ZTexture(*pIn.Texture);
        TexSumWidth=pIn.TexSumWidth;
        TexSumHeight=pIn.TexSumHeight;
        Name=pIn.Name;

        for (long wi=0;wi<pIn.Text.count();wi++)
        {
        Text.push_back(new GLUnicodeChar_struct(pIn.Text[wi]));
        }
    }//_cloneFrom

public:
    GLUnicodeWriter()=delete;

    GLUnicodeWriter(GLenum pTextureEngine);
    GLUnicodeWriter(GLUnicodeWriter&& pIn) {_cloneFrom(pIn);}
    GLUnicodeWriter& operator = (GLUnicodeWriter& pIn) {_cloneFrom(pIn); return *this;}

    ~GLUnicodeWriter()
        {

        while (Text.count())
                delete Text.popR();
        }
    void clear ()
    {
//        TexCode=0;
        if (Texture!=nullptr)
                delete Texture;
        Texture=nullptr;

        TexSumWidth=0;      // total width for texture including all face characters
        TexSumHeight=0;     // total height for texture including all face characters
        while (Text.count())
                delete Text.popR();
        Name=nullptr;
        FontHeight=0;
        MaxBearingH=0;
        MaxBearingW=0;
        MaxWidth=0;
        MaxHeight=0;
    }

    void AtlasUnicode(FT_UInt pFontSize, GLenum pTextureEngine);

//    GLuint TexCode;		// texture object

    ZShader*    TextShader=nullptr; /* one shader for all text */

    zbs::ZArray<Fontstruct> Fonts;


    unsigned int TexSumWidth=0;	// texture width in pixels
    unsigned int TexSumHeight=0;	// texture height in pixels

    long FontHeight=0;
    FT_Int MaxBearingH=0;   // Maximum bearing high (to top of the glyph)
    FT_Int MaxBearingW=0;   // Maximum bearing width (to left of the glyph)

    FT_Int MaxWidth=0;      // Maximum width for a font character
    FT_Int MaxHeight=0;     // Maximum height to a font character




 };//UnicodeAtlas
#endif // __COMMENT__

class GLUnicodeText;

// A renderer class for rendering text displayed by a font loaded using the 
// FreeType library. A single font is loaded, processed into a list of Character
// items for later rendering.
class GLUnicodeWriter
{
public:


    // Constructor
    GLUnicodeWriter(GLenum pTexture=GL_TEXTURE0);
    void initAtlas();

    /** adds a new font to font atlas :
     *  pFontPath : file name which will be searched within default font path
     *  pFontsize : height in pixels of the font
     *  pName : internal name wich could be used for retreiving font within font atlas
     * ( in place of index )
     * Returns font index
     */
    long addFont(const char* pFontPath,const unsigned int pFontSize,const char* pName)
    {
        UnicodeFont* wFont=new UnicodeFont;
        if (wFont->load(pFontPath,pFontSize,pName)<0)
                return -1;
        return FontList.push(wFont);
    }

    GLUnicodeText* newText();


    long getFontIndex(const char*pName);

    zbs::ZArray<UnicodeFont*> FontList;

    // Shader used for text rendering
    ZShader* TextShader;
private:

    GLenum      TextureEngine=GL_TEXTURE0;

};


/**
 * The atlas struct holds a texture that contains the visible US-ASCII characters
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

        FontHeight=pIn.FontHeight;;
        MaxBearingH=pIn.MaxBearingH;;
        MaxBearingW=pIn.MaxBearingW;;
        MaxWidth=pIn.MaxWidth;;
        MaxHeight=pIn.MaxHeight;;
    }

public:
    GLUnicodeText()=delete;

 //   GLUnicodeText(const utf32_t* pText,const char* pFont, unsigned int pFontSize, GLenum pTextureEngine);
    GLUnicodeText(GLUnicodeText&& pIn) {_cloneFrom(pIn);}

    GLUnicodeText(GLUnicodeWriter* pWriter, GLenum pTextureEngine);


    GLUnicodeText& operator = (GLUnicodeText& pIn) {_cloneFrom(pIn); return *this;}

    ~GLUnicodeText()
        {
//        FT_Done_Face(Face);
        delete Texture;
        while (UTexChar.count())
                delete UTexChar.popR();
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

    inline int loadChar(FT_ULong pChar);

    void setText(const utf32_t* pUtf32Text,const char* pFontName);

    void _render(glm::vec3 pPosition,
                 glm::vec3 pColor,
                 float pSx, float pSy);
    void _renderVertical(glm::vec3 pPosition,
                         glm::vec3 pColor,
                         float pSx, float pSy);
    void render(glm::vec3 pPosition,
                glm::vec3 pColor);

    void renderVertical(glm::vec3 pPosition,
                        glm::vec3 pColor);

    void AtlasUSACII(FT_UInt pFontSize, GLenum pTextureEngine);
    void AtlasUnicode(FT_UInt pFontSize, GLenum pTextureEngine);


    GLUnicodeWriter* Writer=nullptr;

    unsigned int TexSumWidth=0;	// texture width in pixels
    unsigned int TexSumHeight=0;	// texture height in pixels

    long FontHeight=0;
    FT_Int MaxBearingH=0;   // Maximum bearing high (to top of the glyph)
    FT_Int MaxBearingW=0;   // Maximum bearing width (to left of the glyph)

    FT_Int MaxWidth=0;      // Maximum width for a font character
    FT_Int MaxHeight=0;     // Maximum height to a font character


    zbs::ZArray<GLUnicodeChar*> UTexChar;

    const utf32_t* OriginText;
//    FT_UInt FontSize;
//    FT_Face Face;

    // Render state
    GLuint VAO=0, VBO=0;
    ZTexture* Texture=nullptr;// One texture for all text
    size_t TextLen=0;

    UnicodeFont* Font=nullptr;

};//GLUnicodeText


#endif //ZGLUNICODE_H
