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

class ZGLUnicodeFont;
#ifdef __COMMENT__
#define __FONTLOCTYPE__
enum FontLoc_type : uint8_t
{
    FLOC_Default, /* default : First search custom location then local to user then system */
    FLOC_Sytem, /* forces search to system location and disregard other locations */
    FLOC_User,  /* forces search to user local location and disregard other locations */
    FLOC_Adhoc, /* forces search to custom location and disregard other locations */

};
#endif
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
    ~ZFont();

 public:
    int _add(const char* pFontPath, const char* pIntName, const bool pResident);

    int add(const char* pName, const char *pIntName, const uint8_t pLocation);
    ZGLUnicodeFont* newFont();
    static bool _testExist(std::string& pPath);
private:
    bool _testExist();

public:
    const char* Name=nullptr;
//    ZDataBuffer*Content=nullptr;
    std::string FontPath;
    bool        Resident=false;
    FT_Long     UnicodeFaceIndex=-1;
    FT_Face     MainFace=nullptr;

    uint8_t*    FaceContent=nullptr;
    size_t      FaceSize=0;

    zbs::ZArray <ZGLUnicodeFont*> LibFont;
};

class ZGLUnicodeFont
{
private:
    void _copyFrom(ZGLUnicodeFont&pIn)
    {
        Name=pIn.Name;
        Face=pIn.Face;
        FontSize=pIn.FontSize;
        MemResident=pIn.MemResident;
    }
public:
    ZGLUnicodeFont()=default;

    ZGLUnicodeFont(ZGLUnicodeFont&pIn) {_copyFrom(pIn);}

    ZGLUnicodeFont& operator = (ZGLUnicodeFont& pIn) {_copyFrom(pIn); return *this;}

    ZGLUnicodeFont(const char* pFontPath,unsigned int pFontSize,const char* pName)
    {
        if (load(pFontPath,pFontSize,pName)<0)
            abort();
    }

    ~ZGLUnicodeFont() {FT_Done_Face(Face);}

    long   load(const char* pFontPath,unsigned int pFontSize,const char* pName);

const char* Name=nullptr; /*internal name :
                            fonts may be retreived eiher by this name or by its index within FontList */
FT_Face Face;
FT_UInt FontSize;
bool    MemResident=false;
};

#endif // ZFONT_H
