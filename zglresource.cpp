#include <zglresource.h>

#include <ztexture.h>
#include <zobject.h>
#include <zshader.h>
#include <zcandyprofile.h>

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
            Objects.erase((size_t)wi);
            return;
        }
    return;
}


long ZGLResource::registerTextureBase(_TextureBase* pTexture)
{
    /* texture must not already exist within registrated textures */
    for (long wi=0;wi<Textures.count();wi++)
            if (Textures[wi]==pTexture)
                    {
                    fprintf (stdout,"ZGLResource::registerTextureBase-W-DBL Double texture registration for <%s> \n",
                             pTexture->Name);
                    return wi; /* already existing : return actual rank */
                    }
    /* else create */
    return Textures.push(pTexture);
}

long ZGLResource::registerCandyProfile(_ZCandyProfileBase* pProfile)
{
    /* texture must not already exist within registrated textures */
    for (long wi=0;wi<CandyProfiles.count();wi++)
            if (CandyProfiles[wi]==pProfile)
                    {
                    fprintf (stdout,"ZGLResource::registerCandyProfile-W-DBL Double candy profile registration for <%s> \n",
                             pProfile->Name);
                    return wi; /* already existing : return actual rank */
                    }
    /* else register */
    return CandyProfiles.push(pProfile);
}


void ZGLResource::deregisterCandyProfileBase(_ZCandyProfileBase* pProfile)
{
    if (pProfile==nullptr)
            return;
    for (long wi=0;wi<CandyProfiles.count();wi++)
        if (pProfile==CandyProfiles[wi])
        {
            if (CandyProfiles[wi]->unShare())/* unshare returns true if no more references exist*/
                    {
                    delete CandyProfiles[wi];
                    CandyProfiles.erase(wi);
                    }

            return;
        }
    return;
}

#ifdef __COMMENT__
long ZGLResource::registerTextTextureBase(_TextureBase* pTexture)
{
    /* shader must not already exist */
    for (long wi=0;wi<Textures.count();wi++)
            if (Textures[wi]==pTexture)
                    return wi; /* already existing : return actual rank */
    /* else create */
    char* wName=(char*)malloc(25);
    memset(wName,0,25);
    sprintf(wName,"TextTexture-%ld",Textures.lastIdx()+1);
    pTexture->Name=wName;
    return Textures.push(pTexture);
}
#endif // __COMMENT__

long ZGLResource::registerShaderBase(_ShaderBase* pShader)
{
    /* shader must not already exist */
    for (long wi=0;wi<Shaders.count();wi++)
            if (Shaders[wi]==pShader)
                    return wi; /* already existing : return actual rank */
    /* else create */
    return Shaders.push(pShader);
}

void ZGLResource::deregisterShader(ZShader &pShader)
{
    deregisterShaderBase(pShader.ShaderBase);
    pShader.ShaderBase=nullptr;
    return;
}

void ZGLResource::deregisterShaderBase(_ShaderBase* pShader)
{
    if (pShader==nullptr)
            return;
    for (long wi=0;wi<Shaders.count();wi++)
        if (pShader==Shaders[wi])
        {
            if (Shaders[wi]->unShare())/* unshare returns true if no more references exist*/
                    {
                    delete Shaders[wi];
                    Shaders.erase(wi);
                    }

            return;
        }
    return;
}
void ZGLResource::deregisterTextureBase(_TextureBase* pTexture)
{
    if (pTexture==nullptr)
            return;
    for (long wi=0;wi<Textures.count();wi++)
        if (pTexture==Textures[wi])
        {
            if (Textures[wi]->unShare()) /* unshare returns true if no more references exist*/
                    {
                    delete Textures[wi];
                    Textures.erase(wi);
                    }
            return;
        }
    return;
}
void ZGLResource::listTextures(FILE* pOutput)
{
    fprintf (pOutput,
             "--------Registrated textures------------------\n"
             "Rank  GLId Shared            Name\n");
    for (long wi=0;wi < Textures.count();wi++)
        {
        fprintf (pOutput,
                 "%3ld> <%3u> <%4d> <%25s>\n",
                 wi,
                 Textures[wi]->GLId,
                 Textures[wi]->Shared,
                 Textures[wi]->Name);
        }
    fprintf (pOutput,
             "-----------------------------------------------\n");
}
void ZGLResource::listShaders(FILE* pOutput)
{
    fprintf (pOutput,
             "--------Registrated Shaders ---------------------------------------------------\n"
             "Rank  GLId Shared            Name                         Matrices             \n");
    for (long wi=0;wi < Shaders.count();wi++)
        {
        fprintf (pOutput,
                 "%3ld> <%3u> <%4d> <%25s> | ",
                 wi,
                 Shaders[wi]->GLId,
                 Shaders[wi]->Shared,
                 Shaders[wi]->Name);

        fprintf (pOutput,"%s ",
                 glGetUniformLocation(Shaders[wi]->GLId,__SHD_MODEL__)>=0?"Model":"--");
        fprintf (pOutput,"%s ",
                 glGetUniformLocation(Shaders[wi]->GLId,__SHD_VIEW__)>=0?"View":"--");
        fprintf (pOutput,"%s ",
                 glGetUniformLocation(Shaders[wi]->GLId,__SHD_PROJECTION__)>=0?"Projection":"--");
        fprintf (pOutput,"%s ",
                 glGetUniformLocation(Shaders[wi]->GLId,__SHD_NORMAL__)>=0?"Normal":"--");
        fprintf (pOutput,"\n");
        }
    fprintf (pOutput,
             "-----------------------------------------------\n");
}

