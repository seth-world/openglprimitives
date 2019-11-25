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

#include <camera.h>

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
    TextSumWidth=0;

    TexSumWidth=0;
    TexSumHeight=0;

    /* find number of lines
     */


    for (;*p;p++) /* iterate thru utf32 text */
    {

    if (loadChar((FT_ULong)*p) < 0) /* ignore if errored (signaled to console log by loadChar) */
                            {
                            continue;
                            }
    GlyphSlot = Font->Face->glyph;

    TextSumWidth += GlyphSlot->bitmap.width ; /* compute total text glyphs width */

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



    /* add special character(s) at the end of texture : this is not part of text */
    utf32_t wTruncChar = 0x000000b6 ; /* pilcrow character */

    if (loadChar((FT_ULong)wTruncChar) >= 0)
        {
        GlyphSlot = Font->Face->glyph;
        if (wCurrentWidth + GlyphSlot->bitmap.width + 1 >= __TEX_MAXWIDTH__)  /* jump to new 'line' */
            {
            TexSumWidth = std::max(TexSumWidth, wCurrentWidth);
            TexSumHeight += wCurrentHeight;
            wCurrentWidth = 0;
            wCurrentHeight = 0;
            }
        wCurrentWidth += GlyphSlot->bitmap.width + 1;
        wCurrentHeight = std::max(wCurrentHeight, GlyphSlot->bitmap.rows);
        }

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
    bool wIsCuttingChar=false;
    bool wIsNewLineChar=false;
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

        wUtfChar->isNewLineChar=((utfStrchr<utf32_t>(GLResources->NewLineCharList,*p))!=nullptr);
        wUtfChar->isCuttingChar=((utfStrchr<utf32_t>(GLResources->CuttingCharList,*p))!=nullptr);


        wUtfChar->Advance.x = (int)(GlyphSlot->advance.x >> 6);
        wUtfChar->Advance.y = (int)(GlyphSlot->advance.y >> 6);


        wUtfChar->Coef = (double)GlyphSlot->advance.x/(double)GlyphSlot->linearHoriAdvance;
        /* in order to use GlyphSlot->advance.y  FT_LOAD_VERTICAL_LAYOUT must be set using FT_Load_Char
         * but this option is marked as "not safe" in freetype doc and should be avoided */
        if (GlyphSlot->advance.y ==0)
           {
            wUtfChar->Advance.y = (int)(GlyphSlot->linearVertAdvance * wUtfChar->Coef);
           }

        wUtfChar->Advance.y =wUtfChar->Advance.y >> 6 ;

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

/*------------------------------------------------------------------
  get special character and add it as last character to texture
 */
//    utf32_t wTruncChar = 0x000000b6 ; /* pilcrow character */

    if (loadChar((FT_ULong)wTruncChar) < 0)
                    {
                    wErrNb++;
                    }
        else
    {
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
    }

    /* Truncation char is the last character of the list (UTexChar.count()-1)
     * while last character index of the text is given by TextLen -1
     * --------------------------------------------------------------------
     */
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

inline void GLUnicodeText::_setUpGLState(glm::vec3 pPosition,
                                  glm::vec3 pColor)
{
    Writer->TextShader->use();
    TextCoordsAttLocation=Writer->TextShader->getNamedAttributeLocation("TextCoords",true); /* abort on error option set*/

    glBindVertexArray(VAO);
    glEnableVertexAttribArray(TextCoordsAttLocation);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glVertexAttribPointer(TextCoordsAttLocation,
                          4,        /* four items */
                          GL_FLOAT, /* type is float */
                          GL_FALSE, /* not to be normalized : use floating point directly */
                          0,        /* no stride */
                          0);


    /* save Blending/no Blending state */
//    GLboolean wBlendEnabled;
    glGetBooleanv(	GL_BLEND, &BlendEnabled);
    if (!BlendEnabled)
            glEnable(GL_BLEND);

    Writer->TextShader->setupTexSampler(Texture);
    Writer->TextShader->setFloat(__TEXTPOSZ__,pPosition.z);
    Writer->TextShader->setVec3(__TEXTCOLOR__,pColor);
}
inline void GLUnicodeText::_postGL()
{
    glDisableVertexAttribArray(TextCoordsAttLocation);

    /* restore Blending/no Blending state */
    if (!BlendEnabled)
            glDisable(GL_BLEND);
    /* restore Pixel load alignment */

}

void GLUnicodeText::_render(glm::vec3 pPosition,
                            glm::vec3 pColor,
                            float pSx,
                            float pSy)
{
    _setUpGLState(pPosition,pColor);

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

    _postGL();

}//GLUnicodeText::_render
/**
 * @brief GLUnicodeText::_setupOneChar prepares the coordinates for printing one character described by pChar
 * @param x2
 * @param y2
 * @param w
 * @param h
 * @param wChar
 * @param wCoords
 * @param wC
 * @return
 */
void
GLUnicodeText::_setupOneChar(float &wStartPosX,                 /* starting position x updated to next character position */
                             float &wStartPosY,                 /* starting position y updated to next character position */
                             float wSx,
                             float wSy,
                             GLUnicodeChar* pChar,              /* character data content */
                             zbs::ZArray<textPoint>& wCoords)   /* array point coords table to draw characters */

{

    /* Calculate the vertex and texture coordinates */
    float x2 = wStartPosX + (pChar->bitmap.left * wSx);
    float y2 = wStartPosY - (pChar->bitmap.top * wSy);


    float w = pChar->bitmap.width * wSx;
    float h = pChar->bitmap.height * wSy;

    /* Advance the cursor to the start of the next character */
    wStartPosX += pChar->Advance.x * wSx;

    /* Skip glyphs that have no pixels */
    if (!w || !h)
            return;

//    wCoords[wC++] = (textPoint) {
    wCoords.push((textPoint) {
            x2, -y2,
             pChar->texX,  // texture s coord
             pChar->texY}); // texture t coord
//    wCoords[wC++] = (textPoint) {
    wCoords.push((textPoint) {
            x2 + w, -y2,
             pChar->texX +  pChar->bitmap.width / TexSumWidth,// texture s coord
             pChar->texY});// texture t coord
//    wCoords[wC++] = (textPoint) {
     wCoords.push((textPoint) {
            x2, -y2 - h, // point <x , y>
             pChar->texX, // texture s coord
             pChar->texY +  pChar->bitmap.height / TexSumHeight});// texture t coord
//    wCoords[wC++] = (textPoint) {
    wCoords.push((textPoint) {
            x2 + w, -y2,// point <x , y>
             pChar->texX +  pChar->bitmap.width / TexSumWidth,// texture s coord
             pChar->texY});// texture t coord
//    wCoords[wC++] = (textPoint) {
    wCoords.push((textPoint) {
            x2, -y2 - h, // point <x , y>
             pChar->texX, // texture s coord
             pChar->texY +  pChar->bitmap.height / TexSumHeight});// texture t coord
//    wCoords[wC++] = (textPoint) {
    wCoords.push((textPoint) {
            x2 + w, -y2 - h, // point <x , y>
             pChar->texX +  pChar->bitmap.width / TexSumWidth, // texture s coord
             pChar->texY +  pChar->bitmap.height / TexSumHeight});// texture t coord
//    return wC;
}//_setupOneChar



/**  _renderToBox() rendering rules
 *
 *  if pBoxWidth is mentionned (>0) then text must fit into box width horizontally
 *  if pBoxHeight is mentionned (>0) then text must fit into box after having been cut horizontally if required
 *
 * ADJUSTING WIDTH
 *
 *  RBP_Wrap  :
 *  - RBP_Wrap  not set : text phrase is not wrapped.
 *
 *    todo : process crlf - lf character codepoint
 *
 *    whole text line must fit on one line
 *      + within pBoxWidth if mentionned
 *      + no limit if not mentionned
 *      if pBoxWidth is mentionned and text line does not fit :
 *          . RBP_AdjustFontsize is set : size of characters are adapted for line to fit within given dimensions
 *          . RBP_AdjustFontsize is not set : text line is truncated to BoxWidth (character unit, not word)
 *
 *     if a generic new line character, then text is wrapped according following RBP_Wrap set algorithm.
 *
 * ADJUSTING HEIGHT (Nb: only if RBP_Wrap is set)
 *
 *  - RBP_Wrap  is set : text phrase is wrapped automatically by word whose pattern is defined by GLResources::CuttingCharList
 *
 *    + text is wrapped to a new line starting with cutting character (space, minus, etc...)
 *
 *       pBoxHeight (Box Maximum Height) is not mentionned : following text line is displayed without restriction.
 *          In this case, text is displayed as a never ending column.
 *
 *      pBoxHeight (Box Maximum Height) is mentionned ( > 0 ) :
 *
 *      + if total height for the text does not exceed maximum box height :
 *      new text line is displayed until another text line.
 *
 *      if total height for the text exceeds box maximum height allowed :
 *          . RBP_AdjustFontsize is set :
 *          size of characters are adapted for line to fit within given dimensions :
 *
 * FONT SIZE APPROXIMATION
 *          according the approximation formula :
 *              reduction coef RD starts to 1.0
 *              Do
 *                   RD * 1.2
 *                   approx number of lines ANL = (total size of text in character length * pSx) / RD / pBoxWidth
 *
 *                   approx size of lines ASL = ANL * character max size * pSy / RD
 *
 *              while ASL > pBoxHeight
 *
 *              pSx / RD
 *              pSy / RD
 *
 *          whole text is reprocessed
 *
 *      + if total height for the text does not exceed box maximum height allowed,
 *      new text line is displayed until another text line.
 *
 *
 *
 *
 */
void
GLUnicodeText::_renderToBox(glm::vec3 pBoxPosition,
                            float     pBoxWidth,
                            float     pBoxHeight,
                            glm::vec3 pColor,
                            float pSx, float pSy,
                            uint16_t pFlag
                            )
{
    _setUpGLState(pBoxPosition,pColor);

    float wBoxWidth=(float)(pBoxWidth>0?pBoxWidth:(float)INT_MAX);
    float wBoxHeight =(float)pBoxHeight>0?pBoxHeight:(float)INT_MAX;

//    int wIBoxWidth = IBoxWidth>0?IBoxWidth:INT_MAX;
//    int wIBoxHeight = IBoxHeight>0?IBoxHeight:INT_MAX;

    zbs::ZArray<textPoint> wCoordinates;

    /*
     *  compute how text will fit into box
     */

    float wMaxLineHeight=0;
    float wCurrentLength=0;

    float wDisplayRelPosX=0;
    float wDisplayRelPosY=0;

    int   wNbLines=0;
    long  wPreviousCuttingChar=-1;

    float wSx=pSx;
    float wSy=pSy;

    struct RefLine {
        RefLine() = default;
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

        long  StartIdx;     /* index of TextUChar the line is starting with */
        long  EndIdx;       /* index of TextUChar the line is Ending with */
        float StartPosX;    /* position : 0 most othe cases */
        float StartPosY;    /* height position for the line */
        float MaxHeight;    /* maximum character Height for the line to be printed */
        float MaxAdvanceY;   /* maximum Advance position height for the line to be printed */
        bool  Truncated;    /* is line being truncated or not */
    } ;

    zbs::ZArray<RefLine> Lines;

    RefLine WrapPosition(0,0,0.0,0.0,0.0,false);
    RefLine CurrentLine(0,0,0.0,0.0,0.0,false);
    Lines.push(CurrentLine);
    bool wEndPrint=false;
    bool wTruncated=false;
    int wIdx=0;

    int wICumWidth=0;
    int     wICumHeight=0;

    float wStartPosX = wDisplayRelPosX = pBoxPosition.x + (this->BoxRightMargin * wSx);
    float wStartPosY= wDisplayRelPosY = pBoxPosition.y;

    while ((wIdx < TextLen)&&!wTruncated && !wEndPrint)
    {

    wIdx=CurrentLine.StartIdx;
    WrapPosition = CurrentLine;

    wTruncated = false;
    for (;(wIdx < TextLen) && !wEndPrint ;wIdx++)
        {
        if (wIdx==(TextLen-1))
                {
                CurrentLine.EndIdx=wIdx;
                wEndPrint=true;
                }
            CurrentLine.MaxHeight = std::max(CurrentLine.MaxHeight,UTexChar[wIdx]->bitmap.height * wSy);
            CurrentLine.StartPosX = wDisplayRelPosX;
            CurrentLine.StartPosX = wDisplayRelPosY;
//            CurrentLine.MaxHeight=std::max(wMaxLineHeight,UTexChar[wIdx]->bitmap.height * wSy);
            CurrentLine.MaxHeight=std::max(wMaxLineHeight,UTexChar[wIdx]->bitmap.height );
            CurrentLine.EndIdx = wIdx;

            /* store last cutting character reference*/
            if (UTexChar[wIdx]->isCuttingChar && (wIdx > 0) )
                    {
                    WrapPosition = CurrentLine;
                    }

            if (UTexChar[wIdx]->isNewLineChar) /* finish current line and open new line */
                {
                CurrentLine.EndIdx=wIdx;
                break;
                }
            if ((wDisplayRelPosX + (UTexChar[wIdx]->bitmap.width * wSx)) > wBoxWidth)
                {
                if (pFlag & RBP_LineWrap) /* if no wrap : cut line to current character */
                    {
                    /* wrap text to next line */
 //                       CurrentLine=WrapPosition;
                        CurrentLine.EndIdx=wIdx;
                        break;
                    }
                if (pFlag & RBP_WordWrap) /* if no wrap : cut line to current character */
                    {
                    /* wrap text to next line */
                        CurrentLine=WrapPosition;
                        wIdx=WrapPosition.EndIdx;
//                        CurrentLine.EndIdx=wIdx;
                        break;
                    }
                /* no wrap : end the line is end of display, text is truncated */
                if (pFlag & RBP_TruncChar)
                        CurrentLine.EndIdx=wIdx-1;
                wTruncated=true;
                wEndPrint=true;
                break;
                }

            int wIWidth = (int)UTexChar[wIdx]->bitmap.width;
            wIWidth = (int)UTexChar[wIdx]->Advance.x ;
            wICumWidth+=wIWidth;
            wDisplayRelPosX +=UTexChar[wIdx]->Advance.x * wSx;
            CurrentLine.MaxAdvanceY=std::max(CurrentLine.MaxAdvanceY,(float)UTexChar[wIdx]->Advance.y);
        }// for (wIdx=CurrentLine.StartIdx;(wIdx < TextLen) ;wIdx++)

        /* print line of text to screen from StartIdx to EndIdx */

        wStartPosX = wDisplayRelPosX =  pBoxPosition.x +  (this->BoxRightMargin * wSx);   /* reset x coord to box origin left */
//        wStartPosY = wStartPosY - (CurrentLine.MaxHeight* wSy) ; /* take line height into account */
//                                /* watch out interval between character lines */
        wStartPosY = wStartPosY + (CurrentLine.MaxAdvanceY  * wSy); /* take line height into account */
                                /* watch out interval between character lines */
        wDisplayRelPosY = wStartPosY;

       /* here compute identation for left / right / center adjust */


        /* Loop through all characters for the line */

        for (size_t wi=CurrentLine.StartIdx; wi < CurrentLine.EndIdx; wi++)
            {
            _setupOneChar(wStartPosX,wStartPosY, wSx,wSy,UTexChar[wi],wCoordinates);
            }//for (p = (const uint8_t *)pText; *p; p++)

        if (wTruncated) /* line has been truncated */
                {
                if (pFlag & RBP_TruncChar) /* display truncation character if requested */
                    _setupOneChar(wStartPosX,wStartPosY, wSx,wSy,UTexChar[TextLen],wCoordinates);
                break;
                }
        if ((wStartPosY + (CurrentLine.MaxAdvanceY  * wSy))>= wBoxHeight) /* if next line is out box height boundary : terminate*/
                {
                    if (pFlag & RBP_TruncChar) /* signal truncation ? */
                            _setupOneChar(wStartPosX,wStartPosY, wSx,wSy,UTexChar[TextLen],wCoordinates);
                    break;
                }
        CurrentLine.StartIdx=CurrentLine.EndIdx;
        CurrentLine.EndIdx=0;
        if (UTexChar[wIdx]->isNewLineChar)
                    CurrentLine.StartIdx++; /* skip new line character when encountered and line is processed*/



    } // while  loop until no more characters or truncated




    /* Draw all the character on the screen in one go */
    glBufferData(GL_ARRAY_BUFFER, (int)wCoordinates.usedSize(), wCoordinates.getDataPtr(), GL_DYNAMIC_DRAW);

    Texture->bind();
    glDrawArrays(GL_TRIANGLES, 0, wCoordinates.count());

    _postGL();
} // _renderToBox


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
//        pPosition.y -= ((float)(UTexChar[wi]->Advance.y>>6)) * pSy;
        pPosition.y -= ((float)(UTexChar[wi]->Advance.y)) * pSy;

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


/**
 * @brief GLUnicodeText::render displays text at position pPosition with color pColor
 * @param pPosition
 * @param pColor
 */
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


void GLUnicodeText::setBox (float pBoxWidth,
                            float pBoxHeight,
                            glm::vec3 pColor,
                            uint16_t pBoxFlag,
                            bool pVisible,
                            float pLineSize,
                            float pRightMargin)
{

    glm::vec2 wSCDim=GLResources->getGLWindowSize();

    IBoxWidth = (int)pBoxWidth;
    IBoxHeight = (int)pBoxHeight;

    BoxWidth=pBoxWidth*2.0/wSCDim.x;
    BoxHeight=pBoxHeight*2.0/wSCDim.y;
    BoxVisible = pVisible;
    BoxLineSize=pLineSize;
    BoxFlag = (RBoxPos)pBoxFlag;
    BoxColor=pColor;
    BoxRightMargin=pRightMargin;
    _boxSetupGL();
}

void GLUnicodeText::_boxSetupGL ()
{
    /* setup box coordinates */
     float wTextBoxcoords[15] = {
        0.0      ,        0.0   ,   0.0,
        BoxWidth ,   0.0        ,   0.0,
        BoxWidth ,   -BoxHeight  ,   0.0,
        0.0      ,   -BoxHeight  ,   0.0,
        0.0      ,   0.0        ,   0.0
        };

    /* setup GL context */
    Writer->getBoxShader();
    Writer->BoxShader->use();

    if (BoxVAO==0)
        glGenVertexArrays(1, &this->BoxVAO);
    if (BoxVBO==0)
        glGenBuffers(1, &this->BoxVBO);

    glBindVertexArray(BoxVAO);

    glBindBuffer(GL_ARRAY_BUFFER, BoxVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(wTextBoxcoords), wTextBoxcoords, GL_STATIC_DRAW);

    /* aPosition because ztextbox.vs shader */
    int wAttPosition=Writer->BoxShader->getNamedAttributeLocation("aPosition",true); /* abort on error option set*/
    glVertexAttribPointer(wAttPosition,
                          3,        /* three items */
                          GL_FLOAT, /* type is float */
                          GL_FALSE, /* not to be normalized : use floating point directly */
                          0,        /* no stride */
                          (void*)0);
    glEnableVertexAttribArray(wAttPosition);
   // unbind
    glBindVertexArray(0);
    Writer->BoxShader->release();
}//_boxSetupGL

void GLUnicodeText::_setupMatrices ()
{
    Model =  glm::translate(GLResources->Camera->getModel(), Position);
    View  =  GLResources->Camera->GetViewMatrix();

    Projection = glm::perspective(glm::radians(GLResources->Camera->Zoom),
                                                GLResources->getWindowRatio(),
                                                0.1f,
                                                100.0f);
}

void GLUnicodeText::_drawBox ()
{
float wLineWidth;
    glGetFloatv(GL_LINE_WIDTH,&wLineWidth); /* get current line width */
    if (BoxLineSize>0)
            glLineWidth(BoxLineSize);

    Writer->BoxShader->use();

/*    if (GLDescriptor->hasTexture())
        {
        GLDescriptor->Texture->bind();
        }
        */
    Writer->BoxShader->setMat4(__MODEL__,Model);
    Writer->BoxShader->setMat4(__PROJECTION__,Projection);
    Writer->BoxShader->setMat4(__VIEW__,View);

                            /* DefaultColor because using ztextbox.fs shader*/
    Writer->BoxShader->setVec3("DefaultColor",BoxColor);

    glBindBuffer(GL_ARRAY_BUFFER, BoxVBO);
    glBindVertexArray(BoxVAO);

    glDrawArrays(GL_LINE_LOOP, 0 , 15);

    glLineWidth(wLineWidth);/* restore line width to its previous value */

    // unbind
    glBindVertexArray(0);
    Writer->BoxShader->release();

    return;
}//_drawBox


void GLUnicodeText::renderToBox(glm::vec3 pTextColor)
{

    glm::vec2 wWSize = GLResources->getGLWindowSize();

    float SX =  2.0f / wWSize.x;
    float SY =  2.0f / wWSize.y;

    _setupMatrices ();

    Writer->TextShader->use();

/*    if (GLDescriptor->hasTexture())
        {
        GLDescriptor->Texture->bind();
        }
        */
    Writer->TextShader->setMat4(__MODEL__,Model);
    Writer->TextShader->setMat4(__PROJECTION__,Projection);
    Writer->TextShader->setMat4(__VIEW__,View);



    _renderToBox(glm::vec3(0,0,0),
                 BoxWidth,
                 BoxHeight,
                 pTextColor,
                 SX,SY,
                 BoxFlag
                 );

    _drawBox();

} //renderToBox


GLTextWriter::GLTextWriter(GLenum pTextureEngine)
{
    TextureEngine=pTextureEngine;

    TextShader=new ZShader("zgltext.vs", "zgltext.fs", "UnicodeShader");
}


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
