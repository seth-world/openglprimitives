#include "zobject.h"
#include <zobjectfunctions.h>
#include <ztexture.h>

//Vertice_type CalculateSurfaceNormal (Vertice_type* pTriangle,ZObject::NormalDirection pFront);

ZObject::ZObject(const char *pName, ObjType pType)
{
    Name=pName;
    Type=pType;
//    isMinMaxInit=false;

    GLResources->registerObject(this);
}

void ZObject::_cloneFrom(ZObject&pIn)
{

    Name=pIn.Name;
    for (int wi=0;wi<pIn.vertices.size();wi++)
        vertices.push_back(pIn.vertices[wi]);
    for (int wi=0;wi<pIn.VName.size();wi++)
        VName.push_back(pIn.VName[wi]);
    for (int wi=0;wi<pIn.Indices.size();wi++)
        Indices.push_back(pIn.Indices[wi]);

    for (int wi=0;wi<pIn.VNormalDir.size();wi++)
        VNormalDir.push_back(pIn.VNormalDir[wi]);
    for (int wi=0;wi<pIn.ShapeIndices.size();wi++)
        ShapeIndices.push_back(pIn.ShapeIndices[wi]);
    for (int wi=0;wi<pIn.ZANormVisu.size();wi++)
        ZANormVisu.push_back(pIn.ZANormVisu[wi]);
    if (pIn.GLDescriptor)
            GLDescriptor=new ZGLObjDescriptor(pIn.GLDescriptor);
    else {
            GLDescriptor=nullptr;
         }
    if (pIn.GLNormVisuDesc)
            GLNormVisuDesc=new ZGLObjDescriptor(pIn.GLNormVisuDesc);
    else {
            GLNormVisuDesc=nullptr;
         }
    if (pIn.GLShapeDesc)
            GLShapeDesc=new ZGLObjDescriptor(pIn.GLShapeDesc);
    else {
            GLShapeDesc=nullptr;
         }
//    ShapeIndices=pIn.ShapeIndices;
//    ZANormVisu=pIn.ZANormVisu;

//    GLResources->registerObject(this);
}

void ZObject::_cloneFrom(ZObject&&pIn)
{
    Name=pIn.Name;
    Type=pIn.Type;

    for (int wi=0;wi<pIn.vertices.size();wi++)
        vertices.push_back(pIn.vertices[wi]);
    for (int wi=0;wi<pIn.VName.size();wi++)
        VName.push_back(pIn.VName[wi]);
    for (int wi=0;wi<pIn.Indices.size();wi++)
        Indices.push_back(pIn.Indices[wi]);

    for (int wi=0;wi<pIn.VNormalDir.size();wi++)
        VNormalDir.push_back(pIn.VNormalDir[wi]);
    for (int wi=0;wi<pIn.ShapeIndices.size();wi++)
        ShapeIndices.push_back(pIn.ShapeIndices[wi]);
    for (int wi=0;wi<pIn.ZANormVisu.size();wi++)
        ZANormVisu.push_back(pIn.ZANormVisu[wi]);
    if (pIn.GLDescriptor)
            GLDescriptor=new ZGLObjDescriptor(pIn.GLDescriptor);
    else {
            GLDescriptor=nullptr;
         }
    if (pIn.GLNormVisuDesc)
            GLNormVisuDesc=new ZGLObjDescriptor(pIn.GLNormVisuDesc);
    else {
            GLNormVisuDesc=nullptr;
         }
    if (pIn.GLShapeDesc)
            GLShapeDesc=new ZGLObjDescriptor(pIn.GLShapeDesc);
    else {
            GLShapeDesc=nullptr;
        }
//    ShapeIndices=pIn.ShapeIndices;
//    ZANormVisu=pIn.ZANormVisu;

//    GLResources->registerObject(this);
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
 /*   if (!isMinMaxInit)
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
            YMax= pInput.y;*/
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

    for (long wi=0;wi<vertices.size();wi++)
    {
        vertices[wi].textcoords.x = (vertices[wi].point.x - xmin) / wXDividor;
        vertices[wi].textcoords.y = (vertices[wi].point.y - ymin) / wYDividor;
    }
    return;
}//computeTexCoords



