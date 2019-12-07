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


#define __WINFONTSYSTEMROOTENV__ "WINDIR"
#define __WINFONTLOCALROOTENV__ "USERPROFILE"
#define __WINFONTSYSTEMDIR__  "\\Fonts\\"
#define __WINFONTLOCALDIR__   "\\AppData\\Local\\Microsoft\\Windows\\Fonts\\"

#define __LINUXFONTLOCALROOTENV__ "HOME"
#define __LINUXFONTSYSTEMDIR__  "/usr/share/fonts/truetype/"
#define __LINUXFONTLOCALDIR__  "/.fonts"

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



    void _linuxListFonts(FILE* pOutput=stdout);
    void _linuxListSystemFonts(FILE* pOutput=stdout);
    void _linuxListLocalFonts(FILE* pOutput=stdout);
    void _linuxSearchFonts(const utf8_t* pSearch,FILE* pOutput=stdout);


class ZCamera;

class ZGLUnicodeFont;


class ZTexture;
class ZObject;
class ZShader;

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

    static constexpr  const char * ShaderRootPath ="/home/gerard/Development/TestOpenGl/shaders/";
    static constexpr const char * TextureRootPath ="/home/gerard/Development/TestOpenGl/textures/";

//    static constexpr const char * FontCustomPath ="/home/gerard/Development/TestOpenGl/fonts/";

    static constexpr const char * FontCustomPath =nullptr;


    int searchForFont(std::string& pFontPath, const char* pFontName, FontLoc_type pLocFlag);

    int searchForLocalFont(std::string& pFontPath,const char* pFontName);
    int searchForSystemFont(std::string& pFontPath,const char* pFontName);



    static std::string _winGetSystemFontPath(const char* pFontName);
    static std::string _winGetLocalFontPath(const char* pFontName);

    static std::string _getLinuxLocalFontPath(const char* pFontName);
    static std::string _getLinuxSystemFontPath(const char* pGenericName, const char*pFileName);
    static std::string _buildLinuxAdhocFontPath(const char*pGenericName,const char*pFullName);



    static void listSystemFonts(FILE* pOutput=stdout)
    {
        _linuxListSystemFonts(pOutput);
    }
    static void listLocalFonts(FILE* pOutput=stdout)
    {
        _linuxListLocalFonts(pOutput);
    }
    static void listFonts(FILE* pOutput=stdout)
    {
        _linuxListFonts(pOutput);
    }
    static void searchFonts(const utf8_t* pSearch,FILE* pOutput=stdout)
    {
        _linuxSearchFonts(pSearch,pOutput);
    }

    static std::string getShaderPath (const char*pName)
    {
        std::string wFullPath=ShaderRootPath ;
        wFullPath += pName;
        return wFullPath;
    }
    static std::string getTexturePath (const char*pName)
    {
        std::string wFullPath=TextureRootPath ;
        wFullPath += pName;
        return wFullPath;
    }

    static std::string getFontPath (const char* pCategory,const char*pFonName,const char*pName)
    {
        std::string wFullPath=FontRootPath ;
        wFullPath += pName;
        return wFullPath;
    }
    static std::string getShaderPath (const std::string pName)
    {
        std::string wFullPath=ShaderRootPath ;
        wFullPath += pName;
        return wFullPath;
    }
    static std::string getTexturePath (const std::string pName)
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
    static ZShader loadShader(const char*pVPath, const char*pFPath, const char*pGPath, const char *pName);


    inline void registerObject(ZObject* pObject) {Objects.push_back(pObject);}
    inline void registerTexture(ZTexture* pTexture) {Textures.push_back(pTexture);}
    inline void registerShader(ZShader* pObject) {Shaders.push_back(pObject);}

    void deregisterShader(ZShader* pObject);
    void deregisterObject(ZObject* pObject);
    void deregisterTexture(ZTexture* pObject);

    void cleanAll();


    void initFreeType();
    void closeFreeType();
    FT_Library getFreeTypeLibrary() {return FreetypeLib; }

    long addFont(const char *pFontName, const char*pIntName, const FontLoc_type pLocFlag=FLOC_Default);
    ZGLUnicodeFont* getFontByName(const char* pName);
    ZGLUnicodeFont* getFont(const long pFontIdx,size_t pFontsize);


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


    zbs::ZArray <ZTexture*> Textures;
    zbs::ZArray <ZShader*> Shaders;
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




#endif // ZRESOURCE_H
