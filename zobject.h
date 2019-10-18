#ifndef ZOBJECT_H
#define ZOBJECT_H

#define __USE_ZARRAY_COPY_CONTRUCTOR__

//#include <vector>
#include <ztoolset/zarray.h>
#include <glm/glm.hpp>

#include <glad/glad.h>
#include <zshader.h>

#include <zvertice.h>
#include <zglobjdescriptor.h>

class ZTexture;

class ZObject {  
public:

    enum ObjType : uint16_t
    {
        None = 0,
        Openbox ,
        Box,
        Arc,
        ArcStrip,
        Pipe,
        Sphere,
        Circle
    };

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
        setupVertices       =   0,
        setupNormals        =   1,
        setupTextures       =   2,
        setupAll            =   setupVertices+setupNormals+setupTextures


    };



    static constexpr Vertice_type NDirFront = Vertice_type(0.0f,0.0f,1.0f);
    static constexpr Vertice_type NDirBack = Vertice_type(0.0f,0.0f,-1.0f);
    static constexpr Vertice_type NDirTop = Vertice_type(0.0f,1.0f,0.0f);
    static constexpr Vertice_type NDirBottom = Vertice_type(0.0f,-1.0f,0.0f);
    static constexpr Vertice_type NDirLeft = Vertice_type(-1.0f,0.0f,0.0f);
    static constexpr Vertice_type NDirRight = Vertice_type(1.0f,0.0f,0.0f);


protected:
    friend class zbs::ZArray<ZObject>;
    ZObject() {}
    void _cloneFrom(ZObject&pObject);
    void _cloneFrom(ZObject&&pObject);
public:
    ZObject(const char* pName,ObjType pType) ;

    ZObject(ZObject& pObject)
    {
        _cloneFrom(pObject);
        GLResources->registerObject(this);
    }// copy CTOR

    ZObject(ZObject&&pObject)
    {
        _cloneFrom(pObject);
        GLResources->registerObject(this);
    }// copy CTOR

    ZObject& operator = (ZObject &pInput)
    {
        clearObject();
        _cloneFrom(pInput);
        return *this;
    }
    ZObject& operator = (ZObject &&pInput)
    {
        clearObject();
        _cloneFrom(pInput);
        return *this;
    }


    ZObject& operator += (ZObject &pInput)
    {
        addObject(pInput);
        return *this;
    }


    ~ZObject( )
    {
        GLResources->deregisterObject(this);
        if (GLDescriptor!=nullptr)
                delete GLDescriptor;
        if (GLNormVisuDesc!=nullptr)
                delete GLNormVisuDesc;
        if (GLShapeDesc!=nullptr)
                delete GLShapeDesc;
    }


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
        ShapeIndices.clear();
        ZANormVisu.clear();
//        Normals.clear();
        VNormalDir.clear();
        VName.clear();
//        isMinMaxInit=false;
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


    void addIndice(unsigned int pIndice) {Indices.push_back(pIndice);}

    zbs::ZArray<Vertice_type> toFlatVertices();
    zbs::ZArray<Vertice_type> toFlatNormals();

    zbs::ZArray<Vertice_type> toRawVertices();
    zbs::ZArray<Vertice_type> toRawNormals();

    zbs::ZArray<TexCoords_type> toFlatTexCoords();

//    zbs::ZArray<Color_type> toFlatColors();

    zbs::ZArray<Vertice_type> generateNormals();
    void computeNormals();

