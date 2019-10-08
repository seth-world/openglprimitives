#ifndef ZGLOBJECT_H
#define ZGLOBJECT_H

#include <cstdio>

#include <ztoolset/zarray.h>

#include <zgltypes.h>
#include <zglconstant.h> /* for ZGLDrawMode */
#include <zcoord.h>


//#include <zvertex.h>

typedef glm::vec3 Vertice_type;
typedef glm::vec3 Color_type;
typedef glm::vec2 TextCoords_type;

class ZCamera;

class ZVertexArray :public zbs::ZArray<Vertice_type>
{
public:
    ZVertexArray():zbs::ZArray<Vertice_type>() {}

    using ZArray<Vertice_type>::clear;


    inline float* getData() {return (float*)getDataPtr();}

    long vertexCount() {return count();}

    inline Vertice_type& operator [] (long pIdx) {return Tab[pIdx];}
    inline ZVertexArray& operator << (const Vertice_type pVertex) {push(pVertex); return *this;}

//    inline size_t getStride() {return Tab[0].getStride();}

    zbs::ZArray<Color_type>     colors;

};

class ZIndexArray : public zbs::ZArray<int>
{
public:
    ZIndexArray():zbs::ZArray<int>() {}

//    using ZArray<int>::count;
    inline int* getData() {return (int*)Tab;}

//    inline long count() {return ZArray<int>::count();}

 //   inline int operator [] (long pIdx) {return (GLuint)Tab[pIdx].Value();}

//    inline void push(const GLuint pIndice) {zbs::ZArray<ZGLIndex>::push(ZGLIndex(pIndice));}
//    inline void push(const ZGLIndex pIndex) {zbs::ZArray<ZGLIndex>::push(pIndex);}

//    inline ZIndexArray& operator << (const GLuint pIndice) {push(pIndice); return *this;}
//    inline ZIndexArray& operator << (const ZGLIndex pIndex) {push(pIndex); return *this;}
};


class ZWorld;
class ZShader;
class UI3DWindow;

class ZObject
{
public:

    ZObject():VertexArray(),IndexArray() {}

    ZObject(const ZObject& pObject):VertexArray(),IndexArray() {addObject(const_cast<ZObject&>(pObject));}

    ZObject(ZWorld* pWorld):VertexArray(),IndexArray() {World=pWorld;}

//    ZObject (ZWorld* pWorld, ZCoord pCenter,float pHigh,float pLength, float pDepth) {World=pWorld;setup(pCenter,pHigh, pLength, pDepth);}

    ~ZObject () {clearGL();}

    ZGLStatus setPosition(const float pX,const float pY,const float pZ);

    ZGLStatus setPosition (ZVect3 pPosition) {return setPosition(pPosition.x,pPosition.y,pPosition.z);}

     void setup (ZCoord pCenter, float pHigh, float pLength, float pDepth);

    inline bool hasIndex() {return (indexCount()!=0);}

    inline float* VertextoFlat () {return VertexArray.getData();}
    inline const int* IndicetoFlat () {return (int*)IndexArray.Data.zptr_void;}

    inline long lastVertexIdx() {return VertexArray.lastIdx();}

    inline long vertexCount()   {return VertexArray.count();}
    inline long indexCount()  {return IndexArray.count();}

    Vertice_type vertex(long pIdx) {return VertexArray.Tab[pIdx];}
    int index(long pIdx) const {return IndexArray.Tab[pIdx];}

    inline ZVect3&  position(long pIdx) {return VertexArray.Tab[pIdx].m_position;}
    inline ZColor&  color(long pIdx) {return VertexArray.Tab[pIdx].m_color;}

    inline void clear() {VertexArray.clear();IndexArray.clear();}
    inline void clearIndex() {IndexArray.clear();}

    inline void addVertex(const Vertice_type pVertex)
    {
        VertexArray.push(pVertex);
        return;
    }
    inline void addIndex(const float pIndice) {IndexArray.push((int)pIndice); return ;}
    inline void addIndex(const int pIndex) {IndexArray.push(pIndex); return ;}


    inline size_t  getIndexSize() {return (size_t)IndexArray.usedSize();}
    inline int*    getIndexPtr() {return &IndexArray[0];}

    inline size_t getVertexSize() {return (size_t)VertexArray.vertices.usedSize();}
    inline float* getVertexPtr()    {return (float*)&VertexArray.vertices[0];}
 //   inline float* getColorPtr()    {return &VertexArray.colors[0];}



