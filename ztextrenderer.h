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




#include <ft2build.h>
#include FT_FREETYPE_H


// Maximum texture width
#define __TEX_MAXWIDTH__ 1024
// Maximum size for an internal font name
#define __FONTNAME_MAX__ 1024


/* documentation : https://en.wikibooks.org/wiki/OpenGL_Programming/Modern_OpenGL_Tutorial_Text_Rendering_01 */

class ZTexture;
/// Holds all state information relevant to a character as loaded using FreeType
/*
struct Character {
    GLuint TextureID;   // ID handle of the glyph texture
    ZTexture* Texture;
    glm::ivec2 Size;    // Size of glyph
    glm::ivec2 Bearing; // Offset from baseline to left/top of glyph
    GLuint Advance;     // Horizontal offset to advance to next glyph
};
*/


struct CharAtlas_struct {
    CharAtlas_struct() {memset(this,0,sizeof(CharAtlas_struct));}

    float advanceX;	// advance.x
    float advanceY;	// advance.y

    float bitmapWidth;	// bitmap.width;
    float bitmapHeight;	// bitmap.height;

    float bitmapLeft;	// bitmap_left;
    float bitmapTop;	// bitmap_top;

    float texX;	// x offset of glyph in texture coordinates
    float texY;	// y offset of glyph in texture coordinates
} ;

struct textPoint {
    GLfloat x;
    GLfloat y;
    GLfloat s;
    GLfloat t;
};

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
        TexWidth=pIn.TexWidth;
        TexHeight=pIn.TexHeight;
        Name=pIn.Name;

        memmove(CharAtlas , pIn.CharAtlas, sizeof(CharAtlas));
    }

public:
    FontAtlas()=delete;

    FontAtlas(const char* pFont, int pFontSize, const char* pName, GLenum pTextureEngine);
    FontAtlas(FontAtlas& pIn) {_cloneFrom(pIn);}
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
        TexWidth=0;
        TexHeight=0;
        memset(CharAtlas,0,sizeof(CharAtlas));
        Name=nullptr;
    }


//    GLuint TexCode;		// texture object
    ZTexture* Texture=nullptr;      // One texture for all characters of Atlas
    unsigned int TexWidth;			// texture width in pixels
    unsigned int TexHeight;			// texture height in pixels

    const char* Name=nullptr; /*internal name :
                                fonts may be retreived eiher by this name or by its index within FontList */

    CharAtlas_struct CharAtlas[128];  /* indice 0 to 31 are not used and wasted : could be optimized */


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
    void renderText(const char *text, float x, float y, float sx, float sy);

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

    void renderText(const char *pText,
                    float pX, float pY,
                    float pSx, float pSy,
                    glm::vec3 pColor,
                    long  pFontIndex);
    void renderTextByName(const char *pText,
                          float pX, float pY,
                          float pSx, float pSy,
                          glm::vec3 pColor,
                          const char* pName);

    void renderText(const char *pText,
                    float pX, float pY,
                    glm::vec3 pColor,
                    long  pFontIndex);

    void renderTextScaled(const char *pText,
                          float pX, float pY,
                          const Color_type pColor,
                          float pScale,
                          long  pFontIndex);


    void renderTextByName(const char *pText,
                          float pX, float pY,
                          glm::vec3 pColor,
                          const char* pName);

    void renderTextScaledByName(const char *pText,
                                float pX, float pY,
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
