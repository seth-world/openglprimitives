#include <ztoolset/zerror.h>

#include "zfont.h"

#include <zglresource.h>
//#include <ztoolset/uristring.h>

const char* decodeFTEncoding(FT_Encoding pEncoding)
{
    switch (pEncoding)
    {
    case FT_ENCODING_NONE:
        return "FT_ENCODING_NONE";
    case FT_ENCODING_UNICODE:
        return "FT_ENCODING_UNICODE";
    case FT_ENCODING_PRC:
        return "FT_ENCODING_PRC";
    case FT_ENCODING_BIG5:
        return "FT_ENCODING_NONE";
    case FT_ENCODING_SJIS:
        return "FT_ENCODING_NONE";
    case FT_ENCODING_JOHAB:
        return "FT_ENCODING_JOHAB";
    case FT_ENCODING_WANSUNG:
        return "FT_ENCODING_WANSUNG";
    case FT_ENCODING_APPLE_ROMAN:
        return "FT_ENCODING_APPLE_ROMAN";
    case FT_ENCODING_MS_SYMBOL:
        return "FT_ENCODING_MS_SYMBOL";
    case FT_ENCODING_OLD_LATIN_2:
        return "FT_ENCODING_OLD_LATIN_2";
    case FT_ENCODING_ADOBE_LATIN_1:
        return "FT_ENCODING_ADOBE_LATIN_1";
    case FT_ENCODING_ADOBE_STANDARD:
        return "FT_ENCODING_ADOBE_STANDARD";
    case FT_ENCODING_ADOBE_EXPERT:
        return "FT_ENCODING_ADOBE_EXPERT";

    default:
        return "Unknown freetype character encoding";
    }
}
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>

ZFont::~ZFont()
{
    if (FaceContent!=nullptr)
            _free(FaceContent);
    while (LibFont.count())
            delete LibFont.popR();
}


bool
ZFont::_testExist()
{
    struct stat wStatBuffer;
    int wSt= stat(  FontPath.c_str(),&wStatBuffer);
    return !(wSt<0);
}
bool
ZFont::_testExist(std::string& pPath)
{
    struct stat wStatBuffer;
    int wSt= stat(  pPath.c_str(),&wStatBuffer);
    return !(wSt<0);
}

#include <zio/zdir.h>
/*
int
ZFont::add(const char* pName,const char* pIntName,const uint8_t pLocFlag)
{
std::string wFontPath;

    searchForFont(wFontPath,pName,pLocFlag)
    switch (pLocation)
    {
    case FLOC_User:
    wFontPath=GLResources->_getLinuxLocalFontPath(pFullName);

    if (!_testExist(wFontPath))
    {
        wFontPath=GLResources->_getLinuxSystemFontPath(pGenericName,pFullName);
        if (!_testExist(wFontPath))
        {
            wFontPath=GLResources->_getLinuxSystemFontPath("Liberation","Arial");
            if (!_testExist(wFontPath))
                    return -1;
        }
    }
        break;
    case FLOC_Sytem:
        wFontPath=GLResources->_getLinuxSystemFontPath(pGenericName,pFullName);
        if (!_testExist(wFontPath))
        {
            wFontPath=GLResources->_getLinuxSystemFontPath("Arial","Arial");
            if (!_testExist(wFontPath))
                    return -1;
        }
        break;
    case FLOC_Adhoc:

        wFontPath = GLResources->_buildLinuxAdhocFontPath(pGenericName,pFullName);
        if (!_testExist(wFontPath))
            return -1;

    }// switch

    Name=pIntName;

    return _add(wFontPath.c_str(),pIntName,false);
}
*/
int
ZFont::_add(const char* pFontPath,const char* pIntName,const bool pResident)
{
    Name=pIntName;
    FontPath=pFontPath;
    Resident=pResident;
    FT_Error wFTerr;

    if (!_testExist())
                    return -1;
    // Load font as face
    fprintf (stdout,"ZFont::%s-I Loading font <%s>  as <%s>\n",
             _GET_FUNCTION_NAME_,
             pFontPath,
             pIntName);

    wFTerr=FT_New_Face(GLResources->getFreeTypeLibrary(), pFontPath, 0, &MainFace);
    if (wFTerr!=FT_Err_Ok)
     {
         fprintf (stderr,"ZFont-%s-E Freetype error while loading font into library.\n"
                  "    font file <%s>\n"
                  "    Error <%d>  <%s>\n",
                  _GET_FUNCTION_NAME_,
                  FontPath.c_str(),
                  wFTerr,
                  getFTErrorString( wFTerr));
         return -1;
     }
    fprintf (stdout,
             "    Number of faces  <%ld> glyphs <%ld> scalable <%s> vertical data <%s>\n"
             "    Family    <%s> style <%s>  \n"
              "    Encodings available <%d>------------------\n"
             ,
             MainFace->num_faces,
             MainFace->num_glyphs,
             (MainFace->face_flags& FT_FACE_FLAG_SCALABLE)==FT_FACE_FLAG_SCALABLE?"Yes":"No",
             (MainFace->face_flags& FT_FACE_FLAG_VERTICAL)==FT_FACE_FLAG_VERTICAL?"Yes":"No",
             MainFace->family_name,
             MainFace->style_name,
             MainFace->num_charmaps);

    for (int wi=0;wi<MainFace->num_charmaps;wi++)
    {
        fprintf (stdout,
                 "    <%s>\n"
                 ,
                 decodeFTEncoding( MainFace->charmaps[wi]->encoding));
    }
    return 0;
}//ZFont::add

/** generates a face from font according a size pSize
 *
 *  this face could be resident in memory or not according Resident parameter
 *
*/
ZGLUnicodeFont*
ZFont::newFont()
{
FT_Error wFTerr;
ZGLUnicodeFont* wUFont=new ZGLUnicodeFont();

    wUFont->Name=Name;
    if (Resident)
    {
     wFTerr=FT_New_Memory_Face(GLResources->getFreeTypeLibrary(),
                               FaceContent,
                               FaceSize,
                               MainFace->face_index,
                               &wUFont->Face);


    }
    else {
        wFTerr=FT_New_Face(GLResources->getFreeTypeLibrary(),
                           FontPath.c_str(),
                           0,
                           &wUFont->Face);
        }
    if (wFTerr!=FT_Err_Ok)
        {
        fprintf (stderr,
                 "ZFont::%s-E Freetype error while creating freetype face from library.\n"
                 "    font <%s>  Error <%d>  <%s>\n",
                 _GET_FUNCTION_NAME_,
                 Name,
                 wFTerr,
                 getFTErrorString( wFTerr));
        return nullptr;
       }
    wFTerr=FT_Select_Charmap(wUFont->Face, FT_ENCODING_UNICODE);
    if (wFTerr!=FT_Err_Ok)
        {
        fprintf (stderr,
                 "ZFont::%s-E Freetype error while selecting Unicode charmap from library.\n"
                 "    font <%s>  Error <%d>  <%s>\n",
                 _GET_FUNCTION_NAME_,
                 Name,
                 wFTerr,
                 getFTErrorString( wFTerr));
        return nullptr;
       }


    LibFont.push(wUFont);

    return wUFont;
}//newFont

// instantiations

template<const unsigned char> size_t utfStrlen(const unsigned char* pString);
template<unsigned char> size_t utfStrlen(unsigned char* pString);

