#ifndef ZTEXTURE_H
#define ZTEXTURE_H

#ifdef __USE_GLAD__
#include <glad/glad.h>
#else
#include <GL/glew.h>
#endif

#include <zglresource.h>

class ZTexture;

class _TextureBase
{
public:
    GLuint  GLId=0;
    GLenum TextureEngine=GL_TEXTURE0;
    int   Shared=0;
    const char* Name=nullptr; /* remark : ZTextTexture has no name (nullptr) */

    _TextureBase()=delete;
    _TextureBase(GLenum pTextureEngine);

    ~_TextureBase();
    GLuint getId() {return GLId;}

//    int load2DImage(const char * path, GLenum pTextureEngine);

    ZTexture* sharePtr();
    ZTexture share();
    bool unShare() {Shared--; if (Shared<0)return true; return false;}
};

class ZTexture
{
    friend class ZGLResource;

protected:
    _TextureBase* TextureBase;
    void _copyFrom(ZTexture& pIn)
    {
        if (TextureBase)
                TextureBase->unShare();
        TextureBase=nullptr;
        if (pIn.TextureBase)
                {
                TextureBase=pIn.TextureBase;
                TextureBase->Shared++;
                }
    }

    ZTexture(GLenum pTextureEngine)
    {
    }
//    ZTexture( GLenum pTextureEngine=GL_TEXTURE0);
//    ZTexture(const char* pPath , const char* pIntName,GLenum pTextureEngine=GL_TEXTURE0); /* after openGL context is created */
public:
    ZTexture()=delete;
    ZTexture(_TextureBase* pTB) {TextureBase=pTB;}

    ZTexture(ZTexture& pIn) {_copyFrom(pIn);}

    ZTexture(ZTexture* pIn) {_copyFrom(*pIn);}

    ~ZTexture()
    {
        GLResources->deregisterTexture(this);
    }

    bool isViable()
    {
        if (!TextureBase)
            return false;
        return TextureBase->GLId!=0 ;
    }

    ZTexture& operator = (ZTexture &pIn) {_copyFrom(pIn); return *this;}

//    int load2D(char const * path);
    void bind() const;

    static void unbind(){ glBindTexture(GL_TEXTURE_2D, 0);}

    GLuint getId() {return TextureBase->GLId;}

    ZTexture share() {return TextureBase->share();}
    ZTexture* sharePtr() {return TextureBase->sharePtr();}


    static int getTextureEngineNumber(GLenum pTextureEngine)
    {
        switch (pTextureEngine)
        {
        case GL_TEXTURE0:
            return 0;
        case GL_TEXTURE1:
            return 1;
        case GL_TEXTURE2:
            return 2;
        default:
            return (int)( pTextureEngine-GL_TEXTURE0);
        }
    }
    GLenum getTextureEngine() {return TextureBase->TextureEngine;}
    int getTextureEngineNumber() const
    {
        return getTextureEngineNumber(TextureBase->TextureEngine);
    }

    const char* getName() const
    {
        if (!TextureBase)
            return nullptr;
        return TextureBase->Name;
    }
    int getGLId() const
    {
        if (!TextureBase)
            return -1;
        return TextureBase->GLId;
    }
//    void setName(const char* pName) {strncpy(Name,pName,sizeof(Name));}



};


class ZTextTexture : public ZTexture
{
    friend class ZGLUnicodeText;
public:
    ZTextTexture(GLenum pTextureEngine):ZTexture(pTextureEngine)
    {
        TextureBase=new _TextureBase(pTextureEngine);
        TextureBase->Name ="TextTexture";
        GLResources->registerTextureBase(TextureBase);

    }
    ZTextTexture(ZTextTexture& pIn):ZTexture(pIn.getTextureEngine())
    {
        _copyFrom(pIn);
    }
    ~ZTextTexture() {}

};


_TextureBase* loadTexture2D(char const * path, GLenum pTextureEngine);

#endif // ZTEXTURE_H
