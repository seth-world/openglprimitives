#include "zobject.h"
#include <zobjectfunctions.h>
#include <ztexture.h>

//Vertice_type CalculateSurfaceNormal (Vertice_type* pTriangle,ZObject::NormalDirection pFront);

ZObject::ZObject(const char *pName)
{
    Name=pName;
    isMinMaxInit=false;
    GLResources->registerObject(this);
}

void ZObject::copy(const ZObject&pObject)
{
    GLResources->registerObject(this);
    Name=pObject.Name;
    for (int wi=0;wi<pObject.vertices.size();wi++)
        vertices.push_back(pObject.vertices[wi]);
    for (int wi=0;wi<pObject.VName.size();wi++)
        VName.push_back(pObject.VName[wi]);
    for (int wi=0;wi<pObject.Indices.size();wi++)
        Indices.push_back(pObject.Indices[wi]);

    for (int wi=0;wi<pObject.VNormalDir.size();wi++)
        VNormalDir.push_back(pObject.VNormalDir[wi]);

    if (pObject.GLDescriptor)
            GLDescriptor=new ZGLObjDescriptor(pObject.GLDescriptor);
    else {
            GLDescriptor=nullptr;
         }
}
void ZObject::copy(const ZObject&&pObject)
{
    copy(pObject);
}


ZObject&
ZObject::addObject(ZObject &pObject)
{
    for (long wi=0;wi<pObject.vertices.size();wi++)
        {
        vertices.push_back(pObject.vertices[wi]);
        }
    for (long wi=0;wi<pObject.Indices.size();wi++)
        {
        Indices.push_back(pObject.Indices[wi]);
        }
//    Normals=pObject.Normals;
    VNormalDir = pObject.VNormalDir;
    VName=pObject.VName;
    return *this;
}

void
ZObject::addVertice(Vertice_type pInput, const char* pName)
{
    ZVertice wVertice(pInput);
    vertices.push_back(wVertice);
    VName.push_back(pName);

    /* compute min and max for x and y to further compute texture coords */
    if (!isMinMaxInit)
            {
            XMin=XMax=pInput.x;
            YMin=YMax=pInput.y;
            isMinMaxInit=true;
            }
    if (XMin>pInput.x)
            XMin= pInput.x;
    if (XMax<pInput.x)
            XMax= pInput.x;
    if (YMin>pInput.y)
            YMin= pInput.y;
    if (YMax<pInput.y)
            YMax= pInput.y;
    return;
}//addVertice

void
ZObject::computeTexCoords()
{
    float xmin=vertices[0].point.x;
    float xmax=vertices[0].point.x;
    float ymin=vertices[0].point.y;
    float ymax=vertices[0].point.y;
    for (long wi=1;wi<vertices.size();wi++)
    {
        if(vertices[wi].point.x > xmax) xmax = vertices[wi].point.x ;
        if(vertices[wi].point.x < xmin) xmin = vertices[wi].point.x ;
        if(vertices[wi].point.y > ymax) ymax = vertices[wi].point.y ;
        if(vertices[wi].point.y < ymin) ymin = vertices[wi].point.y ;
    }
    float wXDividor = xmax-xmin;
    float wYDividor = ymax-ymin;

/*    printf (" xmin %f xmax %f ymin %f ymax %f wXDividor %f wYDividor %f\n",
            xmin,
            xmax,
            ymin,
            ymax,
            wXDividor,
            wYDividor
            );*/

    for (long wi=0;wi<vertices.size();wi++)
    {
        vertices[wi].textcoords.x = (vertices[wi].point.x - xmin) / wXDividor;
        vertices[wi].textcoords.y = (vertices[wi].point.y - ymin) / wYDividor;
    }
    return;
}//computeTexCoords



