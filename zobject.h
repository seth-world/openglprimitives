#ifndef ZOBJECT_H
#define ZOBJECT_H

#define __USE_ZARRAY_COPY_CONTRUCTOR__

//#include <vector>
#include <ztoolset/zarray.h>
#include <glm/glm.hpp>

#include <glad/glad.h>
#include <zshader.h>

#include <zvertice.h>


class ZTexture;

class ZGLObjDescriptor
{
public:

    ZGLObjDescriptor() {}
    ZGLObjDescriptor(ZGLObjDescriptor &&pDescriptor)
    {
       copy(pDescriptor);
    }

    ZGLObjDescriptor(ZGLObjDescriptor* pDescriptor)
    {
        copy (*pDescriptor);
    }


    ~ZGLObjDescriptor()
    {
        deleteGLContext();
    }

    inline ZGLObjDescriptor& copy(const ZGLObjDescriptor &pDescriptor)
    {
        VBO=pDescriptor.VBO;
        VAO=pDescriptor.VAO;
        NormVBO=pDescriptor.NormVBO;
        TexVBO=pDescriptor.TexVBO;
        EBO=pDescriptor.NormVBO;
        Mode=pDescriptor.Mode;
        HasElements=pDescriptor.HasElements;
        count=pDescriptor.count;
        size=pDescriptor.size;
        Texture=pDescriptor.Texture;
        UseTexture=pDescriptor.UseTexture;
        UseDefaultColor=pDescriptor.UseDefaultColor;
        UseDefaultAlpha=pDescriptor.UseDefaultAlpha;

        return *this;
    }

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

   GLuint       PositionAttribArray=0;
   GLuint       NormalAttribArray=0;
   GLuint       TexCoordsAttribArray=0;

   ZTexture*    Texture=nullptr;

   int          count;
   int          size;


   unsigned int Mode ; /* GL_TRIANGLE_FAN GL_TRIANGLES */
   bool         HasElements=false; /* defines wether to use glDrawElements in place of glDrawArrays
                                        if set to true, then EBO must point to a GL_ELEMENT_ARRAY_BUFFER
                                        containing valid index values
*/
   bool         UseTexture=false;
   bool         UseDefaultColor=false;
   bool         UseDefaultAlpha=false;
};//ZGLObjDescriptor



class ZObject {
public:

    enum Direction : int
    {
        DirLeft = -1,
        DirRight = 1,
        DirHigh = 2 ,
        DirDown = 3
    } ;

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
    enum CircleMade : int8_t
    {
        Face    = 0,
        Flat    = 1,
        Sequent  = 2,
    } ;


    enum ShaderSetupOpt : uint16_t
    {
        SetupVertices       =   0,
        setupNormals        =   1,
        setupTextures       =   2
    };

    GLuint cst_defaultPositionLocation = 0;
    GLuint cst_defaultNormalLocation = 1;
    GLuint cst_defaultTexCoordsLocation = 2;

    static constexpr Vertice_type NDirFront = Vertice_type(0.0f,0.0f,1.0f);
    static constexpr Vertice_type NDirBack = Vertice_type(0.0f,0.0f,-1.0f);
    static constexpr Vertice_type NDirTop = Vertice_type(0.0f,1.0f,0.0f);
    static constexpr Vertice_type NDirBottom = Vertice_type(0.0f,-1.0f,0.0f);
    static constexpr Vertice_type NDirLeft = Vertice_type(-1.0f,0.0f,0.0f);
    static constexpr Vertice_type NDirRight = Vertice_type(1.0f,0.0f,0.0f);

    ZObject(const char* pName) ;

    ZObject(const ZObject&pObject) { copy(pObject); }// copy CTOR

    ZObject(const ZObject&&pObject) {copy(pObject); }// copy CTOR

    void copy(const ZObject&pObject);
    void copy(const ZObject&&pObject);

    ZObject& addObject(ZObject &pObject);

    void addVertice(Vertice_type pInput)
    {
        ZVertice wVertice(pInput);
        vertices.push_back(wVertice);
        return;
    }
    void addVertice(Vertice_type pInput, const char* pName);
    void computeTexCoords();

    void clearObject()
    {
        vertices.clear();
        Indices.clear();
//        Normals.clear();
        VNormalDir.clear();
        VName.clear();
        isMinMaxInit=false;
    }//clearObject

    zbs::ZArrayIndex_type verticeCount() {return vertices.size();}

    Vertice_type operator [](zbs::ZArrayIndex_type pIdx) {return vertices[pIdx].point;}

    bool hasIndices() {return (Indices.size()>0);}

    long lastVertexIdx() {return vertices.size()-1;}

