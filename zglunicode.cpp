/*******************************************************************
** This code is part of Breakout.
**
** Breakout is free software: you can redistribute it and/or modify
** it under the terms of the CC BY 4.0 license as published by
** Creative Commons, either version 4 of the License, or (at your
** option) any later version.
******************************************************************/

#ifdef __USE_GLAD__
#include <glad/glad.h>
#else
#include <GL/glew.h>
#endif

#include <iostream>
#include <zglresource.h>

#include <algorithm> /* for std::max */

#include <glm/gtc/matrix_transform.hpp>


#include <zglunicode.h>

#include <ztexture.h>

#include <zfont.h>

/* Freetype error description management as described per Freetype doc */
/*  defined within <zresources.h>
const char* getFTErrorString(const FT_Error error_code)
{
#undef FTERRORS_H_
#define FT_ERROR_START_LIST     switch ( error_code ) {
#define FT_ERRORDEF( e, v, s )    case v: return s;
#define FT_ERROR_END_LIST       }
#include FT_ERRORS_H
    return "Unknown FT error";
}
*/
#include <zglresource.h>
#include <zshader.h>
GLUnicodeText::GLUnicodeText(GLTextWriter* pWriter, GLenum pTextureEngine)
{
    Writer=pWriter;
    Texture = new ZTexture(pTextureEngine);
}
#ifdef __COMMENT__
GLUnicodeText::GLUnicodeText(const utf32_t* pText,const char* pFont, unsigned int pFontSize, GLenum pTextureEngine)
{
    clear();

    OriginText = pText;
    FontSize=pFontSize;

    FT_Error wFTerr;

    // Load font as face
    fprintf (stdout," Loading font <%s> size <%u>\n",pFont,pFontSize);

    wFTerr=FT_New_Face(GLResources->getFreeTypeLibrary(), GLResources->getFontPath( pFont).c_str(), 0, &Face);
    if (wFTerr!=FT_Err_Ok)
     {
         fprintf (stderr,"GLUnicodeText::Load-E Freetype error while loading font into library.\n"
                  "    font file <%s>\n"
                  "    Error <%d>  <%s>\n",
                  pFont,
                  wFTerr,
                  getFTErrorString( wFTerr));
         abort();
     }

    fprintf (stdout,
             "    Number of faces  <%ld> glyphs <%ld> scalable <%s> vertical data <%s>\n"
             "    Family    <%s> style <%s>\n"
             ,
             Face->num_faces,
             Face->num_glyphs,
            (Face->face_flags& FT_FACE_FLAG_SCALABLE)==FT_FACE_FLAG_SCALABLE?"Yes":"No",
             (Face->face_flags& FT_FACE_FLAG_VERTICAL)==FT_FACE_FLAG_VERTICAL?"Yes":"No",
             Face->family_name,
             Face->style_name);



     FT_Set_Pixel_Sizes(Face, 0, FontSize);

     TexSumWidth = 0;
     TexSumHeight = 0;

     wFTerr=FT_Select_Charmap(Face, FT_ENCODING_UNICODE);
     if (wFTerr!=FT_Err_Ok)
         {
         fprintf (stderr,
                  "FontAtlas::Load-E Freetype error while selecting Unicode charmap from library.\n"
                  "    font <%s>  Error <%d>  <%s>\n",
                  Name,
                  wFTerr,
                  getFTErrorString( wFTerr));
        }
}// CTOR
#endif //__COMMENT__


/*  loads a Character with unicode codepoint given by pChar into Font->Face
 *
 * returns  Positive value is character bitmap is loaded
 *          0 if successful
 *          1 if replacement character is loaded
 *
 *          Negative value if error
 *          -1 if character index has been found but no glyph is associated
 *          - 0x0F if no Font has been defined (Font is nullptr)
 */
inline int GLUnicodeText::loadChar(FT_ULong pChar)
{
FT_Error wFTerr=FT_Err_Ok;
int wRet=0;

    if (Font==nullptr)
            return -0x0F;

    FT_UInt wGlyph_index=FT_Get_Char_Index(Font->Face, pChar);
    if (wGlyph_index==0) /* index = 0 : char code is not available as glyph */
    {
      wGlyph_index=FT_Get_Char_Index(Font->Face, (FT_ULong)__REPLACEMENT_CHAR__);   /* use replacement character */
      wRet=1;
    }
//    wFTerr=FT_Load_Char(Face, (FT_ULong)*p,FT_LOAD_RENDER);

    wFTerr = FT_Load_Glyph(Font->Face,          /* handle to face object */
                           wGlyph_index,   /* glyph index           */
                           FT_LOAD_RENDER );
    if (wFTerr!=FT_Err_Ok)
        {
        fprintf (stderr,
                 "%s-E Freetype error while loading Unicode character Glyph character code <%X>.\n"
                 "    Error <%d>  <%s>\n",
                 _GET_FUNCTION_NAME_,
                 pChar,
                 wFTerr,
                 getFTErrorString( wFTerr));
        return -1;

        }
    return wRet;
}//GLUnicodeText::loadChar

