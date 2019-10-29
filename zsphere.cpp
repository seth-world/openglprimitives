///////////////////////////////////////////////////////////////////////////////
// Sphere.cpp
// ==========
// Sphere for OpenGL with (radius, sectors, stacks)
// The min number of sectors is 3 and the min number of stacks are 2.
//
//  AUTHOR: Song Ho Ahn (song.ahn@gmail.com)
// CREATED: 2017-11-01
// UPDATED: 2018-12-13
///////////////////////////////////////////////////////////////////////////////

#ifdef _WIN32
#include <windows.h>    // include windows.h to avoid thousands of compile errors even though this class is not depending on Windows
#endif

#ifdef __APPLE__
#include <OpenGL/gl.h>
#else
#ifdef __USE_GLAD__
#include <glad/glad.h>
#else
#include <GL/glew.h>
#endif
#endif

#include <iostream>
#include <iomanip>
#include <cmath>
#include <zsphere.h>



// constants //////////////////////////////////////////////////////////////////
const int MIN_SECTOR_COUNT = 3;
const int MIN_STACK_COUNT  = 2;



///////////////////////////////////////////////////////////////////////////////
// ctor
///////////////////////////////////////////////////////////////////////////////
ZSphere::ZSphere(float radius, int sectors, int stacks, bool smooth,const char* pName) :
    ZObject(pName,ZObject::Sphere)
{
    generate(radius, sectors, stacks, smooth);
}



///////////////////////////////////////////////////////////////////////////////
// setters
///////////////////////////////////////////////////////////////////////////////
void ZSphere::generate(float radius, int sectors, int stacks, bool smooth)
{
    this->radius = radius;
    this->sectorCount = sectors;
    if(sectors < MIN_SECTOR_COUNT)
        this->sectorCount = MIN_SECTOR_COUNT;
    this->stackCount = stacks;
    if(sectors < MIN_STACK_COUNT)
        this->sectorCount = MIN_STACK_COUNT;
    this->smooth = smooth;

    if(smooth)
        buildVerticesSmooth();
    else
        buildVertices();

    ZObject::setComputeNormals(false);
    ZObject::setComputeTexCoords(false);
}

void ZSphere::setRadius(float radius)
{
    this->radius = radius;
    updateRadius();
}

void ZSphere::setSectorCount(int sectors)
{
    generate(radius, sectors, stackCount, smooth);
}

void ZSphere::setStackCount(int stacks)
{
    generate(radius, sectorCount, stacks, smooth);
}

void ZSphere::setSmooth(bool smooth)
{
    if(this->smooth == smooth)
        return;

    this->smooth = smooth;
    if(smooth)
        buildVerticesSmooth();
    else
        buildVertices();
}



///////////////////////////////////////////////////////////////////////////////
// print itself
///////////////////////////////////////////////////////////////////////////////
void ZSphere::print(const int pLimit,FILE*pOutput) const
{
    std::cout << "===== Sphere " << Name << "=====\n"
              << "        Radius: " << radius << "\n"
              << "  Sector Count: " << sectorCount << "\n"
              << "   Stack Count: " << stackCount << "\n"
              << "Smooth Shading: " << (smooth ? "true" : "false") << "\n"
              << "Triangle Count: " << getTriangleCount() << "\n"
              << "   Index Count: " << getIndexCount() << "\n"
              << "  Vertex Count: " << vertices.count() << "\n" << std::endl;

    ZObject::print(pLimit,pOutput);

}
/*
void ZSphere::GLsetup()
{
    // interleaved array
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_NORMAL_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    glVertexPointer(3, GL_FLOAT, interleavedStride, &interleavedVertices[0].point);
    glNormalPointer(GL_FLOAT, interleavedStride, &interleavedVertices[0].normal);
    glTexCoordPointer(2, GL_FLOAT, interleavedStride, &interleavedVertices[0].textcoords);


}
*/
///////////////////////////////////////////////////////////////////////////////
// draw a sphere in VertexArray mode
// OpenGL RC must be set before calling it
///////////////////////////////////////////////////////////////////////////////
/*void ZSphere::draw_old() const
{
    // interleaved array
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_NORMAL_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    glVertexPointer(3, GL_FLOAT, interleavedStride, &interleavedVertices[0].point);
    glNormalPointer(GL_FLOAT, interleavedStride, &interleavedVertices[0].normal);
    glTexCoordPointer(2, GL_FLOAT, interleavedStride, &interleavedVertices[0].textcoords);

    glDrawElements(GL_TRIANGLES, (unsigned int)Indices.size(), GL_UNSIGNED_INT, Indices.data());

    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_NORMAL_ARRAY);
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);
}*/
/*
void ZSphere::draw() const
{
    // interleaved array
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_NORMAL_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    glVertexPointer(3, GL_FLOAT, interleavedStride, &interleavedVertices[0].point);
    glNormalPointer(GL_FLOAT, interleavedStride, &interleavedVertices[0].normal);
    glTexCoordPointer(2, GL_FLOAT, interleavedStride, &interleavedVertices[0].textcoords);

    glDrawElements(GL_TRIANGLES, (unsigned int)Indices.size(), GL_UNSIGNED_INT, Indices.data());

    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_NORMAL_ARRAY);
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);
}
*/


