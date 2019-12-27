#ifndef ZRESOURCE_H
#define ZRESOURCE_H

#ifdef __USE_GLAD__
#include <glad/glad.h>
#else
#include <GL/glew.h>
#endif
#include <iostream> /* for std::string */

#include <ztoolset/zarray.h>
#include <glm/glm.hpp>
#include <GLFW/glfw3.h>

/* FreeType  */
#include <ft2build.h>
#include FT_FREETYPE_H

const char* getFTErrorString(const FT_Error error_code);

#define __SHADERROOTLOCATION__ "shaderroot"
#define __TEXTUREROOTLOCATION__ "textureroot"

#define __WINFONTSYSTEMROOTENV__ "WINDIR"
#define __WINFONTLOCALROOTENV__ "USERPROFILE"
#define __WINFONTSYSTEMDIR__  "\\Fonts\\"
#define __WINFONTLOCALDIR__   "\\AppData\\Local\\Microsoft\\Windows\\Fonts\\"

#define __LINUXFONTLOCALROOTENV__ "HOME"
#define __LINUXFONTSYSTEMDIR__  "/usr/share/fonts/truetype/"
#define __LINUXFONTLOCALDIR__  ".fonts"

#define __FONTFILESUFFIX__ ".ttf"


enum FontLoc_type : uint8_t
{
    FLOC_Default, /* default : First search custom location then local to user then system */
    FLOC_Sytem, /* forces search to system location and disregard other locations */
    FLOC_User,  /* forces search to user local location and disregard other locations */
    FLOC_Adhoc, /* forces search to custom location and disregard other locations */

};



#ifdef  __USE_WINDOWS__
    /* "%windir%\Fonts\" */
    static constexpr const char * FontRootPath ="%windir%\Fonts\";
    /*"%userprofile%\AppData\Local\Microsoft\Windows\Fonts\"*/
    static constexpr const char * FontLocalPath ="%userprofile%\AppData\Local\Microsoft\Windows\Fonts\";





#else
    static constexpr const char * FontLocalPath ="/home/gerard/.fonts/";
    static constexpr const char * FontRootPath ="/usr/share/fonts/truetype/";
#endif





class ZCamera;

class ZGLUnicodeFont;


class ZTexture;
class ZObject;
class ZShader;
class _ShaderBase;
class _TextureBase;

class ZFont;
/**
 * @brief The ZGLResource class manages OpenGL resources for the application :
 *
 *  + gets information on GL context :
 *      - window size
 *
 *  + manages files paths :
 *      - shaders
 *      - textures
 *      - font files
 *
 *  FONTS
 *
 *  + intializes freetype library
 *
 *  + manages freetype error using getFTErrorString() routine
 *
 *  + loads and adds font to managed font list
 *  + make them available using newFont() giving UnicodeFont object
 *
 *
 * private use :
 *
 *  + register and deregister (frees associated GL resources whenever appropriate)
 *      - GL objects
 *      - shaders
 *      - textures
 *
 *  . cleans up GL resources when application terminates
 *
 */

class ZGLResource
{
public:
    ZGLResource() {initFreeType();}
    ~ZGLResource() ;

#ifdef __USE_WINDOWS__
    static constexpr  const char * ShaderRootPath_Default ="\\shaders";
    static constexpr const char * TextureRootPath_Default ="\\textures";
#else
    static constexpr  const char * ShaderRootPath_Default ="/home/gerard/Development/TestOpenGl/shaders";
    static constexpr const char * TextureRootPath_Default ="/home/gerard/Development/TestOpenGl/textures";
#endif
    std::string TextureRootPath;
    std::string ShaderRootPath;

//    static constexpr const char * FontCustomPath ="/home/gerard/Development/TestOpenGl/fonts/";

    static bool _testExist(std::string& pPath);

    static constexpr const char * FontCustomPath =nullptr;


    int searchForFont(std::string& pFontPath, const char* pFontName, FontLoc_type pLocFlag);

    int searchForLocalFont(std::string& pFontPath,const char* pFontName);
    int searchForSystemFont(std::string& pFontPath,const char* pFontName);


