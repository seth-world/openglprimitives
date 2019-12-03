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

    static constexpr const char * FontRootPath ="/home/gerard/Development/TestOpenGl/fonts/";

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
    static std::string getFontPath (const char*pName)
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
    static std::string getFontPath (const std::string pName)
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

    long addFont(const char *pFontPath,const char*pName, const bool pResident=true);

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

    zbs::ZArray <ZTexture*> Textures;
    zbs::ZArray <ZShader*> Shaders;
    zbs::ZArray <ZObject*> Objects;

    GLFWwindow* GLWindow=nullptr;
    ZCamera*    Camera=nullptr;

    const utf32_t* CuttingCharList=(utf32_t*)U" -/+";
    const utf32_t* NewLineCharList=(utf32_t*)U"\n";
    const utf32_t* SpaceList=(utf32_t*)U" ";
private:

    FT_Library FreetypeLib=nullptr;

    zbs::ZArray<ZFont*> FontList;
} ;

extern ZGLResource* GLResources;

#endif // ZRESOURCE_H