void ZGLResource::listObjects(FILE* pOutput)
{
    fprintf (pOutput,
             "--------Registrated Objects--------------------\n"
             "Rank  Type            Name\n");
    for (long wi=0;wi < Objects.count();wi++)
        {
        fprintf (pOutput,
                 "%3ld> <%15s> <%25s> ",
                 wi,
                 decodeObjectType(Objects[wi]->Type),
                 Objects[wi]->Name);
        if  (Objects[wi]->isChild())
        {
          fprintf (pOutput,
                   " belongs to %s <%s>",
                   decodeObjectType(Objects[wi]->getFatherType()),
                   Objects[wi]->getFatherName());
        }
        else
        {
            fprintf (pOutput,
                     " Standalone\n");
        }

        fprintf (pOutput,"          Shader contexts ");
        for (int wj=0;wj<MaxShaderContext;wj++)
                {
                if (Objects[wi]->ShaderContext[wj])
                    {
                        fprintf (pOutput,"[%s: <%s> rules<%ld>]",
                                 decodeShdCtx(wj),
                                 Objects[wi]->ShaderContext[wj]->getShaderName(),
                                 Objects[wi]->ShaderContext[wj]->count());
                    }
                }
        fprintf (pOutput,
                 "\n          Descriptors     ");
        for (int wj=0;wj<MaxShaderContext;wj++)
                if (Objects[wi]->GLDesc[wj])
                        fprintf (pOutput,"[%s: <%s> <%s>] ",
                                 decodeShdCtx(wj),
                                 Objects[wi]->GLDesc[wj]->VertexData?"Vertex":"--",
                                 Objects[wi]->GLDesc[wj]->Indexes?"Indexes":"--");
        fprintf (pOutput,"\n");
        }
    fprintf (pOutput,
             "-----------------------------------------------\n");
}
void ZGLResource::deregisterTexture(ZTexture* pTexture)
{
    if (pTexture==nullptr)
            return;
    deregisterTextureBase(pTexture->TextureBase);
    return;
}


void ZGLResource::cleanAll()
{

    while (Shaders.count())
            delete Shaders.popR();
    while (Objects.count())
            Objects.popR()->deleteGLContext();
    while (Textures.count())
            {
//debug
            _TextureBase* wT= Textures.popR();
            delete wT;
//            delete Textures.popR();
            }

    while (CandyProfiles.count())
            delete CandyProfiles.popR();
    return;
}//cleanAll

/*------------Shaders--------------------------------*/

long ZGLResource::loadShader(const char*pVPath,const char*pFPath, const char*pGPath, const char*pIntlName)
{
    _ShaderBase* wSh= _InitShaderGeometry(pVPath,pFPath,pGPath ,pIntlName);
    if (!wSh)
            return -1;
    return registerShaderBase(wSh);
 //   ZShader* wSh=new ZShader(pVPath,pFPath,pGPath,pIntlName);
 //   return registerShader(wSh);
}

long ZGLResource::loadShader(const char*pVPath,const char*pFPath, const char*pIntlName)
{
    _ShaderBase* wSh= _InitShader(pVPath,pFPath ,pIntlName);
    if (!wSh)
            return -1;
    return registerShaderBase(wSh);

 //   ZShader* wSh=new ZShader(pVPath,pFPath,pIntlName);
 //   return registerShader(wSh);
}

