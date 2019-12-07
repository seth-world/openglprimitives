#include <zglresource.h>

#include <ztexture.h>
#include <zobject.h>
#include <zshader.h>

#include <zfont.h>
#include <string.h> // for strcmp

#include <camera.h>
#include <ztoolset/utffixedstring.h>
#include <ztoolset/uristring.h>

ZGLResource _GLResources;
ZGLResource* GLResources=&_GLResources;

/* Freetype error description management as described per Freetype doc */
const char* getFTErrorString(const FT_Error error_code)
{
#undef FTERRORS_H_
#define FT_ERROR_START_LIST     switch ( error_code ) {
#define FT_ERRORDEF( e, v, s )    case v: return s;
#define FT_ERROR_END_LIST       }
#include FT_ERRORS_H
    return "Unknown FT error";
}




ZGLResource::~ZGLResource()
    {
    while (FontList.count())
                delete FontList.popR();
    closeFreeType();
    cleanAll();
    }


void
ZGLResource::deregisterObject(ZObject* pObject)
{
    for (long wi=0;wi<Objects.count();wi++)
        if (pObject==Objects[wi])
        {
            Objects[wi]->deleteGLContext();
            Objects.erase(wi);
            return;
        }
    return;
}
void ZGLResource::deregisterShader(ZShader* pObject)
{
    for (long wi=0;wi<Shaders.count();wi++)
        if (pObject==Shaders[wi])
        {
            Shaders[wi]->deleteGLContext();
            Shaders.erase(wi);
            return;
        }
    return;
}
void ZGLResource::deregisterTexture(ZTexture* pObject)
{
    for (long wi=0;wi<Textures.count();wi++)
        if (pObject==Textures[wi])
        {
            Textures[wi]->deleteGLContext();
            Textures.erase(wi);
            return;
        }
    return;
}


void ZGLResource::cleanAll()
{
    while (Textures.count())
            Textures.popR()->deleteGLContext();
    while (Objects.count())
            Objects.popR()->deleteGLContext();
    return;
}

ZShader ZGLResource::loadShader(const char*pVPath,const char*pFPath, const char*pGPath, const char*pName)
{
    return ZShader(pVPath,pFPath,pGPath,pName);
}



long  ZGLResource::addFont(const char *pFontName,const char*pIntName, const FontLoc_type pLocFlag)
{
    std::string wFontPath;
    long wRet=(long)searchForFont(wFontPath,pFontName,pLocFlag);
    if (wRet<0)
            return wRet;

    ZFont* wFont=new ZFont();
    wRet=wFont->_add(wFontPath.c_str(),pIntName,false);
    if (wRet<0)
            return wRet;
    return FontList.push(wFont);
}
std::string ZGLResource::_winGetSystemFontPath(const char* pFontName)
{
    std::string wFontPath;
    wFontPath = getenv(__WINFONTSYSTEMROOTENV__);
    wFontPath += __WINFONTSYSTEMDIR__;
    wFontPath += pFontName;
    wFontPath += __FONTFILESUFFIX__;
    return wFontPath;
}

#ifdef __USE_WINDOWS__
const char DirectoryDelimiter='\\';
#else
const char DirectoryDelimiter='/';
#endif

std::string&
_addConditionalDelimiter(std::string& pString)
{
    std::string::iterator wPos= pString.end();
    wPos--;
    if (*wPos==DirectoryDelimiter)
            return pString;
    pString += DirectoryDelimiter;
    return pString;
}
bool _testConditionalDelimiter(std::string& pString)
{
    std::string::iterator wPos= pString.end();
    wPos--;
    if (*wPos==DirectoryDelimiter)
            return true;
    return false;
}


std::string ZGLResource::_getLinuxSystemFontPath(const char* pGenericName, const char*pFileName)
{
    std::string wFontPath;
    wFontPath += __LINUXFONTSYSTEMDIR__;
    _addConditionalDelimiter(wFontPath);
    wFontPath += pGenericName;  /* font files are located within a subdirectory named as generic font family */
    wFontPath += "/";
    wFontPath += pFileName;
    wFontPath += __FONTFILESUFFIX__;

    return wFontPath;
}
std::string ZGLResource::_linuxGetLocalFontPath(const char* pFontName)
{
    std::string wFontPath;
    wFontPath=_linuxGetLocalFontDir();
    _addConditionalDelimiter(wFontPath);
    wFontPath += pFontName;
    wFontPath += __FONTFILESUFFIX__;
    return wFontPath;
}

