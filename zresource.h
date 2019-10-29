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

class ZTexture;
class ZObject;
class ZShader;

class ZGLResource
{
public:
    ZGLResource() {initFreeType();}
    ~ZGLResource() {closeFreeType();}


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


    void registerGLWindow(GLFWwindow* pWindow) {GLWindow=pWindow;}

    glm::vec2 getGLWindowSize() {
            int wWidth;
            int wHeight;
            glfwGetWindowSize(GLWindow,&wWidth,&wHeight);
            return glm::vec2((float) wWidth,(float)wHeight);
    }

    zbs::ZArray <ZTexture*> Textures;
    zbs::ZArray <ZShader*> Shaders;
    zbs::ZArray <ZObject*> Objects;

    GLFWwindow* GLWindow=nullptr;

private:
//    char WorkArea [255];
    FT_Library FreetypeLib=nullptr;
} ;

extern ZGLResource* GLResources;

#endif // ZRESOURCE_H
