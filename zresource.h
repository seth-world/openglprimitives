#ifndef ZRESOURCE_H
#define ZRESOURCE_H

#include <iostream> /* for std::string */

#include <ztoolset/zarray.h>

class ZTexture;
class ZObject;

class ZGLResource
{
public:
    static constexpr  const char * ShaderRootPath ="/home/gerard/Development/TestOpenGl/shaders/";
    static constexpr const char * TextureRootPath ="/home/gerard/Development/TestOpenGl/textures/";

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

    inline void registerObject(ZObject* pObject) {Objects.push_back(pObject);}
    inline void registerTexture(ZTexture* pTexture) {Textures.push_back(pTexture);}

    void deregisterObject(ZObject* pObject);
    void deregisterTexture(ZTexture* pObject);

    void cleanAll();

    zbs::ZArray <ZTexture*> Textures;
    zbs::ZArray <ZObject*> Objects;


private:
//    char WorkArea [255];
} ;

extern ZGLResource* GLResources;

#endif // ZRESOURCE_H
