#ifndef ZGLOBJDESCRIPTOR_H
#define ZGLOBJDESCRIPTOR_H

#include <glm/glm.hpp>
#include <glad/glad.h>
#include <zglconstants.h>
#include <zshader.h>

class ZTexture;

class ZGLObjDescriptor
{
public:

    ZGLObjDescriptor() = default;
    ZGLObjDescriptor(ZGLObjDescriptor &&pDescriptor)
    {
       cloneFrom(pDescriptor);
    }

    ZGLObjDescriptor(const ZGLObjDescriptor* pDescriptor)
    {
        cloneFrom (*pDescriptor);
    }


    ~ZGLObjDescriptor()
    {
//        deleteGLContext();
    }

    inline ZGLObjDescriptor& cloneFrom(const ZGLObjDescriptor &pDescriptor)
    {
        VBO=pDescriptor.VBO;
        VAO=pDescriptor.VAO;
        NormVBO=pDescriptor.NormVBO;
        TexVBO=pDescriptor.TexVBO;

        EBO=pDescriptor.EBO;

        Count=pDescriptor.Count;

        Mode=pDescriptor.Mode;
        Texture=pDescriptor.Texture;
        UseTexture=pDescriptor.UseTexture;
        UseDefaultColor=pDescriptor.UseDefaultColor;
        UseDefaultAlpha=pDescriptor.UseDefaultAlpha;

        return *this;
    }

    GLuint getPositionAttribute(ZShader* pShader);

    GLuint getNormalAttribute(ZShader* pShader);

    GLuint getTexCoordsAttribute(ZShader* pShader);

    bool hasTexture()
    {
    return (Texture!=nullptr) && UseTexture;
    }

    void setUseTexture(bool pOnOff) {UseTexture=pOnOff;}
    void useDefaultColor(bool pOnOff) {UseDefaultColor=pOnOff;}
    void useDefaultAlpha(bool pOnOff) {UseDefaultAlpha=pOnOff;}

    void deleteGLContext()
    {
        if (VBO)
            glDeleteBuffers(1 ,&VBO);
        VBO=0;
        if (NormVBO)
            glDeleteBuffers(1 ,&NormVBO);
        NormVBO=0;
        if (TexVBO)
            glDeleteBuffers(1 ,&TexVBO);
        TexVBO=0;
        if (EBO)
            glDeleteBuffers(1 ,&EBO);
//        if (EBO_1)
//            glDeleteBuffers(1 ,&EBO_1);
        EBO=0;
        if (VAO)
            glDeleteVertexArrays(1 ,&VAO);
        VAO=0;
    }


   GLuint VBO=0;
   GLuint VAO=0;
   GLuint NormVBO=0;
   GLuint TexVBO=0;
   GLuint EBO=0;
//   GLuint EBO_1=0;

   GLuint       PositionAttribArray=0;
   GLuint       NormalAttribArray=0;
   GLuint       TexCoordsAttribArray=0;

   int          Count=0;

   ZTexture*    Texture=nullptr;

   GLenum       Mode ; /* GL_TRIANGLE_FAN GL_TRIANGLES GL_LINE etc.*/

   bool         UseTexture=false;
   bool         UseDefaultColor=false;
   bool         UseDefaultAlpha=false;
};//ZGLObjDescriptor



#endif // ZGLOBJDESCRIPTOR_H