    ZObject & operator << (ZVertice pInput) {vertices.push_back(pInput); return *this;}
    ZObject & operator << (Vertice_type pInput)
    {
        vertices.push_back(ZVertice(pInput));
        return *this;
    }
    ZObject & operator << (ZObject pInput) {return addObject(pInput); }

//    ZObject & operator << (unsigned int pIndice) {Indices.push_back(pIndice); return *this;}

    ZObject& operator = (ZObject &pInput)
    {
        clearObject();
        copy(pInput);
        return *this;
    }
    ZObject& operator = (ZObject &&pInput)
    {
        clearObject();
        copy(pInput);
        return *this;
    }
    void addIndice(unsigned int pIndice) {Indices.push_back(pIndice);}

    zbs::ZArray<Vertice_type> toFlatVertices();
    zbs::ZArray<Vertice_type> toFlatNormals();

    zbs::ZArray<Vertice_type> toRawVertices();
    zbs::ZArray<Vertice_type> toRawNormals();

    zbs::ZArray<TextCoords_type> toFlatTexCoords();

//    zbs::ZArray<Color_type> toFlatColors();

    zbs::ZArray<Vertice_type> generateNormals();
    void computeNormals();

//    void createGL_ObjectArray(unsigned int pShaderVerticePositions,
//                              int pShaderNormalPosition);

    ZTexture* setTexture(const char* pTextureName); /* creates a new texture */
    void setTexture(ZTexture* pTexture); /* re-use an existing texture */

    ZTexture* getTexture() {return GLDescriptor->Texture;} /* share already set texture to other objects */

    void setUseTexture(bool pOnOff) {GLDescriptor->setUseTexture(pOnOff);}

    bool hasTexture() ;
    bool useTexture() {return GLDescriptor->UseTexture;}

    void createGL_ObjectArray(  uint8_t pShaderSetupOpt );

//    void createGL_ObjectElement(unsigned int pShaderVerticePositions,
//                                int pShaderNormalPosition);
    void createGL_ObjectElement(  uint8_t pShaderSetupOpt );

    void setupGL(ZShader *pShader,
                 uint8_t    pShaderSetupOpt=SetupVertices,
                 GLenum DrawFigure=GL_TRIANGLES,
                 ZTexture *pTexture=nullptr);

    void drawGL(unsigned int pDrawFigure);
    void drawGL() {drawGL(DrawFigure);}

    void drawGLLines();

    void print(int pLimit=-1, FILE* pOutput=stdout);

    void setDefaultColor(Color_type pColor) {DefaultColor=pColor;}
    void setDefaultAlpha(float pAlpha) {DefaultAlpha=pAlpha;}
    void setDrawFigure(unsigned int pMode) {DrawFigure=pMode;}

    void setDefaultPosition(Vertice_type pPosition) {DefaultPosition=pPosition;}

    void setUseDefaultColor(bool pOnOff) {GLDescriptor->useDefaultColor(pOnOff);}
    void setUseDefaultAlpha(bool pOnOff) {GLDescriptor->useDefaultAlpha(pOnOff);}

    void setShader(ZShader* pShader) {Shader=pShader;}

    size_t getStride() {return sizeof(ZVertice);}

    void deleteGLContext()
    {
        if (GLDescriptor==nullptr)
                return;
        GLDescriptor->deleteGLContext();
    }

    size_t verticeOffset = 0;
    size_t normalOffset=offsetof(ZVertice,normal);
    size_t textureOffset = offsetof(ZVertice,textcoords);

    zbs::ZArray<ZVertice>           vertices;
    zbs::ZArray<const char*>        VName;
    zbs::ZArray<unsigned int>       Indices;
    zbs::ZArray<NormalDirection>     VNormalDir; /* one rank per triangle : serie of 3 vertices*/

    Color_type DefaultColor=cst_object_color_default;
    float DefaultAlpha=1.0f;
    unsigned int DrawFigure=GL_TRIANGLES;

    ZGLObjDescriptor*            GLDescriptor=nullptr;

    ZShader* Shader=nullptr;
    const char*Name=nullptr;

    /* position within model where object is initially positionned using first translation for model's origin */
    Vertice_type DefaultPosition=Vertice_type(0.0f,0.0f,0.0f);

    bool isMinMaxInit=false;
    float XMin, XMax,YMin,YMax;


    void setComputeNormals(bool pOnOff) {ComputeNormals=pOnOff;}
    bool havetoComputeNormals() {return ComputeNormals;}

    void setComputeTexCoords(bool pOnOff) {ComputeTexCoords=pOnOff;}
    bool havetoComputeTexCoords() {return ComputeTexCoords;}

    bool ComputeNormals=true;
    bool ComputeTexCoords=true;

};

#endif // ZOBJECT_H
