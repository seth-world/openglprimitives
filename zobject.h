#ifndef ZOBJECT_H
#define ZOBJECT_H

#ifdef __USE_GLAD__
#include <glad/glad.h>
#else
#include <GL/glew.h>
#endif

#define __USE_ZARRAY_COPY_CONTRUCTOR__

#include <ztoolset/zarray.h>
#include <zglconstants.h>
#include <zshadercontext.h>


#include <glm/glm.hpp>
#include <zshader.h>

#include <zvertice.h>
#include <zglobjdescriptor.h>

#include <zmatrices.h>

static constexpr glm::vec3 NDirFront = glm::vec3(0.0f,0.0f,1.0f);
static constexpr glm::vec3 NDirBack = glm::vec3(0.0f,0.0f,-1.0f);
static constexpr glm::vec3 NDirTop = glm::vec3(0.0f,1.0f,0.0f);
static constexpr glm::vec3 NDirBottom = glm::vec3(0.0f,-1.0f,0.0f);
static constexpr glm::vec3 NDirLeft = glm::vec3(-1.0f,0.0f,0.0f);
static constexpr glm::vec3 NDirRight = glm::vec3(1.0f,0.0f,0.0f);



struct TextZone
{
    TextZone()=default;
    TextZone(TextZone& pIn) {_cloneFrom(pIn);}

    TextZone& operator=(TextZone& pIn) {_cloneFrom(pIn); return *this;}

    void _cloneFrom(TextZone& pIn)
    {
        Center=pIn.Center;
        Width=pIn.Width;
        Height=pIn.Height;
    }

    glm::vec3   Center;
    glm::vec3   ToTopLeft;
    float       Width;
    float       Height;
};





class ZTexture;
class ZMetaObject;
class ZSphere;
class ZBoxComponents;

class ZObject
{
public:

    friend class ZMetaObject;
    friend class zbs::ZArray<ZObject>;
    friend class ZCandyText;
    friend class ZCandy;

    enum Direction : int
    {
        DirLeft = -1,
        DirRight = 1,
        DirHigh = 2 ,
        DirDown = 3
    } ;

    enum ObjType : uint16_t
    {
        None = 0,
        Openbox ,
        Box,
        Arc,
        ArcStrip,
        Pipe,
        Circle,
        Sphere,
        MetaObject,
        Candy
    };


    enum CircleMade : int8_t
    {
        Face    = 0,
        Flat    = 1,
        Sequent  = 2,
    } ;


protected:

    ZObject()=delete;
    void _cloneFrom(ZObject&pObject);
    void _cloneFrom(ZObject&&pObject);
public:
    ZObject(const char* pName,ObjType pType) ;

    ZObject(const char* pName,ObjType pType,zbs::ZArray<ZVertice>* pVertices) ;

    void _create(const char* pName,ObjType pType) ;


    ZObject(ZObject& pObject)
    {
        _create(pObject.Name,pObject.Type);
        _cloneFrom(pObject);
        GLResources->registerObject(this);

    }// copy CTOR

    ZObject(ZObject&&pObject)
    {
        _create(pObject.Name,pObject.Type);
        _cloneFrom(pObject);
        GLResources->registerObject(this);
    }// copy CTOR

    ~ZObject( );

    void clearArrays();


    ZObject& operator = (ZObject &pInput)
    {
        _create(pInput.Name,pInput.Type);
//        clearObject(); // _cloneFrom takes care of that
        _cloneFrom(pInput);
        return *this;
    }
    ZObject& operator = (ZObject &&pInput)
    {
//        clearObject();
        _cloneFrom(pInput);
        return *this;
    }

