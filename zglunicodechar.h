#ifndef ZGLUNICODECHAR_H
#define ZGLUNICODECHAR_H

#include <memory.h>  // for malloc, memset
#include <stdlib.h>  // for free
#include <stdint.h>  // for uintxx_t


#ifdef __USE_GLAD__
#include <glad/glad.h>
#else
#include <GL/glew.h>
#endif

#include <glm/glm.hpp>

#include <zglconstants.h>

#include <ztoolset/zcharset.h> // for utfxx_t


#ifndef __BMSTRUCT__
#define __BMSTRUCT__
struct bmstruct{
    float width;    // bitmap width
    float height;   // bitmap height
    float left;     // bearing left : to draw character must position bearing <top,left>
    float top;      // bearing top
};

#endif


class ZGLUnicodeChar
{
private:
    void _cloneFrom(ZGLUnicodeChar& pIn);

public:
    ZGLUnicodeChar()
    {
        clear();
    }

    ZGLUnicodeChar(ZGLUnicodeChar& pIn)
    {
    _cloneFrom(pIn);
    }
    ZGLUnicodeChar(ZGLUnicodeChar* pIn)
    {
    _cloneFrom(*pIn);
    }
    ZGLUnicodeChar& operator = (ZGLUnicodeChar& pIn) {_cloneFrom(pIn); return *this;}

    ~ZGLUnicodeChar()
    {
        if (bitmapBuffer!=nullptr)
                free(bitmapBuffer);
    }

    void clear()
    {
        if (bitmapBuffer!=nullptr)
                free(bitmapBuffer);
        memset(this,0,sizeof(ZGLUnicodeChar));
    }

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

    bool        isCuttingChar   =false;
    bool        isNewLineChar   =false;
    bool        isSpace         =false;

    utf32_t     Codepoint;
} ;//ZGLUnicodeChar



#endif // ZGLUNICODECHAR_H