ZShader ZGLResource::getShaderByName(const char* pIntlName)
{
    for (long wi=0;wi<Shaders.count();wi++)
            if (strcmp(Shaders[wi]->Name,pIntlName)==0)
                        return ZShader(Shaders[wi]->share());

    return ZShader(nullptr);
}
ZShader ZGLResource::getShaderByNameCase(const char* pIntlName)
{
    for (long wi=0;wi<Shaders.count();wi++)
            if (strcasecmp(Shaders[wi]->Name,pIntlName)==0)
                        return ZShader(Shaders[wi]->share());

    return nullptr;
}
ZShader ZGLResource::getShaderByRank(const long pIdx)
{
    if (!Shaders.exists(pIdx))
                    return nullptr;
    return Shaders[pIdx]->share();
}

ZShader ZGLResource::getActiveShader()
{
GLint wCurShader=0;
    glGetIntegerv(GL_CURRENT_PROGRAM,&wCurShader);
    if (wCurShader==0)
        {
        fprintf (stderr,"ZGLResource::getActiveShader-E-NOSHD No current shader is active at this time\n");
        return nullptr;
        }
    for (long wi=0;wi<Shaders.count();wi++)
        {
        if (Shaders[wi]->GLId==wCurShader)
                return Shaders[wi]->share();
        }
    fprintf (stderr,"ZGLResource::getActiveShader-E-NOTRGTD Current shader id <%d> is not registrated within GL resources.\n",(int)wCurShader);
    exit (EXIT_FAILURE);
}//getActiveShader

ZShader* ZGLResource::getShaderByNamePtr(const char* pIntlName)
{
    for (long wi=0;wi<Shaders.count();wi++)
            if (strcmp(Shaders[wi]->Name,pIntlName)==0)
                        return Shaders[wi]->sharePtr();

    return nullptr;
}
ZShader* ZGLResource::getShaderByNameCasePtr(const char* pIntlName)
{
    for (long wi=0;wi<Shaders.count();wi++)
            if (strcasecmp(Shaders[wi]->Name,pIntlName)==0)
                        return Shaders[wi]->sharePtr();

    return nullptr;
}
ZShader* ZGLResource::getShaderByRankPtr(const long pIdx)
{
    if (!Shaders.exists(pIdx))
                    return nullptr;
    return Shaders[pIdx]->sharePtr();
}

ZShader* ZGLResource::getActiveShaderPtr()
{
GLint wShID=0;
    glGetIntegerv(GL_CURRENT_PROGRAM,&wShID);
    if (wShID==0)
        {
        fprintf (stderr,"ZGLResource::getActiveShader-E-NOSHD No current shader is active at this time\n");
        return nullptr;
        }
    for (long wi=0;wi<Shaders.count();wi++)
        {
        if (Shaders[wi]->GLId==wShID)
                return Shaders[wi]->sharePtr();
        }

    fprintf (stderr,
             "ZGLResource::getActiveShader-E-NOTRGTD Current shader id <%d> is not registrated within GL resources.\n",
             (int)wShID);

    listRegistratedShaders();

    exit (EXIT_FAILURE);
}//getActiveShaderPtr


void ZGLResource::listRegistratedShaders(FILE*pOutput)
{
    fprintf (pOutput,
             "\n List of registrated shaders\n"
             " Rank GL-id Name\n");
    for (long wi=0;wi<Shaders.count();wi++)
        {
        fprintf (pOutput,
                 "<%3ld> <%3u> <%s>\n",
                 wi,
                 Shaders[wi]->GLId,
                 Shaders[wi]->Name);
        }
}

/*------------------Fonts-------------------------------*/

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
_RTrim(std::string& pString)
{
    std::string::iterator wPos= pString.end();
    wPos--;
    while ((wPos>pString.begin())&&(*wPos==' ')) /* trim trailing spaces */
                wPos--;
    pString.resize((wPos-pString.begin())+1);
    return pString;
}