    void createVertexOnly(const DrawContext_type pCtx=Draw)
    {
        if (GLDesc[pCtx])
            {
            if (GLDesc[pCtx]->VertexData)
                {
                fprintf (stderr,
                         "ZObject::createVertexOnly-E-VRTEXIST Object <%s> vertex data already exists while trying to create vertex only.\n",
                         Name);
                exit (EXIT_FAILURE);
                }
            delete GLDesc[pCtx];
            }
        GLDesc[pCtx]=new ZGLObjDescriptor();
        GLDesc[pCtx]->VertexData=new zbs::ZArray<ZVertice> ;
    }

    void createVertexAndIndex(const DrawContext_type pCtx=Draw)
    {
        if (GLDesc[pCtx])
            delete GLDesc[pCtx];
        GLDesc[pCtx]=new ZGLObjDescriptor();
        GLDesc[pCtx]->VertexData=new zbs::ZArray<ZVertice> ;
        GLDesc[pCtx]->Indexes=new zbs::ZArray<unsigned int> ;
    }
    void createIndexOnly(const DrawContext_type pCtx=Draw)
    {
        if (GLDesc[pCtx])
            delete GLDesc[pCtx];
        GLDesc[pCtx]=new ZGLObjDescriptor();
        GLDesc[pCtx]->VertexData=nullptr ;
        GLDesc[pCtx]->Indexes=new zbs::ZArray<unsigned int> ;
    }

    ZObject& operator += (ZObject &pInput)
    {
        addObject(pInput);
        return *this;
    }



    /**
     * @brief addObject aggregates pIn object to current object
     */
    ZObject& addObject(ZObject &pIn);

    void addVerticeArray(const DrawContext_type pCtx,zbs::ZArray<glm::vec3>*pIn);

    void computeTexCoords(zbs::ZArray<ZVertice> *vertices);
/*
    void clearObject()
    {

        if (vertices)
            delete vertices;
        vertices=nullptr;
        if (Indices)
            delete Indices;
        Indices=nullptr;
        if (ShapeIndices)
            delete ShapeIndices;
        ShapeIndices=nullptr;
        if (ZANormVisu)
            delete ZANormVisu;
        ZANormVisu=nullptr;
        if (VNormalDir)
            delete VNormalDir;
        VNormalDir=nullptr;
        if (VName)
            delete VName;
        VName=nullptr;
        MatFlag = MM4_Nothing;
//        isMinMaxInit=false;
    }//clearObject
*/
    long verticeCount(const DrawContext_type pCtx=Draw)
    {
        if (!GLDesc[pCtx]->VertexData)
                return -1;
        return GLDesc[pCtx]->VertexData->count();
    }

    ZVertice operator [](zbs::ZArrayIndex_type pIdx) {return GLDesc[Draw]->VertexData->Tab[pIdx];}

    bool hasIndices(const DrawContext_type pCtx=Draw)
    {
        if (GLDesc[pCtx]->Indexes)
                return GLDesc[pCtx]->Indexes->count() > 0;
        return (GLDesc[pCtx]->Indexes!=nullptr);
    }

    long lastVertexIdx(const DrawContext_type pCtx=Draw)
        {
        if (!GLDesc[pCtx]->VertexData)
            {
            if (!GLDesc[Draw]->VertexData)
                                    return -1;
            return GLDesc[Draw]->VertexData->lastIdx();
            }
        return GLDesc[pCtx]->VertexData->lastIdx();
        }

    void setBoxComponents(ZBoxComponents& pBoxComponents);
    TextZone getTextZone();

    ZObject & operator << (ZVertice pInput) {addVertex(Draw,pInput); return *this;}

    ZObject & operator << (Vertice_type pInput) { addVertex(Draw,pInput);  return *this; }

    ZObject & operator << (ZObject pInput) {return addObject(pInput); }

    long addVertex(const DrawContext_type pCtx,ZVertice pInput)
        {
        if (!GLDesc[pCtx])
                    return -1;
        if (!GLDesc[pCtx]->VertexData)
                GLDesc[pCtx]->VertexData=new zbs::ZArray<ZVertice>;
        return GLDesc[pCtx]->VertexData->push(pInput);
        }