void
ZObject::createGL_ObjectArray(  uint8_t pShaderSetupOpt )
        //unsigned int pShaderVerticePosition,
        //                        int pShaderNormalPosition)
{



    Shader->use();

/* GL buffers setup */

    glGenVertexArrays(1, &GLDescriptor->VAO);
    glGenBuffers(1, &GLDescriptor->VBO);

    glBindVertexArray(GLDescriptor->VAO);
    glBindBuffer(GL_ARRAY_BUFFER, GLDescriptor->VBO);

//    glBufferData(GL_ARRAY_BUFFER, ZAObjectVertices.usedSize(), ZAObjectVertices.data(), GL_STATIC_DRAW);
    glBufferData(GL_ARRAY_BUFFER, vertices.usedSize(), vertices.data(), GL_STATIC_DRAW);

/* point position */
    {
    int wPosition=(GLuint)glGetAttribLocation(Shader->getShaderId(),"aPosition");
    if (wPosition<0)
        GLDescriptor->PositionAttribArray= cst_defaultPositionLocation;
    else
        GLDescriptor->PositionAttribArray=(GLuint)wPosition;

    }
   glVertexAttribPointer(GLDescriptor->PositionAttribArray, 3, GL_FLOAT, GL_FALSE, (GLsizei)getStride(), (void*)verticeOffset);
   glEnableVertexAttribArray(GLDescriptor->PositionAttribArray);

/* normals */
    if (pShaderSetupOpt&setupNormals)
        {
        if (havetoComputeNormals())
                    this->computeNormals();

        int wNormal=(GLuint)glGetAttribLocation(Shader->getShaderId(),"aNormal");
        if (wNormal<0)
            GLDescriptor->NormalAttribArray= cst_defaultNormalLocation;
        else
            GLDescriptor->NormalAttribArray=(GLuint)wNormal;
/* debug */
        zbs::ZArray<Vertice_type> wNormals=toFlatNormals();
        glGenBuffers(1, &GLDescriptor->NormVBO);
        glBindBuffer(GL_ARRAY_BUFFER, GLDescriptor->NormVBO);
        glBufferData(GL_ARRAY_BUFFER, wNormals.usedSize(), wNormals.data(), GL_STATIC_DRAW);
        glVertexAttribPointer(GLDescriptor->NormalAttribArray, 3, GL_FLOAT, GL_FALSE, (GLsizei)sizeof(Vertice_type), (void*)0);
        glEnableVertexAttribArray(GLDescriptor->NormalAttribArray);
/* end debug */
//        glVertexAttribPointer(aNormal, 3, GL_FLOAT, GL_FALSE, (GLsizei)getStride(), (void*)normalOffset);
//        glEnableVertexAttribArray(aNormal);
        }

/* texture coordinates */

    if (pShaderSetupOpt&setupTextures)
        {
        if (havetoComputeTexCoords())
                    this->computeTexCoords();
        int wTextCoords=(GLuint)glGetAttribLocation(Shader->getShaderId(),"aTexCoords");
        if (wTextCoords<0)
            GLDescriptor->TexCoordsAttribArray= cst_defaultTexCoordsLocation;
        else
            GLDescriptor->TexCoordsAttribArray=(GLuint)wTextCoords;
        /* debug */
            zbs::ZArray<TextCoords_type> wTexCoords=toFlatTexCoords();
            glGenBuffers(1, &GLDescriptor->TexVBO);
            glBindBuffer(GL_ARRAY_BUFFER, GLDescriptor->TexVBO);
            glBufferData(GL_ARRAY_BUFFER, wTexCoords.usedSize(), wTexCoords.data(), GL_STATIC_DRAW);
            glVertexAttribPointer(GLDescriptor->TexCoordsAttribArray, 3, GL_FLOAT, GL_FALSE, (GLsizei)sizeof(TextCoords_type), (void*)0);
            glEnableVertexAttribArray(GLDescriptor->TexCoordsAttribArray);
        /* end debug */

 //       glVertexAttribPointer(atextures, 3, GL_FLOAT, GL_FALSE, (GLsizei)getStride(), (void*)textureOffset);
//        glEnableVertexAttribArray(atextures);
        }

// You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
// VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
    // unbind
// note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
    glBindBuffer(GL_ARRAY_BUFFER, 0);

// unbind
    glBindVertexArray(0);

    Shader->release();
} // createGL_ObjectArray

