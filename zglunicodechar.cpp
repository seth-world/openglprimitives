#include <zglunicodechar.h>


void ZGLUnicodeChar::_cloneFrom(ZGLUnicodeChar& pIn)
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