    long addVec3(const DrawContext_type pCtx,Vertice_type pInput);

    void addVec3(const DrawContext_type pCtx,Vertice_type pInput,const char*pName);

    long addIndice(const DrawContext_type pCtx,unsigned int pIndice)
        {
        if (!GLDesc[pCtx])
                    return -1;
        if (!GLDesc[pCtx]->Indexes)
                GLDesc[pCtx]->Indexes=new zbs::ZArray<unsigned int>;
        return GLDesc[pCtx]->Indexes->push(pIndice);
        }

    long addNormalDir(const DrawContext_type pCtx,NormalDirection pNormDir )
    {
    if (!GLDesc[pCtx])
                return -1;
    if (!GLDesc[pCtx]->VNormalDir)
            GLDesc[pCtx]->VNormalDir=new zbs::ZArray<NormalDirection>;
    return GLDesc[pCtx]->VNormalDir->push(pNormDir);
    }

    static zbs::ZArray<glm::vec3> *toFlatVertices(zbs::ZArray<ZVertice> *pVertices, zbs::ZArray<unsigned int> *pIndices=nullptr);
    static zbs::ZArray<glm::vec3> *toFlatNormals(zbs::ZArray<ZVertice> *pVertices, zbs::ZArray<unsigned int> *pIndices=nullptr);
    static zbs::ZArray<glm::vec3> *toRawVertices(zbs::ZArray<ZVertice> *pVertices);
    static zbs::ZArray<glm::vec3> *toRawNormals(zbs::ZArray<ZVertice> *pVertices);

    zbs::ZArray<TexCoords_type>* toFlatTexCoords(zbs::ZArray<ZVertice> *pVertices, zbs::ZArray<unsigned int> *pIndices=nullptr);

//    zbs::ZArray<Color_type> toFlatColors();

    zbs::ZArray<glm::vec3>* generateNormals(zbs::ZArray<ZVertice>* pVertices,zbs::ZArray<NormalDirection>*VNormalDir);

    /* calculates normals from pVertices using VNormalDir, and refresh normal coords within pVertices
     *  if VNormalDir does not exist (nullptr) then normal coords is computed using Triangles
     *  if VNormalDir exists then normal is given according face orientation given by VNormalDir
    */
    void computeNormals(zbs::ZArray<ZVertice> *pVertices, zbs::ZArray<NormalDirection> *VNormalDir);

//    void createGL_ObjectArray(unsigned int pShaderVerticePositions,
//                              int pShaderNormalPosition);

//    ZTexture* loadTexture(const char* pTextureName, const char* pIntlName, GLenum pTextureEngine); /* creates a new texture */
//    void setTexture(ZTexture* pTexture); /* re-use an existing texture */
    int setTextureByName(const char* pIntlName, const int pContext=Draw)
    {
        if (ShaderContext[pContext]==nullptr)
                {
                fprintf(stderr,"ZObject::setTextureByName-E-CTXNULL Shader context for object <%s> has not be initialized while trying to set texture <%s>.\n",
                        Name,pIntlName);
                return -1;
                }
        return ShaderContext[pContext]->setTextureByName(pIntlName);
    }
    int setTextureByRank(const long pIdx, const int pContext=Draw)
    {
        if (ShaderContext[pContext]==nullptr)
            {
            fprintf(stderr,"ZObject::setTextureByRank-E-CTXNULL Shader context for object <%s> has not be initialized while trying to set texture rank <%d>.\n",
                    Name,pIdx);
            return -1;
            }
    return ShaderContext[pContext]->setTextureByRank(pIdx);
    }

    const ZTexture* getTexture(const int pContext=Draw)
    {
        if (!ShaderContext[pContext])
                return nullptr;
        return ShaderContext[pContext]->getTexture();
    } /* share already set texture to other objects */