void
ZObject::createGL_ObjectElement(  uint8_t pShaderSetupOpt )
{

    Shader->use();

        glGenVertexArrays(1, &GLDescriptor->VAO);
        glGenBuffers(1, &GLDescriptor->VBO);
        glGenBuffers(1, &GLDescriptor->EBO);

// bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
        glBindVertexArray(GLDescriptor->VAO);

        glBindBuffer(GL_ARRAY_BUFFER, GLDescriptor->VBO);
        glBufferData(GL_ARRAY_BUFFER, vertices.usedSize(), vertices.data(), GL_STATIC_DRAW);

/* point position */
        {
        int wPosition=(GLuint)glGetAttribLocation(Shader->getShaderId(),"aPosition");
        if (wPosition<0)
            GLDescriptor->PositionAttribArray= cst_defaultPositionLocation;
        else
            GLDescriptor->PositionAttribArray=(GLuint)wPosition;

        }
       glVertexAttribPointer(GLDescriptor->PositionAttribArray, 3, GL_FLOAT, GL_FALSE, (GLsizei)getStride(), (void*)verticeOffset);
       glEnableVertexAttribArray(GLDescriptor->PositionAttribArray);

/* normals */
    if (pShaderSetupOpt&setupNormals)
            {
            if (havetoComputeNormals())
                    this->computeNormals();
            int wNormal=(GLuint)glGetAttribLocation(Shader->getShaderId(),"aNormal");
            if (wNormal<0)
                GLDescriptor->NormalAttribArray= cst_defaultNormalLocation;
            else
                GLDescriptor->NormalAttribArray=(GLuint)wNormal;

            glVertexAttribPointer(GLDescriptor->NormalAttribArray, 3, GL_FLOAT, GL_FALSE, (GLsizei)getStride(), (void*)normalOffset);
            glEnableVertexAttribArray(GLDescriptor->NormalAttribArray);
            }
/* texture coordinates */
    if (pShaderSetupOpt&setupTextures)
            {
            if (havetoComputeNormals())
                        this->computeTexCoords();
            int wTextCoords=(GLuint)glGetAttribLocation(Shader->getShaderId(),"aTexCoord");
            if (wTextCoords<0)
                GLDescriptor->TexCoordsAttribArray= cst_defaultTexCoordsLocation;
            else
                GLDescriptor->TexCoordsAttribArray=(GLuint)wTextCoords;

            glVertexAttribPointer(GLDescriptor->TexCoordsAttribArray, 3, GL_FLOAT, GL_FALSE, (GLsizei)getStride(), (void*)textureOffset);
            glEnableVertexAttribArray(GLDescriptor->TexCoordsAttribArray);
            }

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, GLDescriptor->EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, Indices.usedSize(),Indices.data(), GL_STATIC_DRAW); // same indices as vertices

    // remember: do NOT unbind the EBO while a VAO is active as the bound element buffer object IS stored in the VAO; keep the EBO bound.
    //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    // You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
    // VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
        // unbind
    // note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
        glBindBuffer(GL_ARRAY_BUFFER, 0);

    // unbind
        glBindVertexArray(0);
        Shader->release();
} // createGL_ObjectElement

ZTexture*
ZObject::setTexture(const char* pTextureName)
{
//    Shader->use();
    GLDescriptor->Texture = new ZTexture(pTextureName);
    if (GLDescriptor->Texture->Id >  0)
            {
            GLDescriptor->setUseTexture(true);
            return GLDescriptor->Texture;
            }
    GLDescriptor->setUseTexture(false);
    return nullptr;
}
void
ZObject::setTexture(ZTexture* pTexture)
{
//    Shader->use();
    GLDescriptor->Texture = pTexture;
    if (pTexture==nullptr)
            {
            GLDescriptor->setUseTexture(false);
            return;
            }
    if (GLDescriptor->Texture->Id >  0) /* if valid texture */
            GLDescriptor->setUseTexture(true); /* use it */
    else {
        GLDescriptor->setUseTexture(false);
        }
}// setTexture

