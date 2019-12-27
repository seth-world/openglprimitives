///////////////////////////////////////////////////////////////////////////////
// Sphere.h
// ========
// Sphere for OpenGL with (radius, sectors, stacks)
// The min number of sectors is 3 and The min number of stacks are 2.
//
//  AUTHOR: Song Ho Ahn (song.ahn@gmail.com)
// CREATED: 2017-11-01
// UPDATED: 2018-12-13
///////////////////////////////////////////////////////////////////////////////

#ifndef ZSPHERE_H
#define ZSPHERE_H

#ifdef __USE_GLAD__
#include <glad/glad.h>
#else
#include <GL/glew.h>
#endif

//#include <vector>
#define __USE_ZARRAY_COPY_CONTRUCTOR__
#include <ztoolset/zarray.h>
#include <zvertice.h>
#include <zobject.h>

class ZSphere : public ZObject
{
public:
    // ctor/dtor
    ZSphere
    (float radius=1.0f,
     int sectorCount=36,
     int stackCount=18,
     bool smooth=true,
     const char* pName=nullptr) : ZObject(pName,Sphere)
    {

        createVertexAndIndex(Draw);           // not necessary : arrays will be created by addVertex and addIndex routines
//        createVertexAndIndex(Shape);

        generate(radius, sectorCount, stackCount, smooth);
    }
    ~ZSphere() {}

    // getters/setters
    float getRadius() const                 { return radius; }
    int getSectorCount() const              { return sectorCount; }
    int getStackCount() const               { return stackCount; }
    void generate(float radius, int sectorCount, int stackCount, bool smooth=true);
    void setRadius(float radius);
    void setSectorCount(int sectorCount);
    void setStackCount(int stackCount);
    void setSmooth(bool smooth);

    // for vertex data
    unsigned int getIndexCount() const
    {
        if (!GLDesc[Draw]->Indexes)
                return 0;
        return (unsigned int)GLDesc[Draw]->Indexes->count();
    }
    unsigned int getLineIndexCount() const
    {
        if (!GLDesc[Shape])
            return 0;
        if (!GLDesc[Shape]->Indexes)
            return 0;
        return (unsigned int)GLDesc[Shape]->Indexes->count(); }
    unsigned int getTriangleCount() const   { return getIndexCount() / 3; }
//    const float* getVertices() const        { return (const float*)vertices_only.data(); }
//    const float* getNormals() const         { return (const float*)normals.data(); }
//    const float* getTexCoords() const       { return (const float*)texCoords.data(); }
    const unsigned int* getIndices() const  { return GLDesc[Draw]->Indexes->data(); }
    const unsigned int* getLineIndices() const  { return GLDesc[Shape]->Indexes->data(); }

    // for interleaved vertices: V/N/T
//    unsigned int getInterleavedVertexCount() const  { return getVertexCount(); }    // # of vertices
    //unsigned int getInterleavedVertexSize() const   { return (unsigned int)interleavedVertices.size() * sizeof(float); }    // # of bytes
    //int getInterleavedStride() const                { return interleavedStride; }   // should be 32 bytes
    //const float* getInterleavedVertices() const     { return (const float*)interleavedVertices.data(); }

    // draw in VertexArray mode
//    void GLsetup();
//    void draw() const;                                  // draw surface
//    void draw_old() const;                                  // draw surface
    void drawLines(const float lineColor[4]) const;     // draw lines only
    void drawWithLines(const float lineColor[4]) const; // draw surface and lines

    void print(const int pLimit=-1,FILE*pOutput=stdout) const;

protected:

private:
    // member functions
    void updateRadius();
    void buildVerticesSmooth(bool pBuildShape=false);
    void buildVertices();
    void buildInterleavedVertices();
//    void clearArrays();
//    void addVertex(float x, float y, float z);
//    void addNormal(float x, float y, float z);
    void addTexCoord(float s, float t);
    void addIndices_old(unsigned int i1, unsigned int i2, unsigned int i3);
    Vertice_type computeFaceNormal(float x1, float y1, float z1,
                                         float x2, float y2, float z2,
                                         float x3, float y3, float z3);

    // memeber vars
    float radius;
    int sectorCount;                        // longitude, # of slices
    int stackCount;                         // latitude, # of stacks
    bool smooth;
/*
    std::vector<float> vertices;
    std::vector<float> normals;
    std::vector<float> texCoords;
    std::vector<unsigned int> indices;
    std::vector<unsigned int> lineIndices;
*/
//    zbs::ZArray<Vertice_type> vertices_only;
//    zbs::ZArray<Vertice_type> normals;
//    zbs::ZArray<TexCoords_type> texCoords;
//    zbs::ZArray<unsigned int> indices;
//    zbs::ZArray<unsigned int> lineIndices;

    // interleaved
//    zbs::ZArray<ZVertice> interleavedVertices;
//    int interleavedStride;                  // # of bytes to hop to the next vertex (should be 32 bytes)

};

#endif // ZSPHERE_H
