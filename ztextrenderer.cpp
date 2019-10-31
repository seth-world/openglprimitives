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
#include <zresource.h>

#include <algorithm> /* for std::max */

#include <glm/gtc/matrix_transform.hpp>


#include <ztextrenderer.h>

#include <ztexture.h>

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
#include <zresource.h>
#include <zshader.h>


ZTextRenderer::ZTextRenderer(GLenum pTexture)
{
    TextureEngine=pTexture;
    // Load and configure shader
//    TextShader=new ZShader("ztextrenderer.vs", "ztextrenderer.fs", "TextRenderer");
    TextShader=new ZShader("zgltext.vs", "zgltext.fs", "TextRenderer(zgltext)");

//    this->TextShader->setMat4Transpose("mProjection", glm::ortho(0.0f, static_cast<GLfloat>(pWidth), static_cast<GLfloat>(pHeight), 0.0f));
//    this->TextShader->use();
//    this->TextShader->setInt("TextSampler", Texture->getTextureEngineNumber());
    // Configure VAO/VBO for texture quads
    glGenVertexArrays(1, &this->VAO);
    glGenBuffers(1, &this->VBO);
    glBindVertexArray(this->VAO);
    glBindBuffer(GL_ARRAY_BUFFER, this->VBO);

    int wTextCoordsAttLocation=TextShader->getNamedAttributeLocation("TextCoords",true); /* abort on error option set*/
    glEnableVertexAttribArray(wTextCoordsAttLocation);

    glVertexAttribPointer(wTextCoordsAttLocation,
                          4, GL_FLOAT,  /* 4 vertices position <x,y> texture <x,y> */
                          GL_FALSE,     /* no normalization : float is kept */
                          0,            /* no stride */
                          nullptr);     /* no offset within buffer */

/*
   glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 6 * 4, nullptr, GL_DYNAMIC_DRAW);
   glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), nullptr);
*/
    TextShader->use();
    this->TextShader->setInt("TextureSampler", ZTexture::getTextureEngineNumber(TextureEngine));

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}
FontAtlas::FontAtlas(const char* pFont, int pFontSize, const char* pName,GLenum pTextureEngine)
//void atlas::generate(FT_Face face, int height)
{

    clear();
    Name=pName;

    FT_Error wFTerr;

    // Load font as face
    fprintf (stdout," Loading font <%s> size <%u> internal name <%s>\n",pFont,pFontSize,pName);
    FT_Face wFace;
    wFTerr=FT_New_Face(GLResources->getFreeTypeLibrary(), GLResources->getFontPath( pFont).c_str(), 0, &wFace);
    if (wFTerr!=FT_Err_Ok)
     {
         fprintf (stderr,"FontAtlas::Load-E Freetype error while loading font into library.\n"
                  "    font file <%s>\n"
                  "    Error <%d>  <%s>\n",
                  pFont,
                  wFTerr,
                  getFTErrorString( wFTerr));
         abort();
     }



    fprintf (stdout,
             "    Number of faces  <%ld> glyphs <%ld> scalable <%s>\n"
             "    Family    <%s> style <%s>\n"
             ,
             wFace->num_faces,
             wFace->num_glyphs,
            (wFace->face_flags& FT_FACE_FLAG_SCALABLE)==FT_FACE_FLAG_SCALABLE?"Yes":"No",
             wFace->family_name,
             wFace->style_name);


     FT_Set_Pixel_Sizes(wFace, 0, pFontSize);
     FT_GlyphSlot GlyphSlot = wFace->glyph;

    unsigned int wCurrentWidth = 0;
    unsigned int wCurrentHeight = 0;
    TexSumWidth = 0;
    TexSumHeight = 0;

//      memset(CharAtlas, 0, sizeof CharAtlas);
    int wErrNb=0;
    const int MaxFontErr=10;
    const int MaxCharCode=128;
     /* Find minimum size for a texture holding all visible ASCII characters */
     for (FT_ULong wChar = 32; wChar < 128; wChar++)
     {
//         wFTerr=FT_Load_Char(wFace, wChar, FT_LOAD_BITMAP_METRICS_ONLY);
         wFTerr=FT_Load_Char(wFace, wChar, FT_LOAD_RENDER);
         if (wFTerr!=FT_Err_Ok)
         {
             wErrNb++;
             if (wErrNb<MaxFontErr)
                 {
                 fprintf (stderr,
                          "FontAtlas::Load-E Freetype error while loading glyph from library.\n"
                          "    font <%s> character code <%lu>\n"
                          "    Error <%d>  <%s>\n",
                          pFont,
                          wChar,
                          wFTerr,
                          getFTErrorString( wFTerr));
                 }
             continue;
         }//if (wFTerr!=FT_Err_Ok)
         if (GlyphSlot->bitmap.width > __TEX_MAXWIDTH__)
            {
            fprintf (stderr,
                     "FontAtlas::Load-F Fatal error width overflow\n"
                     " for character code <%lu>\n"
                     "   bitmap size <%d>\n"
                     "   Maximum width for texture is <%d>\n",
                     wChar,
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
     } // for (int wChar = 32; wChar < 128; wChar++)

     TexSumWidth = std::max(TexSumWidth, wCurrentWidth);
     TexSumHeight += wCurrentHeight;

     /* Create a texture that will be used to hold all ASCII glyphs */

     this->Texture=new ZTexture(pTextureEngine); /* texture is GL_TEXTURE0 by default */
//     glActiveTexture(GL_TEXTURE0);
//     glGenTextures(1, &tex);
//     glBindTexture(GL_TEXTURE_2D, tex);
     this->Texture->bind();
//     glUniform1i(uniform_tex, 0); //this concerns TextShader program : see further

//     glTexImage2D(GL_TEXTURE_2D, 0, GL_ALPHA, TexWidth, TexHeight, 0, GL_ALPHA, GL_UNSIGNED_BYTE, 0);

     glTexImage2D(GL_TEXTURE_2D,
                  0,
                  GL_RED,
                  (GLsizei)TexSumWidth, (GLsizei)TexSumHeight,
                  0,
                  GL_RED,
                  GL_UNSIGNED_BYTE,
                  nullptr);


     int wPixelAlignment; /* store current pixel reading alignment value (default is 4) */
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

     for (FT_ULong wChar = 32; wChar < 128; wChar++)
     {
         if (FT_Load_Char(wFace, wChar, FT_LOAD_RENDER))
            {
             wErrNb++;
             if (wErrNb<MaxFontErr)
                 {
                 fprintf (stderr,
                          "ZTextRenderer::LOADCH-E Freetype error while loading glyph from library.\n"
                          "    font file <%s> character code <%lu>\n"
                          "    Error <%d>  <%s>\n",
                          pFont,
                          wChar,
                          wFTerr,
                          getFTErrorString( wFTerr));
                 }
             continue;
            }

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

//         CharAtlas[wChar].advanceX = GlyphSlot->advance.x >> 6;
//         CharAtlas[wChar].advanceY = GlyphSlot->advance.y >> 6;

         CharAtlas[wChar].Advance.x = (int)(GlyphSlot->advance.x >> 6);
         CharAtlas[wChar].Advance.y = (int)(GlyphSlot->advance.y >> 6);


         CharAtlas[wChar].Coef = (double)wFace->glyph->advance.x/(double)wFace->glyph->linearHoriAdvance;
         /* in order to use GlyphSlot->advance.y  FT_LOAD_VERTICAL_LAYOUT must be set using FT_Load_Char
          * but this option is marked as "not safe" in freetype doc and should be avoided */
         if (GlyphSlot->advance.y ==0)
            {
             CharAtlas[wChar].Advance.y = (int)(GlyphSlot->linearVertAdvance *CharAtlas[wChar].Coef);
            }

         CharAtlas[wChar].bitmap.width = GlyphSlot->bitmap.width;
         CharAtlas[wChar].bitmap.height = GlyphSlot->bitmap.rows;

//         CharAtlas[wChar].bitmapLeft = GlyphSlot->bitmap_left;
//         CharAtlas[wChar].bitmapTop = GlyphSlot->bitmap_top;

         CharAtlas[wChar].bitmap.left = GlyphSlot->bitmap_left;
         CharAtlas[wChar].bitmap.top = GlyphSlot->bitmap_top;

         CharAtlas[wChar].texX = ox / (float)TexSumWidth;
         CharAtlas[wChar].texY = oy / (float)TexSumHeight;

         wCurrentHeight = std::max(wCurrentHeight, GlyphSlot->bitmap.rows);
         ox += GlyphSlot->bitmap.width + 1;
     }// for

     fprintf(stderr,
             "FontAtlas-I For font <%s> <%d> generated a %d x %d (%d kb) texture atlas with <%d> errors\n",
             pFont,
             pFontSize,
             TexSumWidth, TexSumHeight, TexSumWidth * TexSumHeight / 1024,
             wErrNb);

     FT_Done_Face(wFace);
 }//FontAtlas::FontAtlas

long
ZTextRenderer::getFontIndex(const char*pName)
{
    for (long wi=0;wi<FontList.size();wi++)
            if (strncmp(FontList[wi]->Name,pName,__FONTNAME_MAX__)==0)
            {
                return wi;
            }
    if (FontList.count()==0)
        {
        fprintf (stderr,"ZTextRenderer-F-NOFONT No font have been loaded yet while trying to use font <%s>.\n", pName);
        abort();
        }

    fprintf (stderr,"ZTextRenderer-W-NOTFOUND Font name <%s> has not been found. Using replacement fond name <%s> index <%ld>.\n",
             pName,
             FontList[0]->Name,
            0L);
    return 0L;
}


void
ZTextRenderer::renderByName(const char *pText,
                            glm::vec3 pPosition,
                                glm::vec3  pColor,
                                const char* pName)
{

    long wFontIndex=getFontIndex(pName);
    render(pText,
           pPosition,
               pColor,
               wFontIndex);
    return;
}
void
ZTextRenderer::renderVerticalByName(const char *pText,
                                    glm::vec3 pPosition,
                                glm::vec3  pColor,
                                const char* pName)
{

    long wFontIndex=getFontIndex(pName);
    renderVertical(pText,
                   pPosition,
                   pColor,
                   wFontIndex);
    return;
}
void
ZTextRenderer::renderScaledByName(const char *pText,
                            glm::vec3 pPosition,
                            glm::vec3 pColor,
                            float pScale,
                            const char* pName)
{

    long wFontIndex=getFontIndex(pName);
    renderScaled(pText,
                     pPosition,
                     pColor,
                     pScale,
                     wFontIndex);
    return;
}

void
ZTextRenderer::render(const char *pText,
                      glm::vec3 pPosition,
                      glm::vec3 pColor,
                      long  pFontIndex)
{
    glm::vec2 wWSize = GLResources->getGLWindowSize();

    float sx = 2.0f / wWSize.x;
    float sy = 2.0f / wWSize.y;

    _render(pText,
               pPosition,
               sx, sy,
               pColor,
               pFontIndex);

}
void
ZTextRenderer::renderVertical(const char *pText,
                              glm::vec3 pPosition,
                              glm::vec3 pColor,
                              long  pFontIndex)
{
    glm::vec2 wWSize = GLResources->getGLWindowSize();

    float sx = 2.0f / wWSize.x;
    float sy = 2.0f / wWSize.y;

    _renderVertical(pText,
               pPosition,
               sx, sy,
               pColor,
               pFontIndex);

}
void
ZTextRenderer::renderScaled(const char *pText,
                            glm::vec3 pPosition,
                            const Color_type pColor,
                            float pScale,
                            long  pFontIndex)
{
    glm::vec2 wWSize = GLResources->getGLWindowSize();

    float sx = pScale * 2.0f / wWSize.x;
    float sy = pScale * 2.0f / wWSize.y;

    _render(pText,
               pPosition,
               sx, sy,
               pColor,
               pFontIndex);

}

/**
 * Render text using the currently loaded font and currently set font size.
 * Rendering starts at coordinates (x, y), z is always 0.
 * The pixel coordinates that the FreeType2 library uses are scaled by (sx, sy).
 */


void
ZTextRenderer::_render(const char *pText,
                       glm::vec3 pPosition,
                       float pSx, float pSy,
                       glm::vec3 pColor,
                       long  pFontIndex)
{
    if ((pFontIndex < 0)||(pFontIndex>=FontList.count()))
        {
        fprintf (stderr,"ZTextRenderer::renderText-E Invalid font index : given <%ld> while must be in range of [<0>,<%ld>]\n",
                 pFontIndex,
                 FontList.count()-1L);
        }

    GLboolean wBlendEnabled;

    glGetBooleanv(	GL_BLEND, &wBlendEnabled);
    if (!wBlendEnabled)
            glEnable(GL_BLEND);

    FontAtlas * wFont=FontList[pFontIndex];
    const uint8_t *p;

    /* Use the texture containing the atlas */
    wFont->Texture->bind();

    TextShader->use();

//    TextShader->setInt("TextSampler",wFont->Texture->getTextureEngineNumber()); /* hopefully we'll set TextSampler to 1 */
    TextShader->setupTexSampler(wFont->Texture);
    TextShader->setVec3(__TEXTCOLOR__,pColor);
    TextShader->setFloat(__TEXTPOSZ__,pPosition.z); /* set text depth (z coord) */

    /* Set up the VBO for our vertex data */
 /*   int wTextCoordsAttLocation=(GLuint)glGetAttribLocation(TextShader->getShaderId(),"TextCoords");
    if (wTextCoordsAttLocation<0)
    {
        fprintf (stderr,"ZTextRenderer::renderText-E-TEXCOORDSNTFND Attribute TexCoords not found within shader program.\n");
        abort();
    }
*/
    int wTextCoordsAttLocation=TextShader->getNamedAttributeLocation("TextCoords",true);/*with abort on error option set*/
    glBindVertexArray(VAO);
    glEnableVertexAttribArray(wTextCoordsAttLocation);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glVertexAttribPointer(wTextCoordsAttLocation,
                          4,        /* four items */
                          GL_FLOAT, /* type is float */
                          GL_FALSE, /* not to be normalized : use floating point directly */
                          0,        /* no stride */
                          0);       /* no offset from buffer beginning */

    textPoint wCoords[6 * strlen(pText)];
    int wC = 0;

    uint8_t wCharCode;
    /* Loop through all characters */
    for (p = (const uint8_t *)pText; *p; p++)
        {
        /* character code must be [32,128] else replacement character is used */

        if ((*p < 32)||(*p > 128))
                wCharCode = __REPLACEMENT_CHAR__;
        else
            wCharCode=*p;

//        fprintf(stdout," char <%c> code <%d>\n",*p,*p);

        /* Calculate the vertex and texture coordinates */
        float x2 = pPosition.x + wFont->CharAtlas[wCharCode].bitmap.left * pSx;
        float y2 = -pPosition.y - wFont->CharAtlas[wCharCode].bitmap.top * pSy;


        float w = wFont->CharAtlas[wCharCode].bitmap.width * pSx;
        float h = wFont->CharAtlas[wCharCode].bitmap.height * pSy;



        /* Advance the cursor to the start of the next character */
        pPosition.x += wFont->CharAtlas[wCharCode].Advance.x * pSx;

//        pPosition.x += wFont->CharAtlas[wCharCode].advanceX * pSx;
//        pY += wFont->CharAtlas[wCharCode].advanceY * pSy;  //only for vertical layout otherwise pY remains the same

        /* Skip glyphs that have no pixels */
        if (!w || !h)
                continue;

        wCoords[wC++] = (textPoint) {
                x2, -y2,
                wFont->CharAtlas[wCharCode].texX,  // texture s coord
                wFont->CharAtlas[wCharCode].texY}; // texture t coord
        wCoords[wC++] = (textPoint) {
                x2 + w, -y2,
                wFont->CharAtlas[wCharCode].texX + wFont->CharAtlas[wCharCode].bitmap.width / wFont->TexSumWidth,// texture s coord
                wFont->CharAtlas[wCharCode].texY};// texture t coord
        wCoords[wC++] = (textPoint) {
                x2, -y2 - h, // point <x , y>
                wFont->CharAtlas[wCharCode].texX, // texture s coord
                wFont->CharAtlas[wCharCode].texY + wFont->CharAtlas[wCharCode].bitmap.height / wFont->TexSumHeight};// texture t coord
        wCoords[wC++] = (textPoint) {
                x2 + w, -y2,// point <x , y>
                wFont->CharAtlas[*p].texX + wFont->CharAtlas[wCharCode].bitmap.width / wFont->TexSumWidth,// texture s coord
                wFont->CharAtlas[wCharCode].texY};// texture t coord
        wCoords[wC++] = (textPoint) {
                x2, -y2 - h, // point <x , y>
                wFont->CharAtlas[wCharCode].texX, // texture s coord
                wFont->CharAtlas[wCharCode].texY + wFont->CharAtlas[wCharCode].bitmap.height / wFont->TexSumHeight};// texture t coord
        wCoords[wC++] = (textPoint) {
                x2 + w, -y2 - h, // point <x , y>
                wFont->CharAtlas[wCharCode].texX + wFont->CharAtlas[wCharCode].bitmap.width / wFont->TexSumWidth, // texture s coord
                wFont->CharAtlas[wCharCode].texY + wFont->CharAtlas[wCharCode].bitmap.height / wFont->TexSumHeight};// texture t coord
    }//for (p = (const uint8_t *)pText; *p; p++)

    /* Draw all the character on the screen in one go */
    glBufferData(GL_ARRAY_BUFFER, sizeof (wCoords), wCoords, GL_DYNAMIC_DRAW);

//    wFont->Texture->bind();


    glDrawArrays(GL_TRIANGLES, 0, wC);

    glDisableVertexAttribArray(wTextCoordsAttLocation);

    if (!wBlendEnabled) /* restore blend state */
            glDisable(GL_BLEND);
}//_render

void
ZTextRenderer::_renderVertical(const char *pText,
                       glm::vec3 pPosition,
                       float pSx, float pSy,
                       glm::vec3 pColor,
                       long  pFontIndex)
{
    if ((pFontIndex < 0)||(pFontIndex>=FontList.count()))
        {
        fprintf (stderr,"ZTextRenderer::renderText-E Invalid font index : given <%ld> while must be in range of [<0>,<%ld>]\n",
                 pFontIndex,
                 FontList.count()-1L);
        }

    GLboolean wBlendEnabled;

    glGetBooleanv(	GL_BLEND, &wBlendEnabled);
    if (!wBlendEnabled)
            glEnable(GL_BLEND);

    FontAtlas * wFont=FontList[pFontIndex];
    const uint8_t *p;

    /* Use the texture containing the atlas */
    wFont->Texture->bind();

    TextShader->use();

    TextShader->setupTexSampler(wFont->Texture);
    TextShader->setVec3(__TEXTCOLOR__,pColor);
    TextShader->setFloat(__TEXTPOSZ__,pPosition.z); /* set text depth (z coord) */

    /* Set up the VBO for our vertex data */

    int wTextCoordsAttLocation=TextShader->getNamedAttributeLocation("TextCoords",true);/*with abort on error option set*/
    glBindVertexArray(VAO);
    glEnableVertexAttribArray(wTextCoordsAttLocation);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glVertexAttribPointer(wTextCoordsAttLocation,
                          4,        /* four items */
                          GL_FLOAT, /* type is float */
                          GL_FALSE, /* not to be normalized : use floating point directly */
                          0,        /* no stride */
                          0);       /* no offset from buffer beginning */

    textPoint wCoords[6 * strlen(pText)];
    int wC = 0;

    uint8_t wCharCode;
    /* Loop through all characters */
    for (p = (const uint8_t *)pText; *p; p++)
        {
        /* character code must be [32,128] else replacement character is used */

        if ((*p < 32)||(*p > 128))
                wCharCode = __REPLACEMENT_CHAR__;
        else
            wCharCode=*p;

        /* Calculate the vertex and texture coordinates */

//        float x2 = pPosition.x + wFont->CharAtlas[wCharCode].bitmap.left * pSx;
        float x2 = pPosition.x + ((float)(wFont->MaxWidth - wFont->CharAtlas[wCharCode].bitmap.width)/2.0f) * pSx;
        float y2 = -pPosition.y - wFont->CharAtlas[wCharCode].bitmap.top * pSy;


        float w = wFont->CharAtlas[wCharCode].bitmap.width * pSx;
        float h = wFont->CharAtlas[wCharCode].bitmap.height * pSy;

        /* Advance the cursor to the start of the next character */
//        pX += wFont->CharAtlas[wCharCode].advanceX * pSx;//only for horizontal layout otherwise pX remains the same
//        pPosition.y += wFont->CharAtlas[wCharCode].advanceY * pSy;  //only for vertical layout otherwise pY remains the same
        pPosition.y -= ((float)(wFont->CharAtlas[wCharCode].Advance.y>>6)) * pSy;  //only for vertical layout otherwise pY remains the same
//        pPosition.y -= h*1.2 ;

        /* Skip glyphs that have no pixels */
        if (!w || !h)
                continue;

        wCoords[wC++] = (textPoint) {
                x2, -y2,
                wFont->CharAtlas[wCharCode].texX,  // texture s coord
                wFont->CharAtlas[wCharCode].texY}; // texture t coord
        wCoords[wC++] = (textPoint) {
                x2 + w, -y2,
                wFont->CharAtlas[wCharCode].texX + wFont->CharAtlas[wCharCode].bitmap.width / wFont->TexSumWidth,// texture s coord
                wFont->CharAtlas[wCharCode].texY};// texture t coord
        wCoords[wC++] = (textPoint) {
                x2, -y2 - h, // point <x , y>
                wFont->CharAtlas[wCharCode].texX, // texture s coord
                wFont->CharAtlas[wCharCode].texY + wFont->CharAtlas[wCharCode].bitmap.height / wFont->TexSumHeight};// texture t coord
        wCoords[wC++] = (textPoint) {
                x2 + w, -y2,// point <x , y>
                wFont->CharAtlas[*p].texX + wFont->CharAtlas[wCharCode].bitmap.width / wFont->TexSumWidth,// texture s coord
                wFont->CharAtlas[wCharCode].texY};// texture t coord
        wCoords[wC++] = (textPoint) {
                x2, -y2 - h, // point <x , y>
                wFont->CharAtlas[wCharCode].texX, // texture s coord
                wFont->CharAtlas[wCharCode].texY + wFont->CharAtlas[wCharCode].bitmap.height / wFont->TexSumHeight};// texture t coord
        wCoords[wC++] = (textPoint) {
                x2 + w, -y2 - h, // point <x , y>
                wFont->CharAtlas[wCharCode].texX + wFont->CharAtlas[wCharCode].bitmap.width / wFont->TexSumWidth, // texture s coord
                wFont->CharAtlas[wCharCode].texY + wFont->CharAtlas[wCharCode].bitmap.height / wFont->TexSumHeight};// texture t coord
    }//for (p = (const uint8_t *)pText; *p; p++)

    /* Draw all the character on the screen in one go */
    glBufferData(GL_ARRAY_BUFFER, sizeof (wCoords), wCoords, GL_DYNAMIC_DRAW);

//    wFont->Texture->bind();


    glDrawArrays(GL_TRIANGLES, 0, wC);

    glDisableVertexAttribArray(wTextCoordsAttLocation);

    if (!wBlendEnabled) /* restore blend state */
            glDisable(GL_BLEND);
}//_renderVertical
