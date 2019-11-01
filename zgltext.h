/*******************************************************************
** This code is part of Breakout.
**
** Breakout is free software: you can redistribute it and/or modify
** it under the terms of the CC BY 4.0 license as published by
** Creative Commons, either version 4 of the License, or (at your
** option) any later version.
******************************************************************/
#ifndef ZGLTEXT_H
#define ZGLTEXT_H

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

//class FT_Library;
//class FT_Face;
//class FT_GlyphSlot;

#include <ztoolset/zarray.h>
#include <initializer_list>

#ifndef __BMSTRUCT__
#define __BMSTRUCT__
struct bmstruct{
    float width;    // bitmap width
    float height;   // bitmap height
    float left;     // bearing left : to draw character must position bearing <top,left>
    float top;      // bearing top
};
#endif


class Character {
public:

    Character()=default;
    Character(ZTexture* pTexture,
              long pBitmapWidth,
              long pBitmapHeight,
              long pBearingX,
              long pBearingY,
              long pAdvanceX,
              long pAdvanceY,
              long pxMin,
              long pyMin,
              long pxMax,
              long pyMax,
              double pCoef)
    {
//        TextureID=pTexId;
        Texture=pTexture;
        bitmap.width=pBitmapWidth;
        bitmap.height=pBitmapHeight;

        bitmap.left=pBearingX;
        bitmap.top=pBearingY;

/*        Size.x=(int)pBitmapWidth;
        Size.y=(int)pBitmapHeight;
        Bearing.x=(int)pBearingX;
        Bearing.y=(int)pBearingY;
*/
        Advance.x=(int)pAdvanceX;
        Advance.y=(int)pAdvanceY;
        xMin=pxMin;
        xMax=pxMax;
        yMin=pyMin;
        yMax=pyMax;
        Coef=pCoef;
/* following is an obsolete fallback  */
        if (Advance.y==0)
        {
           Advance.y=(int)(yMax-yMin);
        }

    }
    Character(Character& pIn) {memmove (this,&pIn,sizeof(Character));}

//    GLuint TextureID;   // ID handle of the glyph texture
    ZTexture* Texture;
    /* bitmap information :
     * width,height : bitmap size
     * left,top : bearing : Offset from baseline to left/top of glyph
     */
    bmstruct bitmap;
//    glm::ivec2 Size;    // Size of glyph
//    glm::ivec2 Bearing; // Offset from baseline to left/top of glyph

    glm::ivec2 Advance; // Horizontal (x) or Vertical (y) offset to advance to next glyph

    double     Coef;    // conversion ratio from generic glyph measurement to bitmap metrics
                        // is equal to (double)wFace->glyph->advance.x/(double)wFace->glyph->linearHoriAdvance
    long     xMin, yMin, xMax ,yMax ; // BBox
    Character& operator = (const Character &pIn)
    {
        memmove (this,&pIn,sizeof(Character));
        return  *this;
    }
};

class ZFTFont
{
public:
    Character Tab[128]; /* character 0 to 31 are not used */
    void clear()
    {
        memset (Tab,0,sizeof(Tab));
        FontHeight=0;
        MaxBearingH=0;
        MaxBearingW=0;
        MaxWidth=0;
        MaxHeight=0;
    }
    long FontHeight=0;
    FT_Int MaxBearingH=0;
    FT_Int MaxBearingW=0;

    FT_Int MaxWidth=0;
    FT_Int MaxHeight=0;

    Character& operator [] (int pIdx)
        {
        if ((pIdx<32)||(pIdx>128))
                return Tab[__REPLACEMENT_CHAR__];
        return Tab[pIdx];
        }

};

// A renderer class for rendering text displayed by a font loaded using the 
// FreeType library. A single font is loaded, processed into a list of Character
// items for later rendering.

/* one texture per character
 * one glDraw call per character to print
 * [see ZTextRender : one texture per atlas one glDraw per whole text to print ]
*/

class ZGLText
{
public:
    // Holds a list of pre-compiled Characters
//    std::map<GLchar, Character> Characters;

    ZFTFont Characters;
    // Shader used for text rendering
    ZShader* TextShader;
    // Constructor
    ZGLText(GLenum pTexture=GL_TEXTURE0);

    // Renders a string of text using the precompiled list of characters
 /*   void RenderText(std::string pText,
                    GLfloat pPosX, GLfloat pPosY,
                    GLfloat pScale,
                    const Color_type pColor = Color_type(1.0f));
*/
    void render(std::string pText,
                glm::vec3 pPosition,
                GLfloat pScale,
                const Color_type pColor);

    void renderVertical(std::string pText,
                        glm::vec3 pPosition,
                        GLfloat pScale,
                        const Color_type pColor);

    /** adds a new font
     *  pFontPath : file name which will be searched within default font path
     *  pFontsize : height in pixels of the font
     *  pName : internal name wich could be used for retreiving font within font atlas
     * ( in place of index )
     * Returns font index
     */
    // Pre-compiles a list of characters from the given font
    void LoadUSASCII(const char *pFontFilePath, GLuint pFontSize, const char *pName);



private:
    // Render state
    GLuint VAO, VBO;
    GLenum TextureEngine=GL_TEXTURE0;
    GLuint Width, Height;
};


#endif //ZGLTEXT_H