bool ZObject::hasTexture()
{
    if (GLDescriptor->Texture== nullptr)
                return false;
    /* up here could have texture defined but not loaded successfully (Id must be <> 0 ) */
    return (GLDescriptor->Texture->getId()!=0);
}

void
ZObject::setupGL(ZShader* pShader,
                 uint8_t pShaderSetupOpt,
                 GLenum pMode,
                 ZTexture* pTexture)
{
    if (GLDescriptor!=nullptr)
            delete GLDescriptor;
    GLDescriptor=new ZGLObjDescriptor;

    Shader=pShader;
//    setDefaultAlpha(pAlpha);
//    setDefaultColor(pColor);
    setDrawFigure(pMode);

    if (pTexture!=nullptr)
            setTexture(pTexture);

    if (hasIndices())
        createGL_ObjectElement(pShaderSetupOpt);
    else
        createGL_ObjectArray(pShaderSetupOpt);
    return;
}

void
ZObject::drawGL(unsigned int pDrawFigure)
{
    if (Shader==nullptr)
        {
        fprintf (stderr,"ZObject::drawGL-F-MissShader   Shader definition is missing for ZObject <%s>\n",Name);
        abort();
        }
    Shader->use();
    Shader->setVec3("InColor",DefaultColor);
    Shader->setFloat("InAlpha",DefaultAlpha);

    if (GLDescriptor->hasTexture())
        {
        GLDescriptor->Texture->bind();
        }
#ifdef __COMMENT__
    if (GLDescriptor->hasTexture())
        {
        GLDescriptor->Texture->bind();

        /* Texture exists : set the choices from GLDescriptor preset options */
//        wUseTexture=true;
        Shader->setBool("useInColor",GLDescriptor->UseDefaultColor);
        Shader->setBool("useInAlpha",GLDescriptor->UseDefaultAlpha);
        Shader->setBool("useTexture",GLDescriptor->UseTexture);
        }
    else
    {
        Shader->setBool("useInColor",true);
        Shader->setBool("useInAlpha",true);
        Shader->setBool("useTexture",false);
    }
#endif // __COMMENT__

    if (hasIndices())
    {
        glBindBuffer(GL_ARRAY_BUFFER, GLDescriptor->VBO);
        glEnableVertexAttribArray(GLDescriptor->PositionAttribArray);
        if (GLDescriptor->NormVBO!=0)
            {
            glBindBuffer(GL_ARRAY_BUFFER, GLDescriptor->NormVBO);
            glEnableVertexAttribArray(GLDescriptor->NormalAttribArray);
            }
        if (GLDescriptor->TexVBO!=0)
            {
            glBindBuffer(GL_ARRAY_BUFFER, GLDescriptor->TexVBO);
            glEnableVertexAttribArray(GLDescriptor->TexCoordsAttribArray);
            }
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, GLDescriptor->EBO);
        glBindVertexArray(GLDescriptor->VAO);

        glDrawElements(pDrawFigure, Indices.count(), GL_UNSIGNED_INT, 0);

    }
    else
    {
        glBindBuffer(GL_ARRAY_BUFFER, GLDescriptor->VBO);
        if (GLDescriptor->NormVBO!=0)
            glBindBuffer(GL_ARRAY_BUFFER, GLDescriptor->NormVBO);
        if (GLDescriptor->TexVBO!=0)
            glBindBuffer(GL_ARRAY_BUFFER, GLDescriptor->TexVBO);
        glBindVertexArray(GLDescriptor->VAO); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized
        // activate attrib arrays
        glEnableVertexAttribArray(GLDescriptor->PositionAttribArray); /* there is at least PositionAttribArray to activate */
        if (GLDescriptor->NormalAttribArray>0)
                glEnableVertexAttribArray(GLDescriptor->NormalAttribArray);
        if (GLDescriptor->TexCoordsAttribArray>0)
                glEnableVertexAttribArray(GLDescriptor->TexCoordsAttribArray);
        glDrawArrays(pDrawFigure, 0 , vertices.count());
    }
    return;
} // drawGL