std::string ZGLResource::_buildLinuxAdhocFontPath(const char*pGenericName,const char*pFullName)
{
    std::string wFontPath;
    if (pGenericName!=nullptr)
        {
        wFontPath =pGenericName;  /* Remark : may include directory path  */
        _addConditionalDelimiter(wFontPath);
        }
    wFontPath += pFullName;
    wFontPath += __FONTFILESUFFIX__;
    return wFontPath;
}

#include <zio/zdir.h>
#include <ztoolset/uristring.h>
std::string ZGLResource::_linuxGetLocalFontDir()
{
    std::string wFontPath;
    wFontPath = getenv(__LINUXFONTLOCALROOTENV__);
    wFontPath +=(const char*) __LINUXFONTLOCALDIR__;  /* Remark : no subdirectory  */
    return wFontPath;
}
std::string ZGLResource::_linuxGetSystemRootFontDir()
{
    std::string wFontPath;
    wFontPath +=(const char*) __LINUXFONTSYSTEMDIR__;  /* Remark : no subdirectory  */
    return wFontPath;
}

std::string ZGLResource::_winGetLocalFontDir()
{
    std::string wFontPath;
    wFontPath = getenv(__WINFONTLOCALROOTENV__);
    wFontPath += __WINFONTLOCALDIR__;
}
std::string ZGLResource::_winGetSystemFontDir()
{
    std::string wFontPath;
    wFontPath += getenv(__WINFONTSYSTEMROOTENV__);
    wFontPath +=(const char*) __WINFONTLOCALROOTENV__;  /* Remark : no subdirectory  */
    return wFontPath;
}



int ZGLResource::searchForFont(std::string &pFontPath, const char* pFontName, FontLoc_type pLocFlag)
{

    switch (pLocFlag)
    {
    case FLOC_Adhoc:
        if (ZFont::_testExist(pFontPath))
                return 0;
        return -1;
    case FLOC_User:
        return searchForLocalFont(pFontPath,pFontName);
    case FLOC_Sytem:
        return searchForSystemFont(pFontPath,pFontName);
    case FLOC_Default:
        if (searchForLocalFont(pFontPath,pFontName)<0)
        {
            return searchForSystemFont(pFontPath,pFontName);
        }
    }//switch

}//searchForFont

int ZGLResource::searchForLocalFont(std::string &pFontPath, const char* pFontName)
{
    std::string wFontPath;
#ifdef __USE_WINDOWS__
    wFontPath=_winGetLocalFontDir();
#else
    wFontPath=_linuxGetLocalFontDir();
#endif
    uriString wDirEntry;
    ZDir wFontDir;
    if (wFontDir.setPath((const utf8_t*)wFontPath.c_str())!=ZS_SUCCESS)
            return -1;

    while (wFontDir.readDir(wDirEntry,ZDFT_RegularFile)==ZS_SUCCESS)
        {
            if (wDirEntry!=(const utf8_t*)pFontName)
                    continue;
            pFontPath=wDirEntry.toCString_Strait();
            return 0;
        }
    return -1;
}