void
ZObject::_setupGL_ObjectArray(ZShader *pShader,  uint8_t pShaderSetupOpt )
        //unsigned int pShaderVerticePosition,
        //                        int pShaderNormalPosition)
{
    pShader->use();

/* GL buffers setup */

    glGenVertexArrays(1, &GLDescriptor->VAO);
    glGenBuffers(1, &GLDescriptor->VBO);

    glBindVertexArray(GLDescriptor->VAO);
    glBindBuffer(GL_ARRAY_BUFFER, GLDescriptor->VBO);

//    glBufferData(GL_ARRAY_BUFFER, ZAObjectVertices.usedSize(), ZAObjectVertices.data(), GL_STATIC_DRAW);
    glBufferData(GL_ARRAY_BUFFER, vertices.usedSize(), vertices.data(), GL_STATIC_DRAW);

/* point position */

   glVertexAttribPointer(GLDescriptor->getPositionAttribute(pShader), 3, GL_FLOAT, GL_FALSE, (GLsizei)getStride(), (void*)verticeOffset);
   glEnableVertexAttribArray(GLDescriptor->PositionAttribArray);

/* normals */
    if (pShaderSetupOpt&setupNormals)
        {
        if (havetoComputeNormals())
                    this->computeNormals();

/* debug */
        zbs::ZArray<Vertice_type> wNormals=toFlatNormals();
        glGenBuffers(1, &GLDescriptor->NormVBO);
        glBindBuffer(GL_ARRAY_BUFFER, GLDescriptor->NormVBO);
        glBufferData(GL_ARRAY_BUFFER, wNormals.usedSize(), wNormals.data(), GL_STATIC_DRAW);
        glVertexAttribPointer(GLDescriptor->getNormalAttribute(pShader), 3, GL_FLOAT, GL_FALSE, (GLsizei)sizeof(Vertice_type), (void*)0);
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
        /* debug */
            zbs::ZArray<TextCoords_type> wTexCoords=toFlatTexCoords();
            glGenBuffers(1, &GLDescriptor->TexVBO);
            glBindBuffer(GL_ARRAY_BUFFER, GLDescriptor->TexVBO);
            glBufferData(GL_ARRAY_BUFFER, wTexCoords.usedSize(), wTexCoords.data(), GL_STATIC_DRAW);
            glVertexAttribPointer(GLDescriptor->getTexCoordsAttribute(pShader), 3, GL_FLOAT, GL_FALSE, (GLsizei)sizeof(TextCoords_type), (void*)0);
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

    pShader->release();
} // _setupGL_ObjectArray

void
ZObject::_setupGL_ObjectElement(ZShader* pShader,  uint8_t pShaderSetupOpt )
{
  // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s)
    pShader->use();

    glGenVertexArrays(1, &GLDescriptor->VAO);
    glGenBuffers(1, &GLDescriptor->VBO);
    glGenBuffers(1, &GLDescriptor->EBO);

// bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
    glBindVertexArray(GLDescriptor->VAO);

    glBindBuffer(GL_ARRAY_BUFFER, GLDescriptor->VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.usedSize(), vertices.data(), GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, GLDescriptor->EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, Indices.usedSize(),Indices.data(), GL_STATIC_DRAW); // same indices as vertices

    // remember: do NOT unbind the EBO while a VAO is active as the
/* point position */

    glVertexAttribPointer(GLDescriptor->getPositionAttribute(pShader), 3, GL_FLOAT, GL_FALSE, (GLsizei)getStride(), (void*)verticeOffset);
    glEnableVertexAttribArray(GLDescriptor->PositionAttribArray);


/* normals */
    if (pShaderSetupOpt&setupNormals)
            {
            if (havetoComputeNormals())
                    this->computeNormals();

            glVertexAttribPointer(GLDescriptor->getNormalAttribute(pShader), 3, GL_FLOAT, GL_FALSE, (GLsizei)getStride(), (void*)normalOffset);
            glEnableVertexAttribArray(GLDescriptor->NormalAttribArray);
            }
/* texture coordinates */
    if (pShaderSetupOpt&setupTextures)
            {
            if (havetoComputeNormals())
                        this->computeTexCoords();

            glVertexAttribPointer(GLDescriptor->getTexCoordsAttribute(pShader), 3, GL_FLOAT, GL_FALSE, (GLsizei)getStride(), (void*)textureOffset);
            glEnableVertexAttribArray(GLDescriptor->TexCoordsAttribArray);
            }

    //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    // You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
    // VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
        // unbind
    // note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
        glBindBuffer(GL_ARRAY_BUFFER, 0);

    // unbind
        glBindVertexArray(0);
        pShader->release();
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

//    Shader=pShader;
//    setDefaultAlpha(pAlpha);
//    setDefaultColor(pColor);
    setDrawFigure(pMode);

    if (pTexture!=nullptr)
            setTexture(pTexture);

    if (hasIndices())
        _setupGL_ObjectElement(pShader,pShaderSetupOpt);
    else
        _setupGL_ObjectArray(pShader,pShaderSetupOpt);
    return;
}//setupGL

void
ZObject::drawGL(ZShader* pShader,unsigned int pDrawFigure)
{
    if (pShader==nullptr)
        {
        fprintf (stderr,"ZObject::drawGL-F-MissShader   Shader definition is missing for ZObject <%s>\n",Name);
        abort();
        }
    pShader->use();
/*
    pShader->setVec3("InColor",DefaultColor);
    pShader->setFloat("InAlpha",DefaultAlpha);
*/
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
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, GLDescriptor->EBO);
        glBindVertexArray(GLDescriptor->VAO);

        glDrawElements(pDrawFigure, Indices.count(), GL_UNSIGNED_INT, 0);
    }
    else
    {
        glBindVertexArray(GLDescriptor->VAO);
        glDrawArrays(pDrawFigure, 0 , vertices.count());
    }
    return;
} // drawGL

