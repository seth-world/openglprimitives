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

class Character {
public:

    Character()=default;
    Character(ZTexture* pTexture,
              glm::ivec2 pSize,
              glm::ivec2 pBearing,
              GLuint pAdvance,
              long pxMin,
              long pyMin,
              long pxMax,
              long pyMax)
    {
//        TextureID=pTexId;
        Texture=pTexture;
        Size=pSize;
        Bearing=pBearing;
        Advance=pAdvance;
        xMin=pxMin;
        xMax=pxMax;
        yMin=pyMin;
        yMax=pyMax;

    }
    Character(Character& pIn) {memmove (this,&pIn,sizeof(Character));}

//    GLuint TextureID;   // ID handle of the glyph texture
    ZTexture* Texture;
    glm::ivec2 Size;    // Size of glyph
    glm::ivec2 Bearing; // Offset from baseline to left/top of glyph
    GLuint Advance;     // Horizontal offset to advance to next glyph
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
    }
    long FontHeight=0;
    FT_Int MaxBearingH=0;
    FT_Int MaxBearingW=0;
    Character& operator [] (int pIdx) {return Tab[pIdx];}

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
    ZGLText(GLuint pWidth, GLuint pHeight,GLenum pTexture);

    // Renders a string of text using the precompiled list of characters
    void RenderText(std::string pText,
                    GLfloat pPosX, GLfloat pPosY,
                    GLfloat pScale,
                    const Color_type pColor = Color_type(1.0f));

    void render(std::string pText,
                GLfloat pPosX, GLfloat pPosY,
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
    void LoadFont(const char *pFontFilePath, GLuint pFontSize, const char *pName);



private:
    // Render state
    GLuint VAO, VBO;
    GLenum TextureEngine=GL_TEXTURE0;
    GLuint Width, Height;
};


#endif //ZGLTEXT_H