 //   void setAction (const DrawContext_type pCtx,uint16_t pAction) {GLDesc[pCtx]->Actions=pAction;}

    void _setupGL_ObjectArray(const DrawContext_type pCtx, CSO_type pAction);
    void _setupGL_ObjectArray_old(const DrawContext_type pCtx);
    void _setupGL_ObjectElement(const DrawContext_type pCtx,CSO_type pAction);


    /* prepares and sets up vertices data and set GL buffers and vertex arrays */
    void setupGL(const DrawContext_type pCtx, CSO_type pAction);

    void setupGLDraw();

    void setupGLShape(zbs::ZArray<ZVertice> *pVertex);
    void setupGLNormVisu(zbs::ZArray<ZVertice> *pVertex);
    void setupGLUserDefined(zbs::ZArray<ZVertice> *pVertex, CSO_type pAction);

    void drawGLByContext(const DrawContext_type pCtx);

    void drawGLDraw();
//    void drawGL() {drawGL(DrawFigure);}

    void drawGLNormVisu();
    void drawGLShape();

    void drawGLGeneric(const DrawContext_type pCtx);

    void print(int pLimit=-1, FILE* pOutput=stdout) const;
    void printContext(int pCtx, int pLimit,FILE* pOutput=stdout) const;

    void _printContext(int pCtx, int pLimit,FILE* pOutput) const;

    /* DrawFigure is dependent from ZShaderContext */
    //void setDrawFigure(unsigned int pMode) {DrawFigure=pMode;}

    void setDrawFigure(const DrawContext_type pCtx,GLenum pMode){DrawFigure[pCtx]=pMode;}
    void setDrawFigureAll(GLenum pMode)
    {
        for (long wi=0;wi < MaxShaderContext;wi++)
            DrawFigure[wi]=pMode;
    }


    void generateNormVisuFromCtx( const float pNormVisuHeight,const int pCtx=Draw);

    int setShaderByName(const DrawContext_type pCtx,const char* pShader)
         {
            ZShader* wSh=GLResources->getShaderByNamePtr(pShader);
            if (!wSh)
                            return -1;

                if (ShaderContext[pCtx])
                    ShaderContext[pCtx]->setShader(wSh);
                else
                    return -1;
         }
    int setShaderByRank(const DrawContext_type pCtx,const long pShIdx)
         {
            ZShader* wSh=GLResources->getShaderByRankPtr(pShIdx);
            if (!wSh)
                            return -1;

                if (ShaderContext[pCtx])
                    ShaderContext[pCtx]->setShader(wSh);
                else
                    return -1;
            return 0;
         }


    size_t getStride() {return sizeof(ZVertice);}

    void deleteGLContext()
        {
 /*       for (int wi=0;wi<MaxShaderContext;wi++)
            {
                if (GLDesc[wi])
                    {
                    delete GLDesc[wi];
                    }
                if (ShaderContext[wi])
                        delete ShaderContext[wi];
            }*/
        }


    int createNormVisuContextByRank (const long pShaderRank);
    int createNormVisuContextByName (const char* pShaderName);
protected:
    void _createNormVisuContext(ZShader* pShader);


    /** @brief generateNormVisu  generates vertices for drawing normal vectors at the center of object's faces
     *  pVertices must exist and contain the object vertices
     *  result:  created ZArray of <glm::vec3> with corresponding normal vector coords for drawing using GL_LINE draw figure
     *  height of the drawn line is given by pNormVisuHeight
    */
    static zbs::ZArray<glm::vec3>* generateNormVisu(zbs::ZArray<ZVertice> *pVertices,const float pNormVisuHeight);

public:
    /* normal vector height */
    static constexpr float                            NormalVisuHeight=0.04f;

// material is defined within ZShaderContext using uniform setting
 //   void setMaterial(const ZMaterial& pMaterial) {Material=pMaterial;}

