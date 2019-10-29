/*******************************************************************
** This code is part of Breakout.
**
** Breakout is free software: you can redistribute it and/or modify
** it under the terms of the CC BY 4.0 license as published by
** Creative Commons, either version 4 of the License, or (at your
** option) any later version.
******************************************************************/


#include <iostream>
#include <zresource.h>

#include <algorithm> /* for std::max */

#include <glm/gtc/matrix_transform.hpp>


#include <zgltext.h>

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

//#ifdef __COMMENT__
ZGLText::ZGLText(GLuint pWidth, GLuint pHeight, GLenum pTexture)
{
    TextureEngine=pTexture;
    // Load and configure shader
    TextShader=new ZShader("zgltext.vs", "zgltext.fs", "ZGLTextShader");
//    this->TextShader->setMat4Transpose("mProjection", glm::ortho(0.0f, static_cast<GLfloat>(pWidth), static_cast<GLfloat>(pHeight), 0.0f));

    glm::mat4 wProjection = glm::ortho(0.0f, static_cast<GLfloat>(pWidth), static_cast<GLfloat>(pHeight), 0.0f);

    TextShader->use();
    this->TextShader->setMat4("mProjection", wProjection);

//    int wLoc=glGetUniformLocation(TextShader->ID, "mProjection");
//    glUniformMatrix4fv(wLoc, 1, GL_TRUE, &wProjection[0][0]);

//    this->TextShader->setMat4("mProjection", glm::ortho(0.0f, static_cast<GLfloat>(pWidth), static_cast<GLfloat>(pHeight), 0.0f));
    this->TextShader->setInt("TextureSampler", ZTexture::getTextureEngineNumber(TextureEngine));

    // Configure VAO/VBO for texture quads
    glGenVertexArrays(1, &this->VAO);
    glGenBuffers(1, &this->VBO);

    glBindVertexArray(this->VAO);
    glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 6 * 4, nullptr, GL_DYNAMIC_DRAW);

    int wAttrPos=TextShader->getNamedAttributeLocation("TextCoords",true); /*with abort on error option set*/
    glVertexAttribPointer(wAttrPos, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), nullptr);
    glEnableVertexAttribArray(wAttrPos);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void ZGLText::LoadFont(const char* pFontFile, GLuint pFontSize,const char* pName)
{

    // First clear the previously loaded Characters
    this->Characters.clear();
    // Then initialize and load the FreeType library
/*    FT_Library ft;
    FT_Error wFTerr    = FT_Init_FreeType(&ft);
    if (wFTerr!=FT_Err_Ok) // All functions return a value different than 0 whenever an error occurred
        {
            fprintf (stderr,"ZGLText::Load-E Freetype error while initializing library.\n"
                     "    Error <%d>  <%s>\n",
                     wFTerr,
                     getFTErrorString( wFTerr));
            abort();
        }
    // Load font as face
*/
    FT_Error wFTerr;
    // Load font as face
    FT_Library ft=GLResources->getFreeTypeLibrary();

    FT_Face wFace;
    wFTerr=FT_New_Face(ft, GLResources->getFontPath( pFontFile).c_str(), 0, &wFace);
    if (wFTerr!=FT_Err_Ok)
    {
        fprintf (stderr,"ZGLText::Load-E Freetype error while loading font into library.\n"
                 "    font file <%s>\n"
                 "    Error <%d>  <%s>\n",
                 pFontFile,
                 wFTerr,
                 getFTErrorString( wFTerr));
        abort();
    }

    fprintf (stdout,
             "ZGLText-I loading font <%s> <%u> internal name <%s>\n",
             pFontFile,
             pFontSize,
             pName);

    fprintf (stdout,
             "    Number of faces  <%ld> glyphs <%ld> scalable <%s>\n"
             "    Family    <%s> style <%s>\n"
             ,
             wFace->num_faces,
             wFace->num_glyphs,
            (wFace->face_flags& FT_FACE_FLAG_SCALABLE)==FT_FACE_FLAG_SCALABLE?"Yes":"No",
             wFace->family_name,
             wFace->style_name);


//    fprintf (stdout,"ZTextRender-I Font <%s> size <%u> loaded.\n",pFontFilePath,pFontSize);
    // Set size to load glyphs as
    FT_Set_Pixel_Sizes(wFace, 0, pFontSize);
    // Disable byte-alignment restriction
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1); 
    // Then for the first 128 ASCII characters, pre-load/compile their characters and store them
    int wErrNb=0;
    const int MaxFontErr=10;
    const int MaxCharCode=128;
    size_t wSize=0;
    for (GLubyte wChar = 32; wChar < MaxCharCode; wChar++) // lol see what I did there
    {
        wFTerr=FT_Load_Char(wFace, wChar, FT_LOAD_RENDER);
        // Load character glyph 
        if (wFTerr!=FT_Err_Ok)
        {
            wErrNb++;
            if (wErrNb<MaxFontErr)
            {
            fprintf (stderr,
                     "ZGLText::LOADCH-E Freetype error while loading glyph from library.\n"
                     "    font file <%s> character code <%c>\n"
                     "    Error <%d>  <%s>\n",
                     pFontFile,
                     wChar,
                     wFTerr,
                     getFTErrorString( wFTerr));
            }
            else {
                if (wErrNb==MaxFontErr)
                    fprintf (stderr,
                             "ZGLText::LOADCH-E Freetype error maximum error message number reached.\n"
                             "    Further messages will be ignored.\n");
            }// else
            continue;
        }//if (wFTerr!=FT_Err_Ok)
    // Generate texture
/*    glActiveTexture(TextureEngine);
    GLuint texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);*/

    GLuint wTexId=0;
    ZTexture* wTexture=nullptr;

    if (wFace->glyph->bitmap.buffer)
    {
        wTexture=new ZTexture(TextureEngine);
        wTexId=wTexture->getId();

        wTexture->bind();

        glTexImage2D(
            GL_TEXTURE_2D,
            0,
            GL_RED,
            (GLsizei) wFace->glyph->bitmap.width,
            (GLsizei) wFace->glyph->bitmap.rows,
            0,
            GL_RED,
            GL_UNSIGNED_BYTE,
            wFace->glyph->bitmap.buffer
            );

        /* some stats to report */
        wSize += (size_t)((wFace->glyph->bitmap.rows*wFace->glyph->bitmap.width)/1024);

        // Set texture options
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

         }// if bitmap

// Now store character for later use
        Character character = {
            wTexId,     /* = 0 if no bitmap buffer */
            wTexture,   /* = nullptr if no bitmap buffer */
            glm::ivec2(wFace->glyph->bitmap.width, wFace->glyph->bitmap.rows),
            glm::ivec2(wFace->glyph->bitmap_left, wFace->glyph->bitmap_top),
            (GLuint)wFace->glyph->advance.x
        };

//        Characters.insert(std::pair<GLchar, Character>(wChar, character));
        Characters[wChar]=character;
    }//for (GLubyte wChar

    fprintf (stdout,
             "ZGLText-I Font <%s> size <%u> loaded"
             "  %d glyphs generated : total bitmap size of %ld KB.\n",
             pFontFile,
             pFontSize,
             MaxCharCode,
             wSize);

 //   this->TextShader->setInt("TextSampler", wTexture->getTextureEngineNumber());