int ZGLResource::searchForSystemFont(std::string &pFontPath, const char* pFontName)
{
    std::string wFontPath;
    uriString wFontFamily,wDirEntry;
    ZDir wFontDir;
    ZDir wFontDirFamily;
#ifndef __USE_WINDOWS__
/* for linux */
    wFontPath=_linuxGetSystemRootFontDir();
    if (wFontDir.setPath((const utf8_t*)wFontPath.c_str())!=ZS_SUCCESS)
            return -1;
    while (wFontDir.readDir(wFontFamily,ZDFT_Directory)==ZS_SUCCESS)
    {
        if (wFontDirFamily.setPath(wFontFamily.toUtf())!=ZS_SUCCESS)
                                continue;
        while (wFontDirFamily.readDir(wDirEntry,ZDFT_RegularFile)==ZS_SUCCESS)
        {
            if (wDirEntry.getRootBasename()!=(const utf8_t*)pFontName)
                    continue;
            pFontPath = wDirEntry.toCString_Strait();
            return 0;
        }
    }
    return -1;
#else
/* for windows */
    wFontPath=_winGetSystemFontDir();

    if (wFontDir.setPath((const utf8_t*)wFontPath.c_str())!=ZS_SUCCESS)
            return -1;
    while (wFontDir.readDir(wDirEntry,ZDFT_Directory)==ZS_SUCCESS)
        {
        if (wDirEntry.getRootBasename()!=(const utf8_t*)pFontName)
                continue;
        pFontPath = wDirEntry.toCString_Strait();
        return 0;
        }
    return -1;
#endif // __USE_WINDOWS__
}



ZGLUnicodeFont* ZGLResource::getFontByName(const char* pName)
{
    for (long wi=0;wi<FontList.count();wi++)
            if (strcmp(FontList[wi]->Name,pName)==0)
                        return FontList[wi]->newFont();

    return nullptr;
}



ZGLUnicodeFont* ZGLResource::getFont(const long pFontIdx,size_t pFontsize)
{
    return FontList[pFontIdx]->newFont();
}

void ZGLResource::initFreeType()
{

    FT_Error wFTerr    = FT_Init_FreeType(&FreetypeLib);
    if (wFTerr!=FT_Err_Ok) // All functions return a value different than 0 whenever an error occurred
        {
            fprintf (stderr,"ZGLResource::Load-E Freetype error while initializing Freetype library.\n"
                     "    Error <%d>  <%s>\n",
                     wFTerr,
                     getFTErrorString( wFTerr));
            abort();
        }
}
void ZGLResource::closeFreeType()
{
    if (FreetypeLib != nullptr)
            FT_Done_FreeType(FreetypeLib);
    FreetypeLib=nullptr;
}


void ZGLResource::registerZCamera (ZCamera* pCamera)
{
    Camera=pCamera;
}

/* Font local routines */

void ZGLResource::_linuxListFonts(FILE* pOutput)
{
    std::string wFontPath;
    wFontPath = getenv(__LINUXFONTLOCALROOTENV__);
    wFontPath +=(const char*) __LINUXFONTLOCALDIR__;  /* Remark : no subdirectory  */
    ZDir wFontLocalDir;
    if (wFontLocalDir.setPath((const utf8_t*)wFontPath.c_str())!=ZS_SUCCESS)
        {
        fprintf (stderr,"%s-F-CNTFIND Cannot find directory %s",wFontPath.c_str());
        }
    ZDir wFontDirFamily ;
    uriString wFontFamily;
    uriString wMan;
    fprintf(pOutput,
            "--Local fonts........................\n");
    while (wFontLocalDir.readDir(wFontFamily,ZDFT_RegularFile)==ZS_SUCCESS)
        fprintf(pOutput,
                "       <%s>\n",wFontFamily.getBasename().toUtf());

    ZDir wFontSysDir((const utf8_t*)__LINUXFONTSYSTEMDIR__);
    fprintf(pOutput,
            "--System fonts........................\n");
    while (wFontSysDir.readDir(wFontFamily,ZDFT_Directory)==ZS_SUCCESS)
    {
        fprintf(pOutput,
                "Family <%s>\n",wFontFamily.getBasename().toUtf());
        wMan = wFontFamily ;
        wFontDirFamily.setPath(wMan.toUtf());
        while (wFontDirFamily.readDir(wFontFamily,ZDFT_RegularFile)==ZS_SUCCESS)
            fprintf(pOutput,
                    "     <%s>\n",wFontFamily.getBasename().toUtf());
    }

    return;
}//_linuxListFonts