    int createShaderContextByName(const int pCtx, const char* pShaderName);
    int createShaderContextByRank(const int pCtx, const  int pShaderRank );
    void createShaderContextFromCopy(const int pCtxDest,const int pCtxSrc);

    void setPosition(Vertice_type pPosition);

    bool hasPosition() {if (!MatCtx) return false; return MatCtx->hasPosition();}
    bool hasRotation() {if (!MatCtx) return false; return MatCtx->hasRotation();}

    void setModelRotation (float pAngle,glm::vec3 pAxis);
    void rotate90 ();
    void rotate270 ();


/*---Matrices---------------*/

/* Declares all matrices as local and assigns a value */
    void setAllMatricesValue (ZMat4* pModel,ZMat4* pView,ZMat4* pProjection, ZMat4* pNormal);

    /**
     * @brief createMatrices creates Matrix Context and populates it with matrices according corresponding flag value :
     * set to true : created  and positionned to identity matrix i. e. glm::mat4(1.0f))$
     * false : nothing is done
     */
    void createMatrices (uint8_t pFlag);

/* matrices : set individual matrices as local to the object : */

    void createAllMatrices();
    void createModel ();
    void createView ();
    void createProjection ();
    void createNormal ();
/* Assign matrices case per case with local value : matrice will stay local unless overriden */

    void setModelValue (glm::mat4 pModel) {MatCtx->setModelValue(pModel);}
    void setViewMatrice (ZMat4* pView);
    void setProjectionMatrice (ZMat4* &&pProjection);
    void setNormalMatrice (ZMat4* &&pNormal);
/* Assign matrices case per case as shared from meta object */
    void shareModel (ZMat4* pModel) {if (MatCtx) MatCtx->shareModel(pModel);}
    void shareView (ZMat4* pView);
    void shareProjection (ZMat4* &&pProjection);
    void shareNormal (ZMat4* &&pNormal);

    void setDrawUserDefined(void (*pDrawFunction)()) {drawGLUserDefined=pDrawFunction;}

    ZGLObjDescriptor* getGLDescriptor(const int pCtx);

    void setMetaObject  (ZMetaObject* pFather) {Father.MTO=pFather; FatherType=ZObject::MetaObject;}
    void setCandy       (ZMetaObject* pFather) {Father.MTO=pFather; FatherType=ZObject::Candy;}
    void setSphere      (ZSphere* pFather) {Father.SPH=pFather;FatherType=ZObject::Sphere;}
    bool isChild() {return Father.Void!=nullptr;}

    ObjType getFatherType () {return FatherType;}

    const char* getFatherName();

public:
    /* before glDraw sets up all matrices and copy it to current shader (must have been set previously) */
    void _preprocessGLMatrices(ZShader *wShader);

/* ---------------Data--------------------------------------*/
public:

    ZGLObjDescriptor*   GLDesc[MaxShaderContext] ;
    ZShaderContext* ShaderContext[MaxShaderContext];
    GLenum      DrawFigure[MaxShaderContext];
    float    NormVisuHeight=0.10f;

    const char*Name=nullptr;

    const size_t verticeOffset  =   0;
    const size_t normalOffset   =   offsetof(ZVertice,normal);
    const size_t textureOffset  =   offsetof(ZVertice,texcoords);

    ObjType                          Type=None;

private:
    ZMatCtx*    MatCtx=nullptr;

    void (*drawGLUserDefined)() =nullptr; /* rendering user function for drawGL */

    union {
        void*        Void;
        ZMetaObject* MTO;
        ZSphere*     SPH;
        ZObject*     OBJ;
        }    Father;

    ZObject::ObjType    FatherType=ZObject::None;

    ZBoxComponents*  BoxComponents=nullptr;


};


void _preprocessGLMatrices(ZShader* wShader,
                          ZMatCtx*     wMat);

const char* decodeObjectType(ZObject::ObjType pType);

#endif // ZOBJECT_H