//    this->TextShader->setInt("TextSampler", ZTexture::getTextureEngineNumber(GL_TEXTURE0));

    ZTexture::unbind();
//    glBindTexture(GL_TEXTURE_2D, 0);
    // Destroy FreeType once we're finished
    FT_Done_Face(wFace);
//    FT_Done_FreeType(ft);
}

void ZGLText::RenderText(std::string pText,
                               GLfloat pPosX, GLfloat pPosY,
                               GLfloat pScale,
                               const glm::vec3 pColor)
{   
    // Activate corresponding render state	
    this->TextShader->use();
    this->TextShader->setVec3(__TEXTCOLOR__, pColor);
    glActiveTexture(TextureEngine);
//    glActiveTexture(Texture->getTextureEngine());
    glBindVertexArray(this->VAO);

//    this->TextShader->setInt("TexSampler",2);

    this->TextShader->setInt(__TEXTURESAMPLER__,ZTexture::getTextureEngineNumber(TextureEngine));

    // Iterate through all characters
    std::string::const_iterator c;
    for (c = pText.begin(); c != pText.end(); c++)
    {
        Character ch = Characters[*c];

        GLfloat xpos = pPosX + ch.Bearing.x * pScale;
        GLfloat ypos = pPosY + (this->Characters['H'].Bearing.y - ch.Bearing.y) * pScale;

//        GLfloat xpos = pPosX ;
//        GLfloat ypos = pPosY ;


        GLfloat w = ch.Size.x * pScale;
        GLfloat h = ch.Size.y * pScale;
        if (!ch.Texture)
                {
                pPosX += (ch.Advance >> 6) * pScale; // Bitshift by 6 to get value in pixels (1/64th times 2^6 = 64)
                continue;
                }
        // Update VBO for each character
        GLfloat vertices[6][4] = {
            { xpos,     ypos + h,   0.0, 1.0 },
            { xpos + w, ypos,       1.0, 0.0 },
            { xpos,     ypos,       0.0, 0.0 },

            { xpos,     ypos + h,   0.0, 1.0 },
            { xpos + w, ypos + h,   1.0, 1.0 },
            { xpos + w, ypos,       1.0, 0.0 }
        };

        // Render glyph texture over quad
//        glBindTexture(GL_TEXTURE_2D, ch.TextureID);

        ch.Texture->bind();
        glBindVertexArray(this->VAO);

//        ch.Texture->bind();
        // Update content of VBO memory
        glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
        glBufferSubData(GL_ARRAY_BUFFER,
                        0,                  /* offset from begining of father buffer */
                        sizeof(vertices),   /* size of copied data */
                        vertices            /* data to copy */
                        ); // Be sure to use glBufferSubData and not glBufferData



        // Render quad
        glDrawArrays(GL_TRIANGLES, 0, 6);


        // Now advance cursors for next glyph
        pPosX += (ch.Advance >> 6) * pScale; // Bitshift by 6 to get value in pixels (1/64th times 2^6 = 64)
    }// for
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
//    glBindTexture(GL_TEXTURE_2D, 0);

    ZTexture::unbind();
}