    void addObject(ZObject &pObject);

    void initializeGL();
 //   Q_DECL_CONSTEXPR void paintGL(); /* paintGL is at higher level where object's shape is defined and drawn */
    void draw();
    ZGLStatus draw(ZCamera *pCamera);

    void clearGL();

    /** re-uses an existing shader */
    void setShader(ZShader* pShader){Shader=pShader;}

    /** creates a new shader */
    ZGLStatus loadShadersFromFile(const char* pVertexFile,const char* pFragmentFile);
    ZGLStatus loadShadersFromQResource(const char* pVertexResource,const char* pFragmentResource);
/*
    QOpenGLBuffer& getVBO(QOpenGLBuffer& pVbo) ;
    QOpenGLBuffer& getEBO(QOpenGLBuffer& pEbo) ;


    QOpenGLBuffer& createVBO(QOpenGLBuffer::UsagePattern pUsagePatern=QOpenGLBuffer::StaticDraw);
    QOpenGLBuffer& setupEBO(QOpenGLBuffer::UsagePattern pUsagePatern=QOpenGLBuffer::StaticDraw);
*/

    ZGLStatus rotate() {return ZGL_SUCCESS;}
    ZGLStatus translate(ZVect3 pPosition) {return ZGL_SUCCESS;}

    const ZObject& operator << (ZVertex &pVertex) {addVertex(pVertex); return *this;}
    const ZObject& operator << (const int pIndex) {addIndex(pIndex); return *this;}
    const ZObject& operator << (ZObject &pObject) {addObject(pObject); return *this;}


    void structureScale(float pScale) ;


//    friend ZGLObject& operator <<(ZGLObject& pObject,ZVertex& pVertex);
//    friend ZGLObject& operator <<(ZGLObject& pObject,ZIndice& pIndice);

    friend class UI3DWindow;


    void listAll(FILE* pOutput=stdout);
    void codeList(FILE* pOutput=stdout);
    void codePositionList(FILE* pOutput=stdout);

     ZMat4 getTransformation();

public:

    int UniformWorld=-1;
    int UniformView=-1;
    int UniformProjection=-1;

    ZMat4  Model ;
    ZMat4  View ;
    ZMat4  Projection ;


public:
    // Private Helpers
    void printVersionInformation() {}


    void setFrontDrawMode(const GLDrawMode pDM );
    void setBackDrawMode(const GLDrawMode pDM );
    void setupDrawMode();

    uint32_t FrontDrawMode = (uint32_t)GLDM_Fill  ; /* front face is filled by default */
    uint32_t BackDrawMode = (uint32_t)GLDM_Nothing ; /* backward face is not drawn by default */

    void scheduleDraw() {ToBeDrawn=true;}


private:

    unsigned int VboHandle=0;
    unsigned int EboHandle=0;
    unsigned int VaoHandle=0;

    bool IndexSetup=false;
    int Usage=GL_DYNAMIC_DRAW ; /* could be GL_STATIC_DRAW GL_STREAM_DRAW GL_DYNAMIC_DRAW */
    ZShader*        Shader=nullptr;

    bool ToBeDrawn=false;

private:
    ZVect3          Position; /* Object's position within world */

    ZMat4      ObjectToModel;
    ZMat4      MScale;
    ZMat4      MRotation;
    ZMat4      MTranslation;

    ZWorld*         World=nullptr;


    float           RotationAngle=1.0f;
    ZVect3          RotationAxis = {1.0f,1.0f,1.0f};

public:
    ZVertexArray    VertexArray;
    ZIndexArray     IndexArray;

    zbs::ZArray<char*> VertexName;
    zbs::ZArray<char*> IndiceName;

};// ZGLObject


/* methods */

/**
 * @brief ZObject::addObject
 * @param pObject
 */
inline void ZObject::addObject(ZObject& pObject)
{

    int wIndexBase= (int)(vertexCount()-1);
    for (long wi=0;wi< pObject.vertexCount();wi++)
                    VertexArray.push(pObject.VertexArray.Tab[wi]);
    for (long wi=0;wi<pObject.indexCount();wi++)
                    IndexArray.push(pObject.index(wi)+wIndexBase);
    return;
}


#endif // ZGLOBJECT_H