std::string&
_addConditionalDelimiter(std::string& pString)
{

    _RTrim(pString); /* trim trailing spaces */
    std::string::iterator wPos= pString.end()-1;
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
    _addConditionalDelimiter(wFontPath);
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
        if (ZGLResource::_testExist(pFontPath))
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



ZGLUnicodeFont* ZGLResource::getFont(const long pFontIdx)
{
    if (!FontList.exists(pFontIdx))
            return nullptr;
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


/* Root Paths for candy profiles, shaders and textures */
std::string ZGLResource::getCandyProfilePath (const char*pFileName)
{
    if (CandyProfileRootPath.empty())
    {
        const char* wProfileRootEnv=getenv(__CANDYPROFILEROOTLOCATION__);
        if (wProfileRootEnv==nullptr)
                {
                CandyProfileRootPath=CandyProfileRootPath_Default;
                }
            else
                {
                CandyProfileRootPath=wProfileRootEnv;
                }
        fprintf (stdout,"GLResource-I-SHADERPATH Candy profile root path is set to <%s> \n",CandyProfileRootPath.c_str());
        if (!ZGLResource::_testExist(CandyProfileRootPath))
            {
            fprintf (stderr,"GLResource-E-PATHNOTEXIST Candy profile path <%s> does NOT exist.\n",CandyProfileRootPath.c_str());
            }
    }
    std::string wFullPath=CandyProfileRootPath ;
    _addConditionalDelimiter(wFullPath);
    wFullPath += pFileName;
    return wFullPath;
}//getCandyProfilePath


std::string ZGLResource::getShaderPath (const char*pFileName)
{
    if (ShaderRootPath.empty())
    {
        const char* wShaderRootEnv=getenv(__SHADERROOTLOCATION__);
        if (wShaderRootEnv==nullptr)
                {
                ShaderRootPath=ShaderRootPath_Default;
                }
            else
                {
                ShaderRootPath=wShaderRootEnv;
                }
        fprintf (stdout,"GLResource-I-SHADERPATH Shaders root path is set to <%s> \n",ShaderRootPath.c_str());
        if (!ZGLResource::_testExist(ShaderRootPath))
            {
            fprintf (stderr,"GLResource-E-PATHNOTEXIST Shader path <%s> does NOT exist.\n",ShaderRootPath.c_str());
            }
    }
    std::string wFullPath=ShaderRootPath ;
    _addConditionalDelimiter(wFullPath);
    wFullPath += pFileName;
    return wFullPath;
}//getShaderPath




std::string ZGLResource::getTexturePath (const char*pFileName)
{
    if (TextureRootPath.empty())
    {
        const char* wTextureRootEnv=getenv(__TEXTUREROOTLOCATION__);
        if (wTextureRootEnv==nullptr)
                {
                TextureRootPath=TextureRootPath_Default;
                }
        else
            {
            TextureRootPath=wTextureRootEnv;
            }
        fprintf (stdout,"GLResource-I-TEXTUREPATH Textures root path is set to <%s> \n",TextureRootPath.c_str());
        if (!ZGLResource::_testExist(TextureRootPath))
            {
            fprintf (stderr,"GLResource-E-PATHNOTEXIST Texture path <%s> does NOT exist.\n",TextureRootPath.c_str());
            }
    }//if (TextureRootPath.empty())
    std::string wFullPath=TextureRootPath ;
    _addConditionalDelimiter(wFullPath);

    wFullPath += pFileName;
    return wFullPath;
}

long ZGLResource::loadTexture(const char*pTextureName,const char* pIntlName,GLenum pEngine)
{
    _TextureBase* wT=loadTexture2D(pTextureName,pEngine);
    if (!wT)
            return -1;
    wT->Name=pIntlName;

    return Textures.push(wT);
}

ZTexture* ZGLResource::getTextureByName(const char* pIntlName)
{
    for (long wi=0;wi<Textures.count();wi++)
            if (strcmp(Textures[wi]->Name,pIntlName)==0)
                        return Textures[wi]->sharePtr();

    return nullptr;
}
ZTexture* ZGLResource::getTextureByRank(const long pIdx)
{
    if (!Textures.exists(pIdx))
                    return nullptr;
    return Textures[pIdx]->sharePtr();
}


/* -----------Font local routines-------------------------- */

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
            "--System fonts as <%s>\n",wFontSysDir.getPath().toCString_Strait());
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
            "--Local fonts as <%s>\n",wFontPath.c_str());
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
    _addConditionalDelimiter(wFontPath);
    wFontPath +=(const char*) __LINUXFONTLOCALDIR__;  /* Remark : no subdirectory  */
    ZDir wFontLocalDir;
    if (wFontLocalDir.setPath((const utf8_t*)wFontPath.c_str())!=ZS_SUCCESS)
        {
        fprintf (stderr,"%s-F-CNTFIND Cannot find directory %s",_GET_FUNCTION_NAME_, wFontPath.c_str());
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

#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>

bool
ZGLResource::_testExist(std::string& pPath)
{
    struct stat wStatBuffer;
    int wSt= stat(  pPath.c_str(),&wStatBuffer);
    return !(wSt<0);
}


