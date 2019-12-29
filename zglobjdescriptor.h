#ifndef ZGLOBJDESCRIPTOR_H
#define ZGLOBJDESCRIPTOR_H
#ifdef __USE_GLAD__
#include <glad/glad.h>
#else
#include <GL/glew.h>
#endif

#include <glm/glm.hpp>

#include <zglconstants.h>
#include <zshader.h>
#include <camera.h>


enum NormalDirection : int8_t
{
    Back    = -1,
    Compute = 0,
    Front   = 1,
    Left    = -2,
    Right   = 4,
    Top     = 8,
    Bottom  = 0x10
} ;

class ZTexture;
class ZMat4;

class ZGLObjDescriptor
{
public:

    ZGLObjDescriptor()=default;

    ZGLObjDescriptor(ZGLObjDescriptor &&pDescriptor)
    {
       cloneFrom(pDescriptor);
    }

    ZGLObjDescriptor(const ZGLObjDescriptor* pDescriptor)
    {
        cloneFrom (*pDescriptor);
    }

    ZGLObjDescriptor(zbs::ZArray<ZVertice>* pVertices, zbs::ZArray<unsigned int>* pIndexes)
    {
    setupVertex( pVertices, pIndexes);
    }

    ZGLObjDescriptor(zbs::ZArray<glm::vec3>* pVertices)
    {
    setupVec3( pVertices);
    }

    ~ZGLObjDescriptor();

    int setupVertex(zbs::ZArray<ZVertice>* pVertices, zbs::ZArray<unsigned int>* pIndexes=nullptr);
    int setupVec3(zbs::ZArray<glm::vec3>* pVertices, zbs::ZArray<unsigned int>* pIndexes=nullptr);

    ZGLObjDescriptor& cloneFrom(const ZGLObjDescriptor &pIn);


    ZGLObjDescriptor& operator=(const ZGLObjDescriptor &pIn) {cloneFrom(pIn);}
    ZGLObjDescriptor& operator=(const ZGLObjDescriptor &&pIn) {cloneFrom(pIn);}
//    void setTexture(ZTexture* pTexture);
//    ZTexture* loadTexture(const char* pTextureName, const char* pIntlName, GLenum pTextureEngine);
//    int setTextureByName(const char* pIntlName);
//    int setTextureByRank(const long pIdx);


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
/*
   GLuint       PositionAttribArray=0;
   GLuint       NormalAttribArray=0;
   GLuint       TexCoordsAttribArray=0;
*/
   /* Descriptor rules : to be changed with ZArray of rules */

   bool     ComputeNormals=false;
   bool     ComputeTexCoords=false;
   bool     ComputeNormVisu=false;
   bool     KeepVertices=false;
   float    LineSize=1.0f;

   uint16_t Actions = CSO_setupVertices;//  CoordsSetupOpt

   // Texture is managed under ZShaderContext object

#ifdef __COMMENT__
   int          Count=0;


   ZTexture*    Texture=nullptr;

   GLenum       Mode ; /* GL_TRIANGLE_FAN GL_TRIANGLES GL_LINE etc.*/

   bool         UseTexture=false;
   bool         UseDefaultColor=false;
   bool         UseDefaultAlpha=false;
#endif // __COMMENT__

   zbs::ZArray<ZVertice>*                   VertexData=nullptr; /* contains coords for Drawing object , normal coords and texture coords */
   zbs::ZArray<GLuint>*                     Indexes=nullptr;
   zbs::ZArray<const char*>*                VName=nullptr;
   zbs::ZArray<NormalDirection>*            VNormalDir=nullptr; /* one rank per triangle : serie of 3 vertices*/
   bool NormalComputed=false;
};//ZGLObjDescriptor



#endif // ZGLOBJDESCRIPTOR_H