int GLUnicodeText::setText(const utf32_t* pUtf32Text,const int pFontIdx)
{
}

/**
 * @brief GLUnicodeText::setText prepares a unicode text (expressed in unicode codepoints) ready to be rendered by render() or renderVertical().
 *  This routine formats a texture whose texture engine has been set within UnicodeWriter object according glyphs from font given by pFontName.
 *  pFonName must point to a font that must have been loaded within object UnicodeWriter.
 *
 * @param pUtf32Text
 * @param pFontName
 */
int GLUnicodeText::setText(const utf32_t* pUtf32Text,const char* pFontName, size_t pFontSize)
{
FT_Error wFTerr;
FT_GlyphSlot GlyphSlot=nullptr ;
unsigned int wCurrentWidth = 0;
unsigned int wCurrentHeight = 0;

    Font=GLResources->getFontByName(pFontName);

    if (Font==nullptr)
            return -1;
/*
    long wFontIdx=Writer->getFontIndex(pFontName);
    Font=Writer->FontList[wFontIdx];
*/
//    FT_Set_Pixel_Sizes(Font->Face, 0, Font->FontSize);
    FT_Set_Pixel_Sizes(Font->Face, 0, (FT_UInt)pFontSize);

    const utf32_t* p=pUtf32Text;
    TextLen=0;

    for (;*p;p++) /* iterate thru utf32 text */
    {
    if (loadChar((FT_ULong)*p) < 0) /* ignore if errored (signaled to console log by loadChar) */
                            {
                            continue;
                            }
    GlyphSlot = Font->Face->glyph;
    if (GlyphSlot->bitmap.width > __TEX_MAXWIDTH__)
       {
       fprintf (stderr,
                "%s-F Font <%s> Fatal error texture maximum width overflow\n"
                " for character code <%u>\n"
                "   bitmap size <%d>\n"
                "   Maximum width for texture is <%d>\n",
                _GET_FUNCTION_NAME_,
                Font->Name,
                *p,
                GlyphSlot->bitmap.width,
                __TEX_MAXWIDTH__);
       abort();
       }
        if (wCurrentWidth + GlyphSlot->bitmap.width + 1 >= __TEX_MAXWIDTH__)  /* jump to new 'line' */
            {
            TexSumWidth = std::max(TexSumWidth, wCurrentWidth);
            TexSumHeight += wCurrentHeight;
            wCurrentWidth = 0;
            wCurrentHeight = 0;
            }

        wCurrentWidth += GlyphSlot->bitmap.width + 1;
        wCurrentHeight = std::max(wCurrentHeight, GlyphSlot->bitmap.rows);

    }// for (;*p;p++)

    TexSumWidth = std::max(TexSumWidth, wCurrentWidth);
    TexSumHeight += wCurrentHeight;

    /* Create a texture that will be used to hold all Unicode text glyphs */

    this->Texture->reset(); /* texture is GL_TEXTURE0 by default */
    this->Texture->bind();

    glTexImage2D(GL_TEXTURE_2D,
                 0,
                 GL_RED,
                 (GLsizei)TexSumWidth, (GLsizei)TexSumHeight,
                 0,
                 GL_RED,
                 GL_UNSIGNED_BYTE,
                 nullptr);

    /* store current pixel reading alignment value (default is 4) */
    int wPixelAlignment;
    glGetIntegerv(GL_UNPACK_ALIGNMENT,(GLint*)&wPixelAlignment);

    /* We require 1 byte alignment when uploading texture data */
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    /* Clamping to edges is important to prevent artifacts when scaling */
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    /* Linear filtering usually looks best for text */
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    /* Paste all glyph bitmaps into the texture, remembering the offset */
    int ox = 0;
    int oy = 0;

    wCurrentHeight = 0;
    int wErrNb=0;
    for (const utf32_t *p=pUtf32Text; *p; p++)
    {
        if (loadChar((FT_ULong)*p) < 0)
                        {
                        wErrNb++;
                        continue;
                        }
        GlyphSlot = Font->Face->glyph;
        if (ox + (int)GlyphSlot->bitmap.width + 1 >= __TEX_MAXWIDTH__)
           {
            oy += wCurrentHeight;
            wCurrentHeight = 0;
            ox = 0;
           }

        glTexSubImage2D(GL_TEXTURE_2D,
                        0,
                        ox, oy,
                        GlyphSlot->bitmap.width, GlyphSlot->bitmap.rows,
//                         GL_ALPHA,
                        GL_RED,
                        GL_UNSIGNED_BYTE,
                        GlyphSlot->bitmap.buffer);

        GLUnicodeChar *wUtfChar=new GLUnicodeChar;

        wUtfChar->Advance.x = (int)(GlyphSlot->advance.x >> 6);
        wUtfChar->Advance.y = (int)(GlyphSlot->advance.y >> 6);


        wUtfChar->Coef = (double)GlyphSlot->advance.x/(double)GlyphSlot->linearHoriAdvance;
        /* in order to use GlyphSlot->advance.y  FT_LOAD_VERTICAL_LAYOUT must be set using FT_Load_Char
         * but this option is marked as "not safe" in freetype doc and should be avoided */
        if (GlyphSlot->advance.y ==0)
           {
            wUtfChar->Advance.y = (int)(GlyphSlot->linearVertAdvance * wUtfChar->Coef);
           }

        wUtfChar->bitmap ={(float)GlyphSlot->bitmap.width,
                          (float)GlyphSlot->bitmap.rows,
                          (float)GlyphSlot->bitmap_left,
                          (float)GlyphSlot->bitmap_top};

        wUtfChar->texX = ox / (float)TexSumWidth;
        wUtfChar->texY = oy / (float)TexSumHeight;

        wCurrentHeight = std::max(wCurrentHeight, GlyphSlot->bitmap.rows);
        ox += GlyphSlot->bitmap.width + 1;

        this->MaxWidth = std::max(MaxWidth,(FT_Int)GlyphSlot->bitmap.width);
        this->MaxHeight = std::max(MaxHeight,(FT_Int)GlyphSlot->bitmap.rows);

        UTexChar.push(wUtfChar);
        TextLen++;
    }// for

    fprintf(stderr,
            "GLUnicodeText::%s-I For font <%s> <%d> generated a %d x %d (%d kb) texture atlas with <%d> errors\n",
            _GET_FUNCTION_NAME_,
            Font->Name,
            Font->FontSize,
            TexSumWidth, TexSumHeight, TexSumWidth * TexSumHeight / 1024,
            wErrNb);

    // Configure VAO/VBO for texture quads

    if (VAO==0)
        glGenVertexArrays(1, &this->VAO);
    if (VBO==0)
        glGenBuffers(1, &this->VBO);

    if (wPixelAlignment!=1)
            glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
//    glBindVertexArray(this->VAO);
//    glBindBuffer(GL_ARRAY_BUFFER, this->VBO);

    return 0;
} // setText