void ZGLResource::_linuxListSystemFonts(FILE* pOutput)
{
    ZDir wFontDirFamily ;
    uriString wFontFamily;
    uriString wMan;

    ZDir wFontSysDir((const utf8_t*)__LINUXFONTSYSTEMDIR__);
    fprintf(pOutput,
            "--System fonts........................\n");
    while (wFontSysDir.readDir(wFontFamily,ZDFT_Directory)==ZS_SUCCESS)
    {
        fprintf(pOutput,
                "Family <%s>\n",wFontFamily.getBasename().toUtf());
        wMan = wFontFamily ;
        wFontDirFamily.setPath(wMan.toUtf());
        while (wFontDirFamily.readDir(wFontFamily,ZDFT_RegularFile)==ZS_SUCCESS)
            fprintf(pOutput,
                    "     <%s>\n",wFontFamily.getBasename().toUtf());
    }

    return;
}//_linuxListSystemFonts

void ZGLResource::_linuxListLocalFonts(FILE* pOutput)
{
    std::string wFontPath;
    wFontPath = getenv(__LINUXFONTLOCALROOTENV__);
    wFontPath +=(const char*) __LINUXFONTLOCALDIR__;  /* Remark : no subdirectory  */
    ZDir wFontLocalDir;
    if (wFontLocalDir.setPath((const utf8_t*)wFontPath.c_str())!=ZS_SUCCESS)
        {
        fprintf (stderr,"%s-F-CNTFIND Cannot find directory %s",wFontPath.c_str());
        }
    ZDir wFontDirFamily ;
    uriString wFontFamily;
    uriString wMan;
    fprintf(pOutput,
            "--Local fonts........................\n");
    while (wFontLocalDir.readDir(wFontFamily,ZDFT_RegularFile)==ZS_SUCCESS)
        fprintf(pOutput,
                "       <%s>\n",wFontFamily.getBasename().toUtf());
    return;
}//_linuxListLocalFonts

void ZGLResource::_linuxSearchFonts(const utf8_t* pSearch,FILE* pOutput)
{
    fprintf(pOutput,
            "Searching font files names matching <%s>\n",pSearch);
    std::string wFontPath;
    wFontPath = getenv(__LINUXFONTLOCALROOTENV__);
    wFontPath +=(const char*) __LINUXFONTLOCALDIR__;  /* Remark : no subdirectory  */
    ZDir wFontLocalDir;
    if (wFontLocalDir.setPath((const utf8_t*)wFontPath.c_str())!=ZS_SUCCESS)
        {
        fprintf (stderr,"%s-F-CNTFIND Cannot find directory %s",wFontPath.c_str());
        }
    ZDir wFontDirFamily ;
    uriString wFontFamily;
    uriString wFontFile;
    uriString wMan;
    fprintf(pOutput,
            "--Local fonts........................\n");
    while (wFontLocalDir.readDir(wFontFile,ZDFT_RegularFile)==ZS_SUCCESS)
    {
        if (wFontFile.strcasestr(pSearch)==nullptr)
                continue;
        fprintf(pOutput,
                "       <%s>\n",wFontFile.getBasename().toUtf());
    }
    ZDir wFontSysDir((const utf8_t*)__LINUXFONTSYSTEMDIR__);
    fprintf(pOutput,
            "--System fonts........................\n");
    while (wFontSysDir.readDir(wFontFamily,ZDFT_Directory)==ZS_SUCCESS)
    {
        bool wF=false;

        wMan = wFontFamily ;
        wFontDirFamily.setPath(wMan.toUtf());
        while (wFontDirFamily.readDir(wFontFile,ZDFT_RegularFile)==ZS_SUCCESS)
        {
            if (wFontFile.strcasestr(pSearch)==nullptr)
                    continue;
            if (!wF)
            {
                fprintf(pOutput,
                                "Family <%s>\n",wFontFamily.getBasename().toUtf());
                wF=true;
            }
            fprintf(pOutput,
                    "     <%s>\n",wFontFile.getBasename().toUtf());
        }
    }

    return;
}//_linuxListFonts