void
ZObject::setupGLNormalVisu(ZShader* pShader)
{
    if (GLNormVisuDesc!=nullptr)
            delete GLNormVisuDesc;
    GLNormVisuDesc=new ZGLObjDescriptor;
    pShader->use();

    generateNormVisu();


    glGenVertexArrays(1, &GLNormVisuDesc->VAO);
    glGenBuffers(1, &GLNormVisuDesc->VBO);

// bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
    glBindVertexArray(GLNormVisuDesc->VAO);

    glBindBuffer(GL_ARRAY_BUFFER, GLNormVisuDesc->VBO);
    glBufferData(GL_ARRAY_BUFFER, ZANormVisu.usedSize(), ZANormVisu.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(GLNormVisuDesc->getPositionAttribute(pShader), 3, GL_FLOAT, GL_FALSE, (GLsizei)sizeof(Vertice_type), (void*)verticeOffset);
    glEnableVertexAttribArray(GLNormVisuDesc->PositionAttribArray);

    glBindBuffer(GL_ARRAY_BUFFER, 0);

// unbind
    glBindVertexArray(0);
    pShader->release();
}

void
ZObject::drawGLNormalVisu(ZShader* pShader)
{
    if (pShader==nullptr)
        {
        fprintf (stderr,"ZObject::drawGL-F-MissShader   Shader definition is missing for ZObject <%s>\n",Name);
        abort();
        }
    pShader->use();

    glBindBuffer(GL_ARRAY_BUFFER, GLNormVisuDesc->VBO);
    glBindVertexArray(GLNormVisuDesc->VAO);
    glDrawArrays(GL_LINES, 0 , ZANormVisu.count());

    return;
} // drawGLNormalVisu

void
ZObject::setupGLShape(ZShader* pShader)
{
    if (GLShapeDesc!=nullptr)
            delete GLShapeDesc;
    GLShapeDesc=new ZGLObjDescriptor;
// bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s)
  pShader->use();

    glGenVertexArrays(1, &GLShapeDesc->VAO);
    glGenBuffers(1, &GLShapeDesc->VBO);
    glGenBuffers(1, &GLShapeDesc->EBO);

// bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
    glBindVertexArray(GLShapeDesc->VAO);

    glBindBuffer(GL_ARRAY_BUFFER, GLShapeDesc->VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.usedSize(), vertices.data(), GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, GLShapeDesc->EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, ShapeIndices.usedSize(),ShapeIndices.data(), GL_STATIC_DRAW); // same indices as vertices

  // remember: do NOT unbind the EBO while a VAO is active as the
/* point position */

    glVertexAttribPointer(GLShapeDesc->getPositionAttribute(pShader), 3, GL_FLOAT, GL_FALSE, (GLsizei)getStride(), (void*)verticeOffset);
    glEnableVertexAttribArray(GLShapeDesc->PositionAttribArray);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

// unbind
    glBindVertexArray(0);
    pShader->release();
}//setupGLShape
void
ZObject::drawGLShape(ZShader* pShader)
{
    if (pShader==nullptr)
        {
        fprintf (stderr,"ZObject::drawGL-F-MissShader   Shader definition is missing for ZObject <%s>\n",Name);
        abort();
        }
    pShader->use();
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, GLShapeDesc->EBO);
    glBindVertexArray(GLShapeDesc->VAO);

    glDrawElements(GL_LINE_STRIP, ShapeIndices.count(), GL_UNSIGNED_INT, 0);

    return;
} // drawGLNormalVisu

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
            wFlat.push(vertices[Indices[wi]].normal);
        }
        return wFlat;
    }

    for (size_t wi=0;wi<vertices.size();wi++)
    {
        wFlat.push(vertices[wi].normal);
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
    long wVCount=vertices.size();
    if (vertices.size()<pLimit)
                wVCount=pLimit;

    long wICount=Indices.size();
    if (Indices.size()<pLimit)
                wICount=pLimit;
    long wNVCount=ZANormVisu.size();
    if (ZANormVisu.size()<pLimit)
                wICount=pLimit;
    char wLimitChar[20];
    if (pLimit<0)
        strcpy(wLimitChar,"All data");
    else
        sprintf(wLimitChar,"First %d",pLimit);

    fprintf (pOutput,
             "\nObject <%s>\n",
             Name==nullptr?"No Name":Name);
        fprintf (pOutput,"Vertices count <%ld> displayed <%ld> <%s>\n"
                         "-------------------------------------------------\n",
                 vertices.count(),
                 wVCount,
                 wLimitChar);
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

        fprintf (pOutput,"Indices count <%ld> displayed <%ld> <%s>\n"
                         "-------------------------------------------------------\n",
                 Indices.count(),
                 wICount,
                 wLimitChar);
        if (Indices.count()==0)
            fprintf (pOutput," No indices\n");
        else
        for (long wi=0; wi < wICount;wi+=3)
                fprintf (pOutput,"%u %u %u\n",Indices[wi],Indices[wi+1],Indices[wi+2]);


        fprintf (pOutput,"Normal Vectors count <%ld> displayed <%ld> <%s>\n"
                         "------------------------------------------------------------\n",
                 ZANormVisu.count(),
                 wNVCount,
                 wLimitChar);
        if (ZANormVisu.count()==0)
            fprintf (pOutput," No Normal vectors\n");
        else {
            fprintf (pOutput,
                     "                  Center                      Target\n"
                     "     ---------------------------------------------------------------------------------\n");
            for (long wi=0; wi < wNVCount;wi+=2)
                {
                const char* wName;
                if (wi<VName.size())
                    wName=VName[wi];
                else {
                    wName="NoName";
                    }
                fprintf (pOutput,"   x:% 3f y:% 3f z:% 3f  x:% 3f y:% 3f z:% 3f\n",
                        ZANormVisu[wi].x,
                        ZANormVisu[wi].y,
                        ZANormVisu[wi].z,
                        ZANormVisu[wi+1].x,
                        ZANormVisu[wi+1].y,
                        ZANormVisu[wi+1].z);

                    }// for
            }//else

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

           wNormal= glm::normalize(glm::cross(wTriangle[2] - wTriangle[0], wTriangle[1] - wTriangle[0]));
     //      wNormal=CalculateSurfaceNormal(wTriangle);
           } // switch
       vertices[wi].normal=wNormal;
       vertices[wi+1].normal=wNormal;
       vertices[wi+2].normal=wNormal;

       wNormIdx++;
       }// for loop

    return ;
}//ZObject::generateNormals()



void ZObject::generateNormVisu()
{

    ZANormVisu.clear();


 //   Vertice_type wTriangle[3];
    Vertice_type wCenter;
    Vertice_type wTarget;
    for (long wi=0;wi<vertices.count();wi+=3)
        {
        wCenter=calculateCenter(vertices[wi].point,
                                vertices[wi+1].point,
                                vertices[wi+2].point);

        ZANormVisu.push_back(wCenter);
        wTarget = vertices[wi].normal*NormalVisuSize;
        wTarget = wTarget+ wCenter;
        ZANormVisu.push_back(wTarget);
        }
    return;
}