void GLUnicodeText::_render(glm::vec3 pPosition,
                            glm::vec3 pColor,
                            float pSx,
                            float pSy)
{

    Writer->TextShader->use();
    int wTextCoordsAttLocation=Writer->TextShader->getNamedAttributeLocation("TextCoords",true); /* abort on error option set*/

    glBindVertexArray(VAO);
    glEnableVertexAttribArray(wTextCoordsAttLocation);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glVertexAttribPointer(wTextCoordsAttLocation,
                          4,        /* four items */
                          GL_FLOAT, /* type is float */
                          GL_FALSE, /* not to be normalized : use floating point directly */
                          0,        /* no stride */
                          0);


    /* save Blending/no Blending state */
    GLboolean wBlendEnabled;
    glGetBooleanv(	GL_BLEND, &wBlendEnabled);
    if (!wBlendEnabled)
            glEnable(GL_BLEND);

    Writer->TextShader->setupTexSampler(Texture);
    Writer->TextShader->setFloat(__TEXTPOSZ__,pPosition.z);
    Writer->TextShader->setVec3(__TEXTCOLOR__,pColor);

      /* no offset from buffer beginning */

    textPoint wCoords[6 * TextLen];

    int wC = 0;

    /* Loop through all characters */

    for (size_t wi=0; wi<TextLen; wi++)
        {

        /* Calculate the vertex and texture coordinates */
        float x2 = pPosition.x + UTexChar[wi]->bitmap.left * pSx;
        float y2 = -pPosition.y - UTexChar[wi]->bitmap.top * pSy;


        float w = UTexChar[wi]->bitmap.width * pSx;
        float h = UTexChar[wi]->bitmap.height * pSy;

        /* Advance the cursor to the start of the next character */
        pPosition.x += UTexChar[wi]->Advance.x * pSx;

//        pPosition.x += wFont->CharAtlas[wCharCode].advanceX * pSx;
//        pY += wFont->CharAtlas[wCharCode].advanceY * pSy;  //only for vertical layout otherwise pY remains the same

        /* Skip glyphs that have no pixels */
        if (!w || !h)
                continue;

        wCoords[wC++] = (textPoint) {
                x2, -y2,
                UTexChar[wi]->texX,  // texture s coord
                UTexChar[wi]->texY}; // texture t coord
        wCoords[wC++] = (textPoint) {
                x2 + w, -y2,
                UTexChar[wi]->texX + UTexChar[wi]->bitmap.width / TexSumWidth,// texture s coord
                UTexChar[wi]->texY};// texture t coord
        wCoords[wC++] = (textPoint) {
                x2, -y2 - h, // point <x , y>
                UTexChar[wi]->texX, // texture s coord
                UTexChar[wi]->texY + UTexChar[wi]->bitmap.height / TexSumHeight};// texture t coord
        wCoords[wC++] = (textPoint) {
                x2 + w, -y2,// point <x , y>
                UTexChar[wi]->texX + UTexChar[wi]->bitmap.width / TexSumWidth,// texture s coord
                UTexChar[wi]->texY};// texture t coord
        wCoords[wC++] = (textPoint) {
                x2, -y2 - h, // point <x , y>
                UTexChar[wi]->texX, // texture s coord
                UTexChar[wi]->texY + UTexChar[wi]->bitmap.height / TexSumHeight};// texture t coord
        wCoords[wC++] = (textPoint) {
                x2 + w, -y2 - h, // point <x , y>
                UTexChar[wi]->texX + UTexChar[wi]->bitmap.width / TexSumWidth, // texture s coord
                UTexChar[wi]->texY + UTexChar[wi]->bitmap.height / TexSumHeight};// texture t coord
    }//for (p = (const uint8_t *)pText; *p; p++)

    /* Draw all the character on the screen in one go */
    glBufferData(GL_ARRAY_BUFFER, (int)sizeof (wCoords), wCoords, GL_DYNAMIC_DRAW);

    Texture->bind();
    glDrawArrays(GL_TRIANGLES, 0, wC);

    glDisableVertexAttribArray(wTextCoordsAttLocation);

    /* restore Blending/no Blending state */
    if (!wBlendEnabled)
            glDisable(GL_BLEND);
    /* restore Pixel load alignment */


}//GLUnicodeText::_render