//    void createGL_ObjectArray(unsigned int pShaderVerticePositions,
//                              int pShaderNormalPosition);

    ZTexture* setTexture(const char* pTextureName); /* creates a new texture */
    void setTexture(ZTexture* pTexture); /* re-use an existing texture */

    ZTexture* getTexture() {return getGLDescriptor()->Texture;} /* share already set texture to other objects */

    void setUseTexture(bool pOnOff)
        {
        getGLDescriptor()->setUseTexture(pOnOff);
        }


    bool hasTexture() ;
    bool useTexture() {return getGLDescriptor()->UseTexture;}


    void _setupGL_ObjectArray(ZShader* pShader,  uint8_t pShaderSetupOpt );
    void _setupGL_ObjectElement(ZShader *pShader,  uint8_t pShaderSetupOpt );

    void setupGL(ZShader *pShader,
                 uint8_t    pShaderSetupOpt=setupVertices,
                 GLenum DrawFigure=GL_TRIANGLES,
                 ZTexture *pTexture=nullptr);

    void setupGLShape(ZShader* pShader);
    void setupGLNormalVisu(ZShader* pShader);

    void drawGL(ZShader *pShader, unsigned int pDrawFigure);
    void drawGL(ZShader *pShader) {drawGL(pShader,DrawFigure);}

    void drawGLNormalVisu(ZShader* pShader);
    void drawGLShape(ZShader* pShader);

    void print(int pLimit=-1, FILE* pOutput=stdout);

    void setDefaultColor(Color_type pColor) {DefaultColor=pColor;}
    void setDefaultAlpha(float pAlpha) {DefaultAlpha=pAlpha;}
    void setDrawFigure(unsigned int pMode) {DrawFigure=pMode;}

    void setDefaultPosition(Vertice_type pPosition) {DefaultPosition=pPosition;}

    void setUseDefaultColor(bool pOnOff) {getGLDescriptor()->useDefaultColor(pOnOff);}
    void setUseDefaultAlpha(bool pOnOff) {getGLDescriptor()->useDefaultAlpha(pOnOff);}

   void setShader(ZShader* pShader) {Shader=pShader;}

    size_t getStride() {return sizeof(ZVertice);}

    void deleteGLContext()
    {
        if (GLDescriptor==nullptr)
                return;
        GLDescriptor->deleteGLContext();
    }

    void setComputeNormals(bool pOnOff) {ComputeNormals=pOnOff;}
    bool havetoComputeNormals() {return ComputeNormals;}

    void setComputeTexCoords(bool pOnOff) {ComputeTexCoords=pOnOff;}
    bool havetoComputeTexCoords() {return ComputeTexCoords;}

    void generateNormVisu();
    void setMaterial(const ZMaterial& pMaterial) {Material=pMaterial;}

private:
    inline ZGLObjDescriptor* getGLDescriptor()
    {
        if (GLDescriptor!=nullptr)
                return GLDescriptor;
        fprintf(stderr,"ZObject-F-GLDESCNULL Object descriptor has not been initialized for object <%s>.\n"
                "    invoke setupGL() prior using GLDescriptor.\n",Name);
        abort();
//        exit (EXIT_FAILURE);
    }

public:
    ZMaterial                   Material=ZChrome ;

    ZGLObjDescriptor*            GLDescriptor=nullptr;
    ZGLObjDescriptor*            GLNormVisuDesc=nullptr;
    ZGLObjDescriptor*            GLShapeDesc=nullptr;

    ZShader* Shader=nullptr;
    const char*Name=nullptr;


    size_t verticeOffset = 0;
    size_t normalOffset=offsetof(ZVertice,normal);
    size_t textureOffset = offsetof(ZVertice,texcoords);

    zbs::ZArray<ZVertice>           vertices;
    zbs::ZArray<const char*>        VName;
    zbs::ZArray<unsigned int>       Indices;

    zbs::ZArray<NormalDirection>    VNormalDir; /* one rank per triangle : serie of 3 vertices*/

    Color_type DefaultColor=cst_object_color_default;
    float DefaultAlpha=1.0f;
    unsigned int DrawFigure=GL_TRIANGLES;
    /* position within model where object is initially positionned using first translation for model's origin */
    Vertice_type DefaultPosition=Vertice_type(0.0f,0.0f,0.0f);

    bool                            ComputeNormals=true;
    bool                            ComputeTexCoords=true;
    float                           NormalVisuSize=0.04f;
    zbs::ZArray<Vertice_type>       ZANormVisu;
    zbs::ZArray<GLuint>             ShapeIndices;
    ObjType                         Type=None;
};

#endif // ZOBJECT_H