    static std::string _winGetSystemRootFontPath();
    static std::string _winGetLocalRootFontPath();

    static std::string _winGetLocalFontDir();
    static std::string _winGetSystemFontDir();

    static std::string _winGetSystemFontPath(const char* pFontName);
    static std::string _winGetLocalFontPath(const char* pFontName);

    static std::string _linuxGetSystemRootFontDir();
    static std::string _linuxGetLocalFontDir();
    static std::string _linuxGetLocalFontPath(const char* pFontName);
    static std::string _getLinuxSystemFontPath(const char* pGenericName, const char*pFileName);
    static std::string _buildLinuxAdhocFontPath(const char*pGenericName,const char*pFullName);

    static void _linuxListFonts(FILE* pOutput=stdout);
    static void _linuxListSystemFonts(FILE* pOutput=stdout);
    static void _linuxListLocalFonts(FILE* pOutput=stdout);
    static void _linuxSearchFonts(const utf8_t* pSearch,FILE* pOutput=stdout);

    static void _winListFonts(FILE* pOutput=stdout);
    static void _winListSystemFonts(FILE* pOutput=stdout);
    static void _winListLocalFonts(FILE* pOutput=stdout);
    static void _winSearchFonts(const utf8_t* pSearch,FILE* pOutput=stdout);


    static void listSystemFonts(FILE* pOutput=stdout)
    {
#ifdef __USE_WINDOWS__
         _winListSystemFonts(pOutput);
#else
        _linuxListSystemFonts(pOutput);
#endif
    }
    static void listLocalFonts(FILE* pOutput=stdout)
    {
#ifdef __USE_WINDOWS__
        _winListLocalFonts(pOutput);
#else
        _linuxListLocalFonts(pOutput);
#endif
    }
    static void listFonts(FILE* pOutput=stdout)
    {
#ifdef __USE_WINDOWS__
        _winListFonts(pOutput);
#else
        _linuxListFonts(pOutput);
    }
    static void searchFonts(const utf8_t* pSearch,FILE* pOutput=stdout)
#endif
    {
#ifdef __USE_WINDOWS__
        _winSearchFonts(pSearch,pOutput);
#else
        _linuxSearchFonts(pSearch,pOutput);
#endif
    }
/* Textures */
    std::string getTexturePath (const char*pFileName);

    /* loads a texture within resource base and returns its rank (not to be confused with gl texture id) returns -1 if not successfull */
    long loadTexture(const char*pTextureName, const char *pIntlName, GLenum pEngine);
    /* gets a shared pointer to ZTexture corresponding to its given internal name pIntlName */
    ZTexture* getTextureByName(const char* pIntlName);
    /* gets a shared pointer to ZTexture corresponding to its given rank within resource base */
    ZTexture* getTextureByRank(const long pIdx);

/* Shaders */
    std::string getShaderPath (const char*pFileName);

    long loadShader(const char*pVPath, const char*pFPath, const char*pGPath, const char *pIntlName);
    long loadShader(const char*pVPath, const char*pFPath, const char *pIntlName);

    long registerShaderBase(_ShaderBase* pShader) ;
    void deregisterShader(ZShader &pShader);
    void deregisterShaderBase(_ShaderBase* pShader);

    /* gets a shared pointer to ZShader corresponding to its given internal name pIntlName */
    ZShader getShaderByName(const char* pIntlName);
    /* idem by case regardless */
    ZShader getShaderByNameCase(const char* pIntlName);
    /* gets a shared pointer to ZShader corresponding to its given rank within resource base */
    ZShader getShaderByRank(const long pIdx);
    ZShader getActiveShader();

   /* gets pointers to shaders */
    /* gets a shared pointer to ZShader corresponding to its given internal name pIntlName */
    ZShader* getShaderByNamePtr(const char* pIntlName);
    /* idem by case regardless */
    ZShader* getShaderByNameCasePtr(const char* pIntlName);
    /* gets a shared pointer to ZShader corresponding to its given rank within resource base */
    ZShader* getShaderByRankPtr(const long pIdx);
    ZShader* getActiveShaderPtr();

