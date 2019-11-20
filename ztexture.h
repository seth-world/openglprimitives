#ifndef ZTEXTURE_H
#define ZTEXTURE_H

#ifdef __USE_GLAD__
#include <glad/glad.h>
#else
#include <GL/glew.h>
#endif

#include <zglresource.h>

class ZTexture
{
private:
    void _copyFrom(ZTexture& pIn)
    {
        deleteGLContext();
        pIn.Shared++;    // origin Texture is shared with the current
        Shared=false;       // but current is not shared
        Id=pIn.Id;
        TextureEngine=pIn.TextureEngine;
    }

public:
    ZTexture( GLenum pTextureEngine=GL_TEXTURE0);
    ZTexture(const char* pPath , GLenum pTextureEngine=GL_TEXTURE0); /* after openGL context is created */

    ZTexture(ZTexture& pIn) {_copyFrom(pIn);}

    ZTexture(ZTexture* pIn) {_copyFrom(*pIn);}

    ~ZTexture() /* before OpenGL context is deleted */
    {
        GLResources->deregisterTexture(this);
        deleteGLContext();
    }

    ZTexture& operator = (ZTexture &pIn) {_copyFrom(pIn); return *this;}

    int load2D(char const * path);
    void bind();

    /** Keeps registrated the texture but frees whatever exists within texture
     * and reallocates a new empty texture according already defined TextureEngine */
    void reset()
    {
        deleteGLContext();
        glActiveTexture(TextureEngine);
        glGenTextures(1,&Id);
    }

    static void unbind(){ glBindTexture(GL_TEXTURE_2D, 0);}

    GLuint getId() {return Id;}



    void deleteGLContext()
    {

        if ((Id > 0)&&(Shared==0)) // internal data is not released if marked shared : only the father may
                    glDeleteTextures(1,&Id);
        Id=0;
    }

    GLenum getTextureEngine() {return TextureEngine;}

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

    int getTextureEngineNumber()
    {
        return getTextureEngineNumber(TextureEngine);
    }

    GLuint Id=0;
private:
    GLenum TextureEngine;
    int   Shared=0;
};

#endif // ZTEXTURE_H