///////////////////////////////////////////////////////////////////////////////
// draw lines only
// the caller must set the line width before call this
///////////////////////////////////////////////////////////////////////////////
/*void ZSphere::drawLines(const float lineColor[4]) const
{
    // set line colour
    glColor4fv(lineColor);
    glMaterialfv(GL_FRONT, GL_DIFFUSE,   lineColor);

    // draw lines with VA
    glDisable(GL_LIGHTING);
    glDisable(GL_TEXTURE_2D);
    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(3, GL_FLOAT, 0, vertices.data());

    glDrawElements(GL_LINES, (unsigned int)lineIndices.size(), GL_UNSIGNED_INT, lineIndices.data());

    glDisableClientState(GL_VERTEX_ARRAY);
    glEnable(GL_LIGHTING);
    glEnable(GL_TEXTURE_2D);
}
*/


///////////////////////////////////////////////////////////////////////////////
// draw a sphere surfaces and lines on top of it
// the caller must set the line width before call this
///////////////////////////////////////////////////////////////////////////////
/*void ZSphere::drawWithLines(const float lineColor[4]) const
{
    glEnable(GL_POLYGON_OFFSET_FILL);Normals
    glPolygonOffset(1.0, 1.0f); // move polygon backward
    drawGL(GL_TRIANGLE_STRIP);
    glDisable(GL_POLYGON_OFFSET_FILL);

    // draw lines with VA
    drawLines(lineColor);
}
*/


///////////////////////////////////////////////////////////////////////////////
// update vertex positions only
///////////////////////////////////////////////////////////////////////////////
void ZSphere::updateRadius()
{
    float scale = sqrtf(radius * radius / ( vertices[0].point.x * vertices[0].point.x +
                                            vertices[0].point.y * vertices[0].point.y +
                                            vertices[0].point.z * vertices[0].point.z));

    for(long i = 0; i < vertices.count(); i ++)
                {
                vertices[i].point *= scale;
                }
  /*  float scale = sqrtf(radius * radius / ( vertices_only[0].x * vertices_only[0].x +
                                            vertices_only[0].y * vertices_only[0].y +
                                            vertices_only[0].z * vertices_only[0].z));

    std::size_t i, j;
    std::size_t count = vertices_only.size();
    for(i = 0, j = 0; i < count; i ++, j ++)
    {
        vertices_only[i]   *= scale;
//        vertices[i].x   *= scale;
//        vertices[i].y   *= scale;
//        vertices[i].z   *= scale;

        // for interleaved array
        interleavedVertices[j].point   *= scale;
//        interleavedVertices[j+1] *= scale;
//        interleavedVertices[j+2] *= scale;

    }
*/
    ZObject::setComputeNormals(false);
    ZObject::setComputeTexCoords(false);
}



///////////////////////////////////////////////////////////////////////////////
// dealloc vectors
///////////////////////////////////////////////////////////////////////////////
void ZSphere::clearArrays()
{
 /*   std::vector<float>().swap(vertices);
    std::vector<float>().swap(normals);
    std::vector<float>().swap(texCoords);
    std::vector<unsigned int>().swap(Indices);
    std::vector<unsigned int>().swap(lineIndices);
    */
//    vertices_only.clear();
//    normals.clear();
//    texCoords.clear();
    lineIndices.clear();
    Indices.clear();
    vertices.clear();
}



