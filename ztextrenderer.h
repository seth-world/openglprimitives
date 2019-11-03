/*******************************************************************
** This code is part of Breakout.
**
** Breakout is free software: you can redistribute it and/or modify
** it under the terms of the CC BY 4.0 license as published by
** Creative Commons, either version 4 of the License, or (at your
** option) any later version.
******************************************************************/
#ifndef ZTEXT_RENDERER_H
#define ZTEXT_RENDERER_H

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

struct CharAtlas_struct {
    CharAtlas_struct() {memset(this,0,sizeof(CharAtlas_struct));}

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
} ;


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

/**
 * The atlas struct holds a texture that contains the visible US-ASCII characters
 * of a certain font rendered with a certain character height.
 * It also contains an array that contains all the information necessary to
 * generate the appropriate vertex and texture coordinates for each character.
 *
 * After the constructor is run, you don't need to use any FreeType functions anymore.
 */
class FontAtlas
{
private:
    void _cloneFrom(FontAtlas&pIn) {
//        TexCode=pIn.TexCode;
        if (Texture!=nullptr)
                delete Texture;
        Texture = new ZTexture(*pIn.Texture);
        TexSumWidth=pIn.TexSumWidth;
        TexSumHeight=pIn.TexSumHeight;
        Name=pIn.Name;

        memmove(CharAtlas , pIn.CharAtlas, sizeof(CharAtlas));
    }

public:
    FontAtlas()=delete;

    FontAtlas(const char* pFont, unsigned int pFontSize, const char* pName, GLenum pTextureEngine);
    FontAtlas(FontAtlas&& pIn) {_cloneFrom(pIn);}
    FontAtlas& operator = (FontAtlas& pIn) {_cloneFrom(pIn); return *this;}

    ~FontAtlas() {
//        glDeleteTextures(1, &TexCode);
        delete Texture;
    }
    void clear ()
    {
//        TexCode=0;
        if (Texture!=nullptr)
                delete Texture;
        Texture=nullptr;

        TexSumWidth=0;      // total width for texture including all face characters
        TexSumHeight=0;     // total height for texture including all face characters
        memset(CharAtlas,0,sizeof(CharAtlas));
        Name=nullptr;
        FontHeight=0;
        MaxBearingH=0;
        MaxBearingW=0;
        MaxWidth=0;
        MaxHeight=0;
    }

    void AtlasUSACII(FT_UInt pFontSize, GLenum pTextureEngine);
    void AtlasUnicode(FT_UInt pFontSize, GLenum pTextureEngine);

//    GLuint TexCode;		// texture object
    ZTexture* Texture=nullptr;  // One texture for all characters of a font face (Atlas)
    unsigned int TexSumWidth=0;	// texture width in pixels
    unsigned int TexSumHeight=0;	// texture height in pixels

    long FontHeight=0;
    FT_Int MaxBearingH=0;   // Maximum bearing high (to top of the glyph)
    FT_Int MaxBearingW=0;   // Maximum bearing width (to left of the glyph)

    FT_Int MaxWidth=0;      // Maximum width for a font character
    FT_Int MaxHeight=0;     // Maximum height to a font character


    const char* Name=nullptr; /*internal name :
                                fonts may be retreived eiher by this name or by its index within FontList */

    CharAtlas_struct CharAtlas[128];  /* indice 0 to 31 are not used and wasted : could be optimized */

    FT_Face Face;
    bool    FaceUSASCII=true;
};


// A renderer class for rendering text displayed by a font loaded using the 
// FreeType library. A single font is loaded, processed into a list of Character
// items for later rendering.
class ZTextRenderer
{
public:

    // Shader used for text rendering
    ZShader* TextShader;
    // Constructor
    ZTextRenderer(GLenum pTexture);
    void initAtlas();
    void _renderUSASCII(const char *text, float x, float y, float sx, float sy);

    /** adds a new font to font atlas :
     *  pFontPath : file name which will be searched within default font path
     *  pFontsize : height in pixels of the font
     *  pName : internal name wich could be used for retreiving font within font atlas
     * ( in place of index )
     * Returns font index
     */
    long addFont(const char* pFontPath,const int pFontSize,const char* pName)
    {
        FontAtlas* wFont=new FontAtlas(pFontPath,pFontSize,pName,TextureEngine);
        return FontList.push(wFont);
    }

    long getFontIndex(const char*pName);

    void _render(const char *pText,
                    glm::vec3 pPosition,
                    float pSx, float pSy,
                    glm::vec3 pColor,
                    long  pFontIndex);

    void _renderVertical(const char *pText,
                         glm::vec3 pPosition,
                         float pSx, float pSy,
                         glm::vec3 pColor,
                         long  pFontIndex);

    void render(const char *pText,
                    glm::vec3 pPosition,
                    glm::vec3 pColor,
                    long  pFontIndex);

    void renderVertical(const char *pText,
                glm::vec3 pPosition,
                glm::vec3 pColor,
                long  pFontIndex);

    void renderScaled(const char *pText,
                      glm::vec3 pPosition,
                      const Color_type pColor,
                      float pScale,
                      long  pFontIndex);


    void renderByName(const char *pText,
                      glm::vec3 pPosition,
                      glm::vec3 pColor,
                      const char* pName);

    void renderVerticalByName(const char *pText,
                              glm::vec3 pPosition,
                              glm::vec3 pColor,
                              const char* pName);

    void renderScaledByName(const char *pText,
                            glm::vec3 pPosition,
                            glm::vec3 pColor,
                            float pScale,
                            const char* pName);


    zbs::ZArray<FontAtlas*> FontList;

private:
    // Render state
    GLuint VAO, VBO;
    ZTexture*   Texture=nullptr;
    GLenum      TextureEngine=GL_TEXTURE0;
    GLuint rawvbo;
//    FT_Library rawft;
//    FT_Face rawface;
//    FT_GlyphSlot GlyphSlot;
//    int atlas_width;



//    void atlas_create(FT_Face face, int height);


};


#endif //ZTEXT_RENDERER_H