void
ZObject::drawGLLines()
{

    if (Shader==nullptr)
        {
        fprintf (stderr,"ZObject::drawGL-F-MissShader   Shader definition is missing for ZObject <%s>\n",Name);
        abort();
        }
    Shader->use();
    Shader->setVec3("InColor",DefaultColor);
    Shader->setFloat("InAlpha",DefaultAlpha);
    glDisable(GL_TEXTURE_2D);
    if (hasIndices())
    {
        glBindVertexArray(GLDescriptor->VAO);
        glBindBuffer(GL_ARRAY_BUFFER, GLDescriptor->VBO);
        if (GLDescriptor->NormVBO!=0)
            glBindBuffer(GL_ARRAY_BUFFER, GLDescriptor->NormVBO);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, GLDescriptor->EBO);
        glDrawElements(GL_LINES, Indices.count(), GL_UNSIGNED_INT, 0);

    }
    else
    {
        if (GLDescriptor->hasTexture())
             GLDescriptor->Texture->bind();
        glBindVertexArray(GLDescriptor->VAO); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized
        glBindBuffer(GL_ARRAY_BUFFER, GLDescriptor->VBO);
        if (GLDescriptor->NormVBO!=0)
            glBindBuffer(GL_ARRAY_BUFFER, GLDescriptor->NormVBO);


        glDrawArrays(GL_LINES, 0 , vertices.count());
    }
    return;
} // drawGL

/*
zbs::ZArray<Color_type>
ZObject::toFlatColors()
{
    zbs::ZArray<Color_type> wFlat;
    if (hasIndices())
    {
        for (size_t wi=0;wi<Indices.size();wi++)
        {
            wFlat.push_back(vertices[Indices[wi]].color);
        }
        return wFlat;
    }
    for (size_t wi=0;wi<vertices.size();wi++)
    {
        wFlat.push_back(vertices[wi].color);
    }
    return wFlat;
}//toFlatColors
*/
zbs::ZArray<Vertice_type>
ZObject::toRawVertices()
{
    zbs::ZArray<Vertice_type> wFlat;
    for (size_t wi=0;wi<vertices.size();wi++)
        {
        wFlat.push_back(vertices[wi].point);
        }
    return wFlat;
}//toRawVertices

zbs::ZArray<Vertice_type>
ZObject::toFlatVertices()
{
    zbs::ZArray<Vertice_type> wFlat;
    if (hasIndices())
    {
        for (size_t wi=0;wi<Indices.size();wi++)
        {
            wFlat.push_back(vertices[Indices[wi]].point);
        }
        return wFlat;
    }

    for (size_t wi=0;wi<vertices.size();wi++)
    {
        wFlat.push_back(vertices[wi].point);
    }
    return wFlat;
}//toFlatVertices

zbs::ZArray<Vertice_type>
ZObject::toRawNormals()
{
    zbs::ZArray<Vertice_type> wFlat;
    for (size_t wi=0;wi<vertices.size();wi++)
        {
        wFlat.push_back(vertices[wi].normal);
        }
    return wFlat;
}//toRawNormals

zbs::ZArray<Vertice_type>
ZObject::toFlatNormals()
{
    zbs::ZArray<Vertice_type> wFlat;
    if (hasIndices())
    {
        for (size_t wi=0;wi<Indices.size();wi++)
        {
            wFlat.push_back(vertices[Indices[wi]].normal);
        }
        return wFlat;
    }

    for (size_t wi=0;wi<vertices.size();wi++)
    {
        wFlat.push_back(vertices[wi].normal);
    }
    return wFlat;
}//toFlatNormals


zbs::ZArray<TextCoords_type>
ZObject::toFlatTexCoords()
{
    zbs::ZArray<TextCoords_type> wFlat;
    if (hasIndices())
    {
        for (size_t wi=0;wi<Indices.size();wi++)
        {
            wFlat.push_back(vertices[Indices[wi]].textcoords);
        }
        return wFlat;
    }

    for (size_t wi=0;wi<vertices.size();wi++)
    {
        wFlat.push_back(vertices[wi].textcoords);
    }
    return wFlat;
}//toFlatTexCoords