///////////////////////////////////////////////////////////////////////////////
// build vertices of sphere with smooth shading using parametric equation
// x = r * cos(u) * cos(v)
// y = r * cos(u) * sin(v)
// z = r * sin(u)
// where u: stack(latitude) angle (-90 <= u <= 90)
//       v: sector(longitude) angle (0 <= v <= 360)
///////////////////////////////////////////////////////////////////////////////
void ZSphere::buildVerticesSmooth()
{
    const float PI = 3.1415926f;

    // clear memory of prev arrays
    clearArrays();

    float x, y, z, xy;                              // vertex position
    float nx, ny, nz, lengthInv = 1.0f / radius;    // normal
    float s, t;                                     // texCoord

    float sectorStep = 2 * PI / sectorCount;
    float stackStep = PI / stackCount;
    float sectorAngle, stackAngle;

    for(int i = 0; i <= stackCount; ++i)
    {
        stackAngle = PI / 2 - i * stackStep;        // starting from pi/2 to -pi/2
        xy = radius * cosf(stackAngle);             // r * cos(u)
        z = radius * sinf(stackAngle);              // r * sin(u)

        // add (sectorCount+1) vertices per stack
        // the first and last vertices have same position and normal, but different tex coords
        for(int j = 0; j <= sectorCount; ++j)
        {
            sectorAngle = j * sectorStep;           // starting from 0 to 2pi

            // vertex position
            x = xy * cosf(sectorAngle);             // r * cos(u) * cos(v)
            y = xy * sinf(sectorAngle);             // r * cos(u) * sin(v)
//            addVertex(x, y, z);

            // normalized vertex normal
            nx = x * lengthInv;
            ny = y * lengthInv;
            nz = z * lengthInv;

//            addNormal(nx, ny, nz);

            // vertex tex coord between [0, 1]
            s = (float)j / sectorCount;
            t = (float)i / stackCount;
//            addTexCoord(s, t);

            vertices << ZVertice(x,y,z,nx,ny,nz,s,t);

        }// for j
    }//for i

    // Indices
    //  k1--k1+1
    //  |  / |
    //  | /  |
    //  k2--k2+1
    unsigned int k1, k2;
    for(int i = 0; i < stackCount; ++i)
    {
        k1 = i * (sectorCount + 1);     // beginning of current stack
        k2 = k1 + sectorCount + 1;      // beginning of next stack

        for(int j = 0; j < sectorCount; ++j, ++k1, ++k2)
        {
            // 2 triangles per sector excluding 1st and last stacks
            if(i != 0)
            {
                addIndices(k1, k2, k1+1);   // k1---k2---k1+1
            }

            if(i != (stackCount-1))
            {
                addIndices(k1+1, k2, k2+1); // k1+1---k2---k2+1
            }


            // vertical lines for all stacks
            lineIndices.push_back(k1);
            lineIndices.push_back(k2);
            if(i != 0)  // horizontal lines except 1st stack
            {
                lineIndices.push_back(k1);
                lineIndices.push_back(k1 + 1);
            }
        }
    }

    // generate interleaved vertex array as well
    //buildInterleavedVertices();
}