/* windows */
void ZGLResource::_winListFonts(FILE* pOutput)
{
    std::string wFontPath;
    wFontPath = _winGetLocalFontDir();
    ZDir wFontLocalDir;
    if (wFontLocalDir.setPath((const utf8_t*)wFontPath.c_str())!=ZS_SUCCESS)
        {
        fprintf (stderr,"%s-F-CNTFIND Cannot find directory %s",wFontPath.c_str());
        }
    ZDir wFontDirFamily ;
    uriString wFontFamily;
    uriString wMan;
    fprintf(pOutput,
            "--Local fonts........................\n");
    while (wFontLocalDir.readDir(wFontFamily,ZDFT_RegularFile)==ZS_SUCCESS)
        fprintf(pOutput,
                "       <%s>\n",wFontFamily.getBasename().toUtf());

    ZDir wFontSysDir((const utf8_t*)_winGetSystemFontDir().c_str());
    fprintf(pOutput,
            "--System fonts........................\n");
    while (wFontSysDir.readDir(wFontFamily,ZDFT_Directory)==ZS_SUCCESS)
        {
        fprintf(pOutput,
                "     <%s>\n",wFontFamily.getBasename().toUtf());
        }

    return;
}//_winListFonts

void ZGLResource::_winListSystemFonts(FILE* pOutput)
{
    ZDir wFontDirFamily ;
    uriString wFontFamily;
    uriString wMan;

    ZDir wFontSysDir((const utf8_t*)_winGetSystemFontDir().c_str());
    fprintf(pOutput,
            "--System fonts........................\n");
    while (wFontSysDir.readDir(wFontFamily,ZDFT_Directory)==ZS_SUCCESS)
    {
            fprintf(pOutput,
                    "     <%s>\n",wFontFamily.getBasename().toUtf());
    }

    return;
}//_winListSystemFonts

void ZGLResource::_winListLocalFonts(FILE* pOutput)
{
    std::string wFontPath;
    wFontPath = _winGetLocalFontDir();
    ZDir wFontLocalDir;
    if (wFontLocalDir.setPath((const utf8_t*)wFontPath.c_str())!=ZS_SUCCESS)
        {
        fprintf (stderr,"%s-F-CNTFIND Cannot find directory %s",wFontPath.c_str());
        }
    ZDir wFontDirFamily ;
    uriString wFontFamily;
    uriString wMan;
    fprintf(pOutput,
            "--Local fonts........................\n");
    while (wFontLocalDir.readDir(wFontFamily,ZDFT_RegularFile)==ZS_SUCCESS)
        fprintf(pOutput,
                "       <%s>\n",wFontFamily.getBasename().toUtf());
    return;
}//_winListLocalFonts

void ZGLResource::_winSearchFonts(const utf8_t* pSearch,FILE* pOutput)
{
    fprintf(pOutput,
            "Searching font files names matching <%s>\n",pSearch);
    std::string wFontPath;
    wFontPath = _winGetLocalFontDir();
    ZDir wFontLocalDir;
    if (wFontLocalDir.setPath((const utf8_t*)wFontPath.c_str())!=ZS_SUCCESS)
        {
        fprintf (stderr,"%s-F-CNTFIND Cannot find directory %s",wFontPath.c_str());
        }
    ZDir wFontDirFamily ;
    uriString wFontFamily;
    uriString wFontFile;
    uriString wMan;
    fprintf(pOutput,
            "--Local fonts........................\n");
    while (wFontLocalDir.readDir(wFontFile,ZDFT_RegularFile)==ZS_SUCCESS)
    {
        if (wFontFile.strcasestr(pSearch)==nullptr)
                continue;
        fprintf(pOutput,
                "       <%s>\n",wFontFile.getBasename().toUtf());
    }
    ZDir wFontSysDir((const utf8_t*)_winGetSystemFontDir().c_str());
    fprintf(pOutput,
            "--System fonts........................\n");
    while (wFontSysDir.readDir(wFontFile,ZDFT_Directory)==ZS_SUCCESS)
    {
            if (wFontFile.strcasestr(pSearch)==nullptr)
                    continue;
            fprintf(pOutput,
                    "     <%s>\n",wFontFile.getBasename().toUtf());
    }

    return;
}//_winListFonts





