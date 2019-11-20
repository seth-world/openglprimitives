#ifndef ZFONT_H
#define ZFONT_H

#ifdef __USE_GLAD__
#include <glad/glad.h>
#else
#include <GL/glew.h>
#endif

#include <zconfig.h>

#include <stdlib.h> // for abort()

#include <ft2build.h>
#include FT_FREETYPE_H

#include <list>

#include <ztoolset/zdatabuffer.h>

class UnicodeFont;

/**
 * @brief The ZFont class holds the freetype2 font.
 * loads font definition from font file
 *
 * generates a UnicodeFont, operational object for using fonts
 * NB: fonts shader and matrices are hold by TextWriter
 */
class ZFont
{
public:
    ZFont()=default;
    ~ZFont()
    {
        if (FaceContent!=nullptr)
                _free(FaceContent);
    }

 public:
    int add(const char* pFontPath,const char* pName,const bool pResident);
    UnicodeFont* newFont();

private:
    bool _testExist();
public:
    const char* Name=nullptr;
//    ZDataBuffer*Content=nullptr;
    const char* FontPath=nullptr;
    bool        Resident=false;
    FT_Long     UnicodeFaceIndex=-1;
    FT_Face     MainFace=nullptr;

    uint8_t*    FaceContent=nullptr;
    size_t      FaceSize=0;
};

class UnicodeFont
{
private:
    void _copyFrom(UnicodeFont&pIn)
    {
        Name=pIn.Name;
        Face=pIn.Face;
        FontSize=pIn.FontSize;
        MemResident=pIn.MemResident;
    }
public:
    UnicodeFont()=default;

    UnicodeFont(UnicodeFont&pIn) {_copyFrom(pIn);}

    UnicodeFont& operator = (UnicodeFont& pIn) {_copyFrom(pIn); return *this;}

    UnicodeFont(const char* pFontPath,unsigned int pFontSize,const char* pName)
    {
        if (load(pFontPath,pFontSize,pName)<0)
            abort();
    }

    ~UnicodeFont() {FT_Done_Face(Face);}

    long   load(const char* pFontPath,unsigned int pFontSize,const char* pName);

const char* Name=nullptr; /*internal name :
                            fonts may be retreived eiher by this name or by its index within FontList */
FT_Face Face;
FT_UInt FontSize;
bool    MemResident=false;
};

#endif // ZFONT_H