///////////////////////////////////////////////////////////////////////////////
// generate vertices with flat shading
// each triangle is independent (no shared vertices)
///////////////////////////////////////////////////////////////////////////////
void ZSphere::buildVertices()
{
    const float PI = 3.1415926f;

    // tmp vertex definition (x,y,z,s,t)
    struct Vertex
    {
        float x, y, z, s, t;
    };
    zbs::ZArray<Vertex> tmpVertices;

    float sectorStep = 2 * PI / sectorCount;
    float stackStep = PI / stackCount;
    float sectorAngle, stackAngle;

    // compute all vertices first, each vertex contains (x,y,z,s,t) except normal
    for(int i = 0; i <= stackCount; ++i)
    {
        stackAngle = PI / 2 - i * stackStep;        // starting from pi/2 to -pi/2
        float xy = radius * cosf(stackAngle);       // r * cos(u)
        float z = radius * sinf(stackAngle);        // r * sin(u)

        // add (sectorCount+1) vertices per stack
        // the first and last vertices have same position and normal, but different tex coords
        for(int j = 0; j <= sectorCount; ++j)
        {
            sectorAngle = j * sectorStep;           // starting from 0 to 2pi

            Vertex vertex;
            vertex.x = xy * cosf(sectorAngle);      // x = r * cos(u) * cos(v)
            vertex.y = xy * sinf(sectorAngle);      // y = r * cos(u) * sin(v)
            vertex.z = z;                           // z = r * sin(u)
            vertex.s = (float)j/sectorCount;        // s
            vertex.t = (float)i/stackCount;         // t
            tmpVertices.push_back(vertex);
        } // for j
    }// for i

    // clear memory of prev arrays
    clearArrays();

    Vertex v1, v2, v3, v4;                          // 4 vertex positions and tex coords
//    zbs::ZArray<float> n;                           // 1 face normal
    Vertice_type        wNormal;                    // 1 face normal
    ZVertice            wPoint;                     // storage unit : vertices - normals - texture coords

    int i, j, k, vi1, vi2;
    int index = 0;                                  // index for vertex
    for(i = 0; i < stackCount; ++i)
    {
        vi1 = i * (sectorCount + 1);                // index of tmpVertices
        vi2 = (i + 1) * (sectorCount + 1);

        for(j = 0; j < sectorCount; ++j, ++vi1, ++vi2)
        {
            // get 4 vertices per sector
            //  v1--v3
            //  |    |
            //  v2--v4
            v1 = tmpVertices[vi1];
            v2 = tmpVertices[vi2];
            v3 = tmpVertices[vi1 + 1];
            v4 = tmpVertices[vi2 + 1];

            // if 1st stack and last stack, store only 1 triangle per sector
            // otherwise, store 2 triangles (quad) per sector
            if(i == 0) // a triangle for first stack v1 v2 v4 ==========================
            {
                // put a triangle
                wNormal = computeFaceNormal(v1.x,v1.y,v1.z, v2.x,v2.y,v2.z, v4.x,v4.y,v4.z);

                vertices << ZVertice(v1.x,v1.y,v1.z,wNormal.x,wNormal.y,wNormal.z,v1.s,v1.t);
                vertices << ZVertice(v2.x,v2.y,v2.z,wNormal.x,wNormal.y,wNormal.z,v2.s,v2.t);
                vertices << ZVertice(v4.x,v4.y,v4.z,wNormal.x,wNormal.y,wNormal.z,v4.s,v4.t);
                // put Indices of 1 triangle
                Indices.push_back(index);
                Indices.push_back(index+1);
                Indices.push_back(index+2);


   /*             addVertex(v1.x, v1.y, v1.z);
                addVertex(v2.x, v2.y, v2.z);
                addVertex(v4.x, v4.y, v4.z);

                // put tex coords of triangle
                addTexCoord(v1.s, v1.t);
                addTexCoord(v2.s, v2.t);
                addTexCoord(v4.s, v4.t);

                // put normal
                wNormal = computeFaceNormal(v1.x,v1.y,v1.z, v2.x,v2.y,v2.z, v4.x,v4.y,v4.z);
                normals.push_back(wNormal);
                normals.push_back(wNormal);
                normals.push_back(wNormal);

                // put Indices of 1 triangle
                Indices.push_back(index);
                Indices.push_back(index+1);
                Indices.push_back(index+2);

                for(k = 0; k < 3; ++k)  // same normals for 3 vertices
                {
                    addNormal(n[0], n[1], n[2]);
                }

                // put Indices of 1 triangle
                addIndices(index, index+1, index+2);
*/
                // Indices for line (first stack requires only vertical line)
                lineIndices.push_back(index);
                lineIndices.push_back(index+1);

                index += 3;     // for next
            }
            else if(i == (stackCount-1)) // a triangle for last stack  v1 v2 v3 =========
            {
                wNormal = computeFaceNormal(v1.x,v1.y,v1.z, v2.x,v2.y,v2.z, v3.x,v3.y,v3.z);

                vertices << ZVertice(v1.x,v1.y,v1.z,wNormal.x,wNormal.y,wNormal.z,v1.s,v1.t);
                vertices << ZVertice(v2.x,v2.y,v2.z,wNormal.x,wNormal.y,wNormal.z,v2.s,v2.t);
                vertices << ZVertice(v3.x,v3.y,v3.z,wNormal.x,wNormal.y,wNormal.z,v3.s,v3.t);

                // put Indices of 1 triangle
                Indices.push_back(index);
                Indices.push_back(index+1);
                Indices.push_back(index+2);

     /*
                // put a triangle
                addVertex(v1.x, v1.y, v1.z);
                addVertex(v2.x, v2.y, v2.z);
                addVertex(v3.x, v3.y, v3.z);

                // put tex coords of triangle
                addTexCoord(v1.s, v1.t);
                addTexCoord(v2.s, v2.t);
                addTexCoord(v3.s, v3.t);

                // put normal
                wNormal = computeFaceNormal(v1.x,v1.y,v1.z, v2.x,v2.y,v2.z, v3.x,v3.y,v3.z);
                normals.push_back(wNormal);
                normals.push_back(wNormal);
                normals.push_back(wNormal);
                // put Indices of 1 triangle
                Indices.push_back(index);
                Indices.push_back(index+1);
                Indices.push_back(index+2);
*/
/*
                // put normal
                n = computeFaceNormal(v1.x,v1.y,v1.z, v2.x,v2.y,v2.z, v3.x,v3.y,v3.z);
                for(k = 0; k < 3; ++k)  // same normals for 3 vertices
                {
                    addNormal(n[0], n[1], n[2]);
                }

                // put Indices of 1 triangle
                addIndices(index, index+1, index+2);
*/

                // Indices for lines (last stack requires both vert/hori lines)
                lineIndices.push_back(index);
                lineIndices.push_back(index+1);
                lineIndices.push_back(index);
                lineIndices.push_back(index+2);

                index += 3;     // for next
            }
            else // 2 triangles for others ====================================
            {
                // put quad vertices: v1-v2-v3-v4
                wNormal = computeFaceNormal(v1.x,v1.y,v1.z, v2.x,v2.y,v2.z, v3.x,v3.y,v3.z);

                vertices << ZVertice(v1.x,v1.y,v1.z,wNormal.x,wNormal.y,wNormal.z,v1.s,v1.t);
                vertices << ZVertice(v2.x,v2.y,v2.z,wNormal.x,wNormal.y,wNormal.z,v2.s,v2.t);
                vertices << ZVertice(v3.x,v3.y,v3.z,wNormal.x,wNormal.y,wNormal.z,v3.s,v3.t);
                vertices << ZVertice(v4.x,v4.y,v4.z,wNormal.x,wNormal.y,wNormal.z,v4.s,v4.t);

                // put Indices of quad (2 triangles)
                Indices.push_back(index);
                Indices.push_back(index+1);
                Indices.push_back(index+2);

                Indices.push_back(index+2);
                Indices.push_back(index+1);
                Indices.push_back(index+3);

 /*
                // put quad vertices: v1-v2-v3-v4
                addVertex(v1.x, v1.y, v1.z);
                addVertex(v2.x, v2.y, v2.z);
                addVertex(v3.x, v3.y, v3.z);
                addVertex(v4.x, v4.y, v4.z);

                // put tex coords of quad
                addTexCoord(v1.s, v1.t);
                addTexCoord(v2.s, v2.t);
                addTexCoord(v3.s, v3.t);
                addTexCoord(v4.s, v4.t);

                // put normal
                wNormal = computeFaceNormal(v1.x,v1.y,v1.z, v2.x,v2.y,v2.z, v3.x,v3.y,v3.z);
                normals.push_back(wNormal);
                normals.push_back(wNormal);
                normals.push_back(wNormal);
                // put Indices of quad (2 triangles)
                Indices.push_back(index);
                Indices.push_back(index+1);
                Indices.push_back(index+2);

                Indices.push_back(index+2);
                Indices.push_back(index+1);
                Indices.push_back(index+3);

                // put normal
                n = computeFaceNormal(v1.x,v1.y,v1.z, v2.x,v2.y,v2.z, v3.x,v3.y,v3.z);
                for(k = 0; k < 4; ++k)  // same normals for 4 vertices
                {
                    addNormal(n[0], n[1], n[2]);
                }

                // put Indices of quad (2 triangles)
                addIndices(index, index+1, index+2);
                addIndices(index+2, index+1, index+3);
*/
                // Indices for lines
                lineIndices.push_back(index);
                lineIndices.push_back(index+1);
                lineIndices.push_back(index);
                lineIndices.push_back(index+2);

                index += 4;     // for next
            } // else
        }// for j
    }// for i

    // generate interleaved vertex array as well
//    buildInterleavedVertices();
}//buildVerticesFlat