    void listRegistratedShaders(FILE*pOutput=stdout);

/* Fonts */
    static std::string getFontPath (const char* pCategory,const char*pFonName,const char*pName)
    {
        std::string wFullPath=FontRootPath ;
        wFullPath += pName;
        return wFullPath;
    }
    std::string getShaderPath (const std::string pName)
    {
        std::string wFullPath=ShaderRootPath ;
        wFullPath += pName;
        return wFullPath;
    }
    std::string getTexturePath (const std::string pName)
    {
        std::string wFullPath=TextureRootPath ;
        wFullPath += pName;
        return wFullPath;
    }
    static std::string getLocalFontPath (const std::string pName)
    {
        std::string wFullPath=FontRootPath ;
        wFullPath += pName;
        wFullPath += ".ttf";
        return wFullPath;
    }
    static std::string getCustomFontPath (const std::string pName)
    {
        std::string wFullPath=FontRootPath ;
        wFullPath += pName;
        return wFullPath;
    }
    static std::string getSystemFontPath (const std::string pName)
    {
        std::string wFullPath=FontRootPath ;
        wFullPath += pName;
        return wFullPath;
    }



    inline long registerObject(ZObject* pObject) {return Objects.push(pObject);}
    void deregisterObject(ZObject* pObject);

    long registerTextureBase(_TextureBase* pTexture) ;
//    long registerTextTextureBase(_TextureBase* pTexture) ;
    void deregisterTexture(ZTexture* pObject);
    void deregisterTextureBase(_TextureBase* pTexture);

    void listTextures(FILE* pOutput=stdout);
    void listShaders(FILE* pOutput=stdout);
    void listObjects(FILE* pOutput=stdout);

    void cleanAll();

/* Fonts and FreeType */

    void initFreeType();
    void closeFreeType();
    FT_Library getFreeTypeLibrary() {return FreetypeLib; }

    long addFont(const char *pFontName, const char*pIntName, const FontLoc_type pLocFlag=FLOC_Default);
    ZGLUnicodeFont* getFontByName(const char* pName);
    ZGLUnicodeFont* getFont(const long pFontIdx);


    void registerGLWindow(GLFWwindow* pWindow) {GLWindow=pWindow;}

    void registerZCamera (ZCamera* pCamera) ;

    glm::vec2 getGLWindowSize() {
            int wWidth;
            int wHeight;
            glfwGetWindowSize(GLWindow,&wWidth,&wHeight);
            return glm::vec2((float) wWidth,(float)wHeight);
    }
    /**
     * @brief getWindowRatio obtains the ratio from window dimensions necessary to setup Projection matrix
     * @return a float with the ratio
     */
    float getWindowRatio ()
    {
        int wWidth;
        int wHeight;
        glfwGetWindowSize(GLWindow,&wWidth,&wHeight);
        return ((float)wWidth / (float)wHeight);
    }

    void setCuttingCharList(const utf32_t*pCuttingCharList) {CuttingCharList=pCuttingCharList;}

    utf32_t getTruncateCharacter() {return TruncChar;}
    void setTruncateCharacter(utf32_t pChar) {TruncChar=pChar;}


    zbs::ZArray <_TextureBase*> Textures;
    zbs::ZArray <_ShaderBase*> Shaders;
    zbs::ZArray <ZObject*> Objects;

    GLFWwindow* GLWindow=nullptr;
    ZCamera*    Camera=nullptr;

    const utf32_t* CuttingCharList=(utf32_t*)U" -/+";
    const utf32_t* NewLineCharList=(utf32_t*)U"\n";
    const utf32_t* SpaceList=(utf32_t*)U" ";
private:
    utf32_t     TruncChar = 0xB6 ; /* pilcrow character */
//    utf32_t     TruncChar= 0x2BC1 ;

    FT_Library FreetypeLib=nullptr;

    zbs::ZArray<ZFont*> FontList;
} ;

extern ZGLResource* GLResources;

std::string&    _addConditionalDelimiter(std::string& pString);
bool            _testConditionalDelimiter(std::string& pString);
std::string&    _RTrim(std::string& pString);

#endif // ZRESOURCE_H