void GLUnicodeText::_renderVertical(glm::vec3 pPosition,
                                    glm::vec3 pColor,
                                    float pSx,
                                    float pSy)
{

    Writer->TextShader->use();
    int wTextCoordsAttLocation=Writer->TextShader->getNamedAttributeLocation("TextCoords",true); /* abort on error option set*/

    glBindVertexArray(VAO);
    glEnableVertexAttribArray(wTextCoordsAttLocation);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glVertexAttribPointer(wTextCoordsAttLocation,
                          4,        /* four items */
                          GL_FLOAT, /* type is float */
                          GL_FALSE, /* not to be normalized : use floating point directly */
                          0,        /* no stride */
                          0);


    /* save Blending/no Blending state */
    GLboolean wBlendEnabled;
    glGetBooleanv(	GL_BLEND, &wBlendEnabled);
    if (!wBlendEnabled)
            glEnable(GL_BLEND);

    Writer->TextShader->setupTexSampler(Texture);
    Writer->TextShader->setFloat(__TEXTPOSZ__,pPosition.z);
    Writer->TextShader->setVec3(__TEXTCOLOR__,pColor);

      /* no offset from buffer beginning */

    textPoint wCoords[6 * TextLen];

    int wC = 0;

    /* Loop through all characters */

    for (size_t wi=0; wi<TextLen; wi++)
        {

        /* Calculate the vertex and texture coordinates */
//        float x2 = pPosition.x + UTexChar[wi]->bitmap.left * pSx;
//        float y2 = -pPosition.y - UTexChar[wi]->bitmap.top * pSy;
        float x2 = pPosition.x + ((float)(this->MaxWidth - UTexChar[wi]->bitmap.width)/2.0f) * pSx;
        float y2 = -pPosition.y - UTexChar[wi]->bitmap.top * pSy;

        float w = UTexChar[wi]->bitmap.width * pSx;
        float h = UTexChar[wi]->bitmap.height * pSy;

        /* Advance the cursor to the start of the next character */
//        pPosition.x += UTexChar[wi]->Advance.x * pSx;
        pPosition.y -= ((float)(UTexChar[wi]->Advance.y>>6)) * pSy;
//        pPosition.x += wFont->CharAtlas[wCharCode].advanceX * pSx;
//        pY += wFont->CharAtlas[wCharCode].advanceY * pSy;  //only for vertical layout otherwise pY remains the same

        /* Skip glyphs that have no pixels */
        if (!w || !h)
                continue;

        wCoords[wC++] = (textPoint) {
                x2, -y2,
                UTexChar[wi]->texX,  // texture s coord
                UTexChar[wi]->texY}; // texture t coord
        wCoords[wC++] = (textPoint) {
                x2 + w, -y2,
                UTexChar[wi]->texX + UTexChar[wi]->bitmap.width / TexSumWidth,// texture s coord
                UTexChar[wi]->texY};// texture t coord
        wCoords[wC++] = (textPoint) {
                x2, -y2 - h, // point <x , y>
                UTexChar[wi]->texX, // texture s coord
                UTexChar[wi]->texY + UTexChar[wi]->bitmap.height / TexSumHeight};// texture t coord
        wCoords[wC++] = (textPoint) {
                x2 + w, -y2,// point <x , y>
                UTexChar[wi]->texX + UTexChar[wi]->bitmap.width / TexSumWidth,// texture s coord
                UTexChar[wi]->texY};// texture t coord
        wCoords[wC++] = (textPoint) {
                x2, -y2 - h, // point <x , y>
                UTexChar[wi]->texX, // texture s coord
                UTexChar[wi]->texY + UTexChar[wi]->bitmap.height / TexSumHeight};// texture t coord
        wCoords[wC++] = (textPoint) {
                x2 + w, -y2 - h, // point <x , y>
                UTexChar[wi]->texX + UTexChar[wi]->bitmap.width / TexSumWidth, // texture s coord
                UTexChar[wi]->texY + UTexChar[wi]->bitmap.height / TexSumHeight};// texture t coord
    }//for (p = (const uint8_t *)pText; *p; p++)

    /* Draw all the character on the screen in one go */
    glBufferData(GL_ARRAY_BUFFER, (int)sizeof (wCoords), wCoords, GL_DYNAMIC_DRAW);

    Texture->bind();
    glDrawArrays(GL_TRIANGLES, 0, wC);

    glDisableVertexAttribArray(wTextCoordsAttLocation);

    /* restore Blending/no Blending state */
    if (!wBlendEnabled)
            glDisable(GL_BLEND);
    /* restore Pixel load alignment */


}//GLUnicodeText::_renderVertical