///////////////////////////////////////////////////////////////////////////////
// generate interleaved vertices: V/N/T
// stride must be 32 bytes
///////////////////////////////////////////////////////////////////////////////
void ZSphere::buildInterleavedVertices()
{
//    std::vector<float>().swap(interleavedVertices);

    long i;
    long count = vertices_only.size();
    for(i = 0; i < count; i ++)
    {
 //       Vertice_type wN=normals[i];
        ZObject::vertices.push_back(ZVertice(vertices_only[i],normals[i],texCoords[i]));
        //interleavedVertices.push_back(ZVertice(vertices[i],normals[i],texCoords[i]));

 /*       interleavedVertices.push_back(vertices[i]);
        interleavedVertices.push_back(vertices[i+1]);
        interleavedVertices.push_back(vertices[i+2]);

        interleavedVertices.push_back(normals[i]);
        interleavedVertices.push_back(normals[i+1]);
        interleavedVertices.push_back(normals[i+2]);

        interleavedVertices.push_back(texCoords[j]);
        interleavedVertices.push_back(texCoords[j+1]);
        */
    }
    for(i = 0; i < Indices.count(); i ++)
    {
        ZObject::Indices.push_back(Indices[i]);
    }
}



////////////////////////(long)///////////////////////////////////////////////////////
// add single vertex to array
///////////////////////////////////////////////////////////////////////////////
void ZSphere::addVertex(float x, float y, float z)
{
    vertices_only.push_back(Vertice_type(x, y,  z));
//    vertices.push_back(y);
//    vertices.push_back(z);
}



