#include "ztexture.h"
#include <stb_image.h>
#include <zglresource.h>

 _TextureBase::_TextureBase(GLenum pTextureEngine)
 {
     glActiveTexture(pTextureEngine);

     TextureEngine=pTextureEngine;
     glGenTextures(1,&GLId);
//     GLResources->registerTextureBase(this);
 }

_TextureBase::~_TextureBase()
    {
        glDeleteTextures(1,&GLId);
    }




ZTexture _TextureBase::share()
{
    Shared++;
    return ZTexture(this);
}
ZTexture* _TextureBase::sharePtr()
{
    Shared++;
    return new ZTexture(this);
}

/*
ZTexture::ZTexture( GLenum pTextureEngine)
{
    glActiveTexture(pTextureEngine);
    GLResources->registerTexture(this);
    TextureEngine=pTextureEngine;
    glGenTextures(1,&Id);
}
ZTexture::ZTexture(const char* pPath , const char *pIntName, GLenum pTextureEngine)
{
    glActiveTexture(pTextureEngine);
    GLResources->registerTexture(this);
    TextureEngine=pTextureEngine;
    setName(pIntName);
//    glGenTextures(1,&Id);  -- gentexture is made within load2D()
    load2D(pPath);
}
*/
void ZTexture::bind() const
{
//    glEnable(GL_TEXTURE_2D);  // not allowed in opengl 3.3
    glActiveTexture(TextureBase->TextureEngine);
    glBindTexture(GL_TEXTURE_2D, TextureBase->GLId);
}


// utility function for loading a 2D texture from file
// ---------------------------------------------------
_TextureBase* loadTexture2D(char const * path, GLenum pTextureEngine)
{
//    unsigned int textureID;
    std::string wPath=GLResources->getTexturePath(path);
    int width, height, nrComponents;
    unsigned char *wImageData=nullptr;
    wImageData = stbi_load(wPath.c_str(),
                                    &width,
                                    &height,
                                    &nrComponents,
                                    0);
    if (!wImageData)
        {
        std::cerr << "Texture failed to load at path: " << wPath << std::endl;
        stbi_image_free(wImageData);
        return nullptr;
        }
    printf ("texture <%s> has been successfully loaded \n",wPath.c_str());

    _TextureBase* wTB=new _TextureBase(pTextureEngine);
//    GLuint wId=0;
//    glActiveTexture(pTextureEngine);
//    glGenTextures(1, &wId);

    GLenum format=GL_RGB;
    if (nrComponents == 1)
        format = GL_RED;
    else if (nrComponents == 3)
        format = GL_RGB;
    else if (nrComponents == 4)
        format = GL_RGBA;

    glBindTexture(GL_TEXTURE_2D, wTB->GLId);
    glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, wImageData);
    glGenerateMipmap(GL_TEXTURE_2D);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    stbi_image_free(wImageData);


    return wTB;
}//load2D


#ifdef __COMMENT__
// utility function for loading a 2D texture from file
// ---------------------------------------------------
unsigned int loadTexture(char const * path)
{
    unsigned int textureID;

    int width, height, nrComponents;
    unsigned char *data = stbi_load(GLResources->getTexturePath(path).c_str(),
                                    &width,
                                    &height,
                                    &nrComponents,
                                    0);
    if (!data)
        {
        std::cout << "Texture failed to load at path: " << path << std::endl;
        stbi_image_free(data);
        return 0; /* 0 is errored : must be 1 or more */
        }
    printf ("texture %s has been successfully loaded \n",GLResources->getTexturePath(path).c_str());
    glActiveTexture(GL_TEXTURE0);
    glGenTextures(1, &textureID);

    GLenum format=GL_RGB;
    if (nrComponents == 1)
        format = GL_RED;
    else if (nrComponents == 3)
        format = GL_RGB;
    else if (nrComponents == 4)
        format = GL_RGBA;

    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    stbi_image_free(data);

    return textureID;
}//loadTexture
#endif // __COMMENT__
///////////////////////////////////////////////////////////////////////////////
// load raw image as a texture
///////////////////////////////////////////////////////////////////////////////
/*GLuint loadTexture_1(const char* fileName, bool wrap)
{
    Image::Bmp bmp;
    if(!bmp.read(fileName))
        return 0;     // exit if failed load image

    // get bmp info
    int width = bmp.getWidth();
    int height = bmp.getHeight();
    const unsigned char* data = bmp.getDataRGB();
    GLenum type = GL_UNSIGNED_BYTE;    // only allow BMP with 8-bit per channel

    // We assume the image is 8-bit, 24-bit or 32-bit BMP
    GLenum format;
    int bpp = bmp.getBitCount();
    if(bpp == 8)
        format = GL_LUMINANCE;
    else if(bpp == 24)
        format = GL_RGB;
    else if(bpp == 32)
        format = GL_RGBA;
    else
        return 0;               // NOT supported, exit

    // gen texture ID
    GLuint texture;
    glGenTextures(1, &texture);

    // set active texture and configure it
    glBindTexture(GL_TEXTURE_2D, texture);

    // select modulate to mix texture with color for shading
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    //glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE);

    // if wrap is true, the texture wraps over at the edges (repeat)
    //       ... false, the texture ends at the edges (clamp)
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrap ? GL_REPEAT : GL_CLAMP);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrap ? GL_REPEAT : GL_CLAMP);
    //glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    //glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    // copy texture data
    glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, type, data);
    //glGenerateMipmap(GL_TEXTURE_2D);

    // build our texture mipmaps
    switch(bpp)
    {
    case 8:
        gluBuild2DMipmaps(GL_TEXTURE_2D, 1, width, height, GL_LUMINANCE, type, data);
        break;
    case 24:
        gluBuild2DMipmaps(GL_TEXTURE_2D, 3, width, height, GL_RGB, type, data);
        break;
    case 32:
        gluBuild2DMipmaps(GL_TEXTURE_2D, 4, width, height, GL_RGBA, type, data);
        break;
    }

    return texture;
}
*/