void GLUnicodeText::render(glm::vec3 pPosition,
                           const Color_type pColor)
{
    glm::vec2 wWSize = GLResources->getGLWindowSize();

    float sx =  2.0f / wWSize.x;
    float sy = 2.0f / wWSize.y;

    _render(pPosition,
            pColor,
            sx, sy);
}
void GLUnicodeText::renderVertical(glm::vec3 pPosition,
                                   const Color_type pColor)
{
    glm::vec2 wWSize = GLResources->getGLWindowSize();

    float sx =  2.0f / wWSize.x;
    float sy = 2.0f / wWSize.y;

    _renderVertical(pPosition,
                    pColor,
                    sx, sy);
}


GLTextWriter::GLTextWriter(GLenum pTextureEngine)
{
    TextureEngine=pTextureEngine;

    TextShader=new ZShader("zgltext.vs", "zgltext.fs", "UnicodeShader");
}

/*
long
GLUnicodeWriter::getFontIndex(const char*pName)
{
    for (long wi=0;wi<FontList.size();wi++)
            if (strncmp(FontList[wi]->Name,pName,__FONTNAME_MAX__)==0)
            {
                return wi;
            }
    if (FontList.count()==0)
        {
        fprintf (stderr,"GLUnicodeWriter::%s-F-NOFONT No font have been loaded yet while trying to use font <%s>.\n",
                 _GET_FUNCTION_NAME_,
                 pName);
        abort();
        }

    fprintf (stderr,"GLUnicodeWriter::%s-W-NOTFOUND Font name <%s> has not been found. Using replacement fond name <%s> index <%ld>.\n",
             _GET_FUNCTION_NAME_,
             pName,
             FontList[0]->Name,
            0L);
    return 0L;
}
*/
GLUnicodeText* GLTextWriter::newText()
{
    GLUnicodeText* wT=new GLUnicodeText(this,TextureEngine);
    TextList.push(wT);
    return wT;
}

GLTextWriter::~GLTextWriter()
{
    while (TextList.count())
                delete TextList.popR();
}