///////////////////////////////////////////////////////////////////////////////
// add single normal to array
///////////////////////////////////////////////////////////////////////////////
void ZSphere::addNormal(float nx, float ny, float nz)
{
    normals.push_back(Vertice_type(nx, ny,  nz));
//    normals.push_back(nx);
//    normals.push_back(ny);
//    normals.push_back(nz);
}



///////////////////////////////////////////////////////////////////////////////
// add single texture coord to array
///////////////////////////////////////////////////////////////////////////////
void ZSphere::addTexCoord(float s, float t)
{
    texCoords.push_back(TexCoords_type(s, t));
//    texCoords.push_back(s);
//    texCoords.push_back(t);
}



///////////////////////////////////////////////////////////////////////////////
// add 3 Indices to array
///////////////////////////////////////////////////////////////////////////////
void ZSphere::addIndices(unsigned int i1, unsigned int i2, unsigned int i3)
{
    Indices.push_back(i1);
    Indices.push_back(i2);
    Indices.push_back(i3);
}



///////////////////////////////////////////////////////////////////////////////
// return face normal of a triangle v1-v2-v3
// if a triangle has no surface (normal length = 0), then return a zero vector
///////////////////////////////////////////////////////////////////////////////
/*zbs::ZArray<float> ZSphere::computeFaceNormal(float x1, float y1, float z1,  // v1
                                             float x2, float y2, float z2,  // v2
                                             float x3, float y3, float z3)  // v3*/
Vertice_type ZSphere::computeFaceNormal(float x1, float y1, float z1,  // v1
                                        float x2, float y2, float z2,  // v2
                                        float x3, float y3, float z3)  // v3
{
    const float EPSILON = 0.000001f;
    Vertice_type wNormal=Vertice_type(0.0f,0.0f,0.0f); // default return value (0,0,0)

//    zbs::ZArray<float> normal;
//    normal.addValues(3, 0.0f);     // default return value (0,0,0)
    float nx, ny, nz;

    // find 2 edge vectors: v1-v2, v1-v3
    float ex1 = x2 - x1;
    float ey1 = y2 - y1;
    float ez1 = z2 - z1;
    float ex2 = x3 - x1;
    float ey2 = y3 - y1;
    float ez2 = z3 - z1;

    // cross product: e1 x e2
    nx = ey1 * ez2 - ez1 * ey2;
    ny = ez1 * ex2 - ex1 * ez2;
    nz = ex1 * ey2 - ey1 * ex2;

    // normalize only if the length is > 0
    float length = sqrtf(nx * nx + ny * ny + nz * nz);


    if(length > EPSILON)
    {
        // normalize
        float lengthInv = 1.0f / length;
        wNormal.x= nx * lengthInv;
        wNormal.y = ny * lengthInv;
        wNormal.z = nz * lengthInv;
 /*       normal[0] = nx * lengthInv;
        normal[1] = ny * lengthInv;
        normal[2] = nz * lengthInv;*/
    }

//    return normal;
    return wNormal;
}
