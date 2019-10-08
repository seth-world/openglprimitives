#ifndef ZTEXTURE_H
#define ZTEXTURE_H
#include <glad/glad.h>
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
    GLuint getId() {return Id;}

    void deleteGLContext()
    {

        if (Id > 0)
            glDeleteTextures(1,&Id);
        Id=0;
    }

GLuint Id=0;
GLenum TextureEngine;
};

#endif // ZTEXTURE_H
