#ifndef ZTEXTURE_H
#define ZTEXTURE_H

#ifdef __USE_GLAD__
#include <glad/glad.h>
#else
#include <GL/glew.h>
#endif

#include <zresource.h>

class ZTexture
{
public:
    ZTexture( GLenum pTextureEngine=GL_TEXTURE0);
    ZTexture(const char* pPath , GLenum pTextureEngine=GL_TEXTURE0); /* after openGL context is created */

    ~ZTexture() /* before OpenGL context is deleted */
    {
        GLResources->deregisterTexture(this);
        deleteGLContext();
    }

    int load2D(char const * path);
    void bind();

    static void unbind(){ glBindTexture(GL_TEXTURE_2D, 0);}

    GLuint getId() {return Id;}



    void deleteGLContext()
    {

        if (Id > 0)
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
GLenum TextureEngine;
};

#endif // ZTEXTURE_H