void
ZObject::print(int pLimit,FILE* pOutput)
{
    long wVCount=pLimit<0?vertices.size():(long)pLimit;
    long wICount=pLimit<0?Indices.size():(long)pLimit;
    char wLimitChar[20];
    if (pLimit<0)
        strcpy(wLimitChar,"All data");
    else
        sprintf(wLimitChar,"First %d",pLimit);

    fprintf (pOutput,
             "\nObject <%s>\n",
             Name==nullptr?"No Name":Name);
        fprintf (pOutput,"Vertices <%s>\n--------\n",wLimitChar);
            fprintf (pOutput,
                     "                  Vertices                            Normals                         Texture \n"
                     "     -----------------------------------------------------------------------------------------------------\n");
        for (long wi=0; wi < wVCount;wi++)
            {
            const char* wName;
            if (wi<VName.size())
                wName=VName[wi];
            else {
                wName="NoName";
                }
            fprintf (pOutput,"%s x:% 3f y:% 3f z:% 3f  x:% 3f y:% 3f z:% 3f s:% 3f t:% 3f \n",
                    wName,
                    vertices[wi].point.x,
                    vertices[wi].point.y,
                    vertices[wi].point.z,
                    vertices[wi].normal.x,
                    vertices[wi].normal.y,
                    vertices[wi].normal.z,
                    vertices[wi].textcoords.x,
                    vertices[wi].textcoords.y);
            }

        fprintf (pOutput,"Indices <%s>\n--------\n",wLimitChar);
        if (Indices.count()==0)
            fprintf (pOutput," No indices\n");
        else
        for (long wi=0; wi < wICount;wi+=3)
                fprintf (pOutput,"%u %u %u\n",Indices[wi],Indices[wi+1],Indices[wi+2]);

}// print

zbs::ZArray<Vertice_type>
ZObject::generateNormals()
{
    computeNormals();
    return toFlatNormals();
}

void
ZObject::computeNormals()
{
    Vertice_type wNormal;
    Vertice_type wTriangle[3];
    zbs::ZArray<Vertice_type> pReturn;
    long wNormIdx=0;
    for (unsigned int wi=0;wi<vertices.size();wi=wi+3)
       {
        if ((vertices.size()-wi)<3)
        {
          fprintf (stderr," Vertices object are not grouped per triangle\n");
          return ;
        }
        if (wNormIdx>=VNormalDir.size())
        {
        fprintf (stderr," Normal directions are not homogeneous per triangles for object\n");
        exit (EXIT_FAILURE);
        }
       switch (VNormalDir[wNormIdx])
           {
           case ZObject::Front:
               wNormal= ZObject::NDirFront;
           break;
           case ZObject::Back:
               wNormal= ZObject::NDirBack;
           break;
           case ZObject::Top:
               wNormal= ZObject::NDirTop;
           break;
           case ZObject::Bottom:
               wNormal= ZObject::NDirBottom;
           break;
           case ZObject::Left:
               wNormal= ZObject::NDirLeft;
           break;
           case ZObject::Right:
               wNormal= ZObject::NDirRight;
           break;
           default: /* case ZObject::Compute */
           wTriangle[0]=vertices[wi].point;/* compute normal for a triangle A B C */
           wTriangle[1]=vertices[wi+1].point;
           wTriangle[2]=vertices[wi+2].point;

           Vertice_type wNormal= glm::normalize(glm::cross(wTriangle[2] - wTriangle[0], wTriangle[1] - wTriangle[0]));
     //      wNormal=CalculateSurfaceNormal(wTriangle);
           } // switch
       vertices[wi].normal=wNormal;
       vertices[wi+1].normal=wNormal;
       vertices[wi+2].normal=wNormal;

       wNormIdx++;
       }// for loop

    return ;
}//ZObject::generateNormals()
