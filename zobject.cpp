#ifdef __USE_GLAD__
#include <glad/glad.h>
#else
#include <GL/glew.h>
#endif

#include <zobject.h>

#include <zobjectfunctions.h>
#include <zboxcomponents.h>

#include <zmetaobject.h>
#include <zsphere.h>

#include <ztexture.h>

ZObject::ZObject(const char* pName,ObjType pType,zbs::ZArray<ZVertice>* pVertices)
{
   _create(pName,pType);
    GLDesc[Draw] = new ZGLObjDescriptor(pVertices,nullptr);
}

//Vertice_type CalculateSurfaceNormal (Vertice_type* pTriangle,ZObject::NormalDirection pFront);

ZObject::ZObject(const char *pName, ObjType pType)
{

//    isMinMaxInit=false;
    _create(pName,pType);
//    createVertexAndIndex();
//    GLDesc[Draw]=new ZGLObjDescriptor; /* created instantly */

}

void ZObject::_create(const char *pName, ObjType pType)
{
    Father.Void=nullptr;

    for (int wi=0;wi<MaxShaderContext;wi++)
        {
        GLDesc[wi]=nullptr;
        ShaderContext[wi]=nullptr;
        DrawFigure[wi]=0;
        }
    Name=pName;
    Type=pType;
    GLResources->registerObject(this);
    return;
}

void ZObject::_cloneFrom(ZObject&&pIn)
{
    Name=pIn.Name;
    Type=pIn.Type;

    for (int wi=0;wi<MaxShaderContext;wi++)
        {
        if (GLDesc[wi])
                delete GLDesc[wi];
        GLDesc[wi]=nullptr;
        if (pIn.GLDesc[wi])
                {
                GLDesc[wi]=new ZGLObjDescriptor(pIn.GLDesc[wi]);
                if (pIn.GLDesc[wi]->VertexData)
                {
                for (long wi=0;wi<pIn.GLDesc[wi]->VertexData->count();wi++)
                        GLDesc[wi]->VertexData[wi] = pIn.GLDesc[wi]->VertexData[wi];
                }
                if (pIn.GLDesc[wi]->Indexes)
                {
                for (long wi=0;wi<pIn.GLDesc[wi]->Indexes->count();wi++)
                        GLDesc[wi]->Indexes[wi] = pIn.GLDesc[wi]->Indexes[wi];
                }
                if (pIn.GLDesc[wi]->VName)
                {
                for (long wi=0;wi<pIn.GLDesc[wi]->VName->count();wi++)
                        GLDesc[wi]->VName[wi] = pIn.GLDesc[wi]->VName[wi];
                }
                if (pIn.GLDesc[wi]->VNormalDir)
                {
                for (long wi=0;wi<pIn.GLDesc[wi]->VNormalDir->count();wi++)
                        GLDesc[wi]->VNormalDir[wi] = pIn.GLDesc[wi]->VNormalDir[wi];
                }
                }
        }// for

    for (int wi=0;wi<MaxShaderContext;wi++)
        {
        if (ShaderContext[wi])
                delete ShaderContext[wi];
        ShaderContext[wi]=nullptr;
        if (pIn.ShaderContext[wi])
                    ShaderContext[wi]=new ZShaderContext(*pIn.ShaderContext[wi]);
        }// for

    if (MatCtx)
        delete MatCtx;
    if (pIn.MatCtx)
            MatCtx=new ZMatCtx(*pIn.MatCtx);

    NormVisuHeight=pIn.NormVisuHeight;
    BoxComponents=pIn.BoxComponents;

}

ZObject::~ZObject( )
{
    GLResources->deregisterObject(this);
    for (int wi=0;wi<MaxShaderContext;wi++)
        {
        if (GLDesc[wi])
                    delete GLDesc[wi];
        if (ShaderContext[wi])
                    delete ShaderContext[wi];
        }

    if (MatCtx)
            delete MatCtx;
    if (BoxComponents)
            delete BoxComponents;
}

void
ZObject::clearArrays()
{
for (int wi=0;wi<MaxShaderContext;wi++)
if (GLDesc[wi])
    {
    if (GLDesc[wi]->VertexData)
            GLDesc[wi]->VertexData->clear();
    if (GLDesc[wi]->Indexes)
           GLDesc[wi]->Indexes->clear();
    }
}

void ZObject::setBoxComponents(ZBoxComponents& pBoxComponents)
{
    BoxComponents=new ZBoxComponents(pBoxComponents);
}

TextZone ZObject::getTextZone()
{
    TextZone wTZ;

    wTZ.Width = BoxComponents->FTR.x - BoxComponents->FTL.x;
    wTZ.Height = BoxComponents->FTR.y - BoxComponents->FLR.y;

    wTZ.Center= (BoxComponents->FLMid + BoxComponents->BRMid) / 2.0f;
    wTZ.ToTopLeft = BoxComponents->FTL - wTZ.Center;



    return wTZ;
}

ZObject&
ZObject::addObject(ZObject &pIn)
{
    for (int wj=0;wj<MaxShaderContext;wj++)
    {
    if (pIn.GLDesc[wj])
    {
    if (!GLDesc[wj])
        {
        GLDesc[wj]=new ZGLObjDescriptor(pIn.GLDesc[wj]);
        continue;
        }
    if (pIn.GLDesc[wj]->VertexData)
    {
    if (!GLDesc[wj]->VertexData)
            GLDesc[wj]->VertexData=new zbs::ZArray<ZVertice>;
    for (long wi=0;wi<pIn.GLDesc[wj]->VertexData->count();wi++)
        {
        GLDesc[wj]->VertexData->push(pIn.GLDesc[wj]->VertexData->Tab[wi]);
        }
    }
    if (pIn.GLDesc[wj]->Indexes)
    {
    if (!GLDesc[wj]->Indexes)
            GLDesc[wj]->Indexes=new zbs::ZArray<unsigned int>;
    for (long wi=0;wi<pIn.GLDesc[wj]->Indexes->count();wi++)
        {
        GLDesc[wj]->Indexes->push(pIn.GLDesc[wj]->Indexes->Tab[wi]);
        }
    }
    if (pIn.GLDesc[wj]->VName)
    {
    if (!GLDesc[wj]->VName)
            GLDesc[wj]->VName=new zbs::ZArray<const char*>;
    for (long wi=0;wi<pIn.GLDesc[wj]->VName->count();wi++)
        {
        GLDesc[wj]->VName->push(pIn.GLDesc[wj]->VName->Tab[wi]);
        }
    }
    }//if (pIn.GLDesc[wj])
    }// for wj

    return *this;
}
long
ZObject::addVec3(const DrawContext_type pCtx,Vertice_type pInput)
    {
    if (!GLDesc[pCtx])
                return -1;
    if (!GLDesc[pCtx]->VertexData)
            GLDesc[pCtx]->VertexData=new zbs::ZArray<ZVertice>;
    return GLDesc[pCtx]->VertexData->push(ZVertice(pInput));
    }
void
ZObject::addVec3(const DrawContext_type pCtx,Vertice_type pInput, const char* pName )
{
    if (!GLDesc[pCtx]->VertexData)
        {
       GLDesc[pCtx]->VertexData=new zbs::ZArray<ZVertice>;
        }
    if (!GLDesc[pCtx]->VName)
        {
       GLDesc[pCtx]->VName=new zbs::ZArray<const char*>;
        }
    ZVertice wVertice(pInput);
    GLDesc[pCtx]->VertexData->push(wVertice);
    GLDesc[pCtx]->VName->push(pName);

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
ZObject::addVerticeArray(const DrawContext_type pCtx,zbs::ZArray<glm::vec3>*pIn)
{
    if (!GLDesc[pCtx]->VertexData)
        {
       GLDesc[pCtx]->VertexData=new zbs::ZArray<ZVertice>;
        }

    for (long wi=0;wi<pIn->count();wi++)
            GLDesc[pCtx]->VertexData->push(ZVertice(pIn->Tab[wi]));
}

void
ZObject::computeTexCoords(zbs::ZArray<ZVertice>*vertices)
{
    float xmin=vertices->Tab[0].point.x;
    float xmax=vertices->Tab[0].point.x;
    float ymin=vertices->Tab[0].point.y;
    float ymax=vertices->Tab[0].point.y;
    for (long wi=1;wi<vertices->size();wi++)
    {
        if(vertices->Tab[wi].point.x > xmax) xmax = vertices->Tab[wi].point.x ;
        if(vertices->Tab[wi].point.x < xmin) xmin = vertices->Tab[wi].point.x ;
        if(vertices->Tab[wi].point.y > ymax) ymax = vertices->Tab[wi].point.y ;
        if(vertices->Tab[wi].point.y < ymin) ymin = vertices->Tab[wi].point.y ;
    }
    float wXDividor = xmax-xmin;
    float wYDividor = ymax-ymin;

    for (long wi=0;wi<vertices->size();wi++)
    {
        vertices->Tab[wi].texcoords.x = (vertices->Tab[wi].point.x - xmin) / wXDividor;
        vertices->Tab[wi].texcoords.y = (vertices->Tab[wi].point.y - ymin) / wYDividor;
    }
    return;
}//computeTexCoords


//----------Position and Rotation---------------------------

void ZObject::setPosition(Vertice_type pPosition)
{    
    MatCtx->setPosition(pPosition);
}

void ZObject::setModelRotation (float pAngle,glm::vec3 pAxis)
{
    if (!MatCtx)
    {
       fprintf (stderr,"ZObject::setModelRotation-F-MATNULL Object <%s> Matrice context is null. Cannot rotate model.\n",
                Name);
    }
    MatCtx->setRotationAxis(pAxis);
    MatCtx->RotationAngle=pAngle;
}

void ZObject::rotate90 ()
{
    if (!MatCtx)
    {
       fprintf (stderr,"ZObject::rotate90-F-MATNULL Object <%s> Matrice context is null. Cannot rotate model.\n",
                Name);
    }
    MatCtx->setRotationAxis(glm::vec3(0.0,0.0,1.0));
    MatCtx->RotationAngle=glm::radians(90.0f);

}

void ZObject::rotate270 ()
{
    if (!MatCtx)
    {
       fprintf (stderr,"ZObject::rotate90-F-MATNULL Object <%s> Matrice context is null. Cannot rotate model.\n",
                Name);
    }
    MatCtx->setRotationAxis(glm::vec3(0.0,0.0,1.0));
    MatCtx->RotationAngle=glm::radians(270.0f);

}

//-------------Prepare (setup) GL data vertexes, indices and normals ---------

void
ZObject::_setupGL_ObjectArray(const DrawContext_type pCtx, uint16_t pAction)
{
    int wVDataCtx=pCtx;
    ZShader* wShader=GLResources->getActiveShaderPtr();
    if (wShader==nullptr)
            {
            fprintf(stderr,"ZObject::drawGL-E-NOSHD Object <%s> No active shader while setting up GL environment.No action made.\n",
                    Name);
            return;
            }

    if (!GLDesc[pCtx])
            GLDesc[pCtx]=new ZGLObjDescriptor;

     /* Normal coordinates may be computed but not set : we could need them for computing shape coordinate */
    if (pAction & CSO_ComputeNormals)
//        this->computeNormals(GLDesc[wVDataCtx]->VertexData,GLDesc[wVDataCtx]->VNormalDir);
        this->computeNormals(GLDesc[wVDataCtx]->VertexData,nullptr);
    if (pAction & CSO_ComputeTextures)
                this->computeTexCoords(GLDesc[wVDataCtx]->VertexData);


/* GL buffers setup */
    glGenVertexArrays(1, &GLDesc[pCtx]->VAO);
    glGenBuffers(1, &GLDesc[pCtx]->VBO);

    glBindVertexArray(GLDesc[pCtx]->VAO);
    glBindBuffer(GL_ARRAY_BUFFER, GLDesc[pCtx]->VBO);

//    zbs::ZArray<ZVertice>* wVertexData=GLDesc[pCtx]->VertexData;
    if (!GLDesc[pCtx]->VertexData)
        {
        if (!GLDesc[Draw]->VertexData)
            {
            fprintf (stderr,"ZObject::_setupGL_ObjectArray-F-NOGLDESC No valid GL descriptor found for object <%s> trying to setup GL for context <%s>.\n",
                     Name,
                     decodeShdCtx(pCtx));
            exit (EXIT_FAILURE);
            }
        wVDataCtx=Draw;
        }
    glBufferData(GL_ARRAY_BUFFER,
                 GLDesc[wVDataCtx]->VertexData->usedSize(),
                 GLDesc[wVDataCtx]->VertexData->data(),
                 GL_STATIC_DRAW);

/* point position */
    int wPos=wShader->getNamedAttributeLocation(__SHD_POSITION_ATTR__,false);
    if (wPos<0)
            wPos=cst_defaultPositionLocation;
    glVertexAttribPointer(wPos,
                          3,
                          GL_FLOAT,
                          GL_FALSE,
                          (GLsizei)getStride(),
                          (void*)verticeOffset);
    glEnableVertexAttribArray(wPos);

/* normals */



    if (pAction & CSO_setupNormals)
        {
//        glBindBuffer(GL_ARRAY_BUFFER, GLDesc[pCtx]->VBO);
        int wNorm=wShader->getNamedAttributeLocation(__SHD_NORMAL_ATTR__,false);
        if (wNorm<0)
                wNorm=cst_defaultNormalLocation;
        glVertexAttribPointer(wNorm,
                              3,
                              GL_FLOAT,
                              GL_FALSE,
                              (GLsizei)getStride(),
                              (void*)normalOffset);
        glEnableVertexAttribArray(wNorm);
/* end debug */
//        glVertexAttribPointer(aNormal, 3, GL_FLOAT, GL_FALSE, (GLsizei)getStride(), (void*)normalOffset);
//        glEnableVertexAttribArray(aNormal);
        }

/* texture coordinates */

    if (pAction & CSO_setupTextures)
        {
        glBindBuffer(GL_ARRAY_BUFFER, GLDesc[pCtx]->VBO);
        int wTex=wShader->getNamedAttributeLocation(__SHD_TEXCOORDS_ATTR__,false);
        if (wTex<0)
                wTex=cst_defaultTexCoordsLocation;
        glVertexAttribPointer(wTex,
                              2,
                              GL_FLOAT,
                              GL_FALSE,
                              (GLsizei)getStride(),
                              (void*)textureOffset);
        glEnableVertexAttribArray(wTex);
        }

// You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
// VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
    // unbind
// note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
    glBindBuffer(GL_ARRAY_BUFFER, 0);

// unbind
    glBindVertexArray(0);

    wShader->release();
    delete wShader;
} // _setupGL_ObjectArray
#ifdef __COMMENT__
void
ZObject::_setupGL_ObjectArray_old(const DrawContext_type pCtx )
{
    ZShader* wShader=GLResources->getActiveShaderPtr();
    if (wShader==nullptr)
            {
            fprintf(stderr,"ZObject::drawGL-E-NOSHD Object <%s> No active shader while setting up GL environment.No action made.\n",
                    Name);
            return;
            }

    if (!GLDesc[Draw])
            GLDesc[pCtx]=new ZGLObjDescriptor;


/* GL buffers setup */
    glGenVertexArrays(1, &GLDesc[pCtx]->VAO);
    glGenBuffers(1, &GLDesc[pCtx]->VBO);

    glBindVertexArray(GLDesc[pCtx]->VAO);
    glBindBuffer(GL_ARRAY_BUFFER, GLDesc[pCtx]->VBO);

    glBufferData(GL_ARRAY_BUFFER, GLDesc[pCtx]->VertexData->usedSize(), GLDesc[pCtx]->VertexData->data(), GL_STATIC_DRAW);

/* point position */
    int wPos=wShader->getNamedAttributeLocation(__POSITION_ATR_NAME__,false);
    if (wPos<0)
            wPos=cst_defaultPositionLocation;
    glVertexAttribPointer(wPos, 3, GL_FLOAT, GL_FALSE, (GLsizei)getStride(), (void*)verticeOffset);
    glEnableVertexAttribArray(wPos);

/* normals */
    if (GLDesc[pCtx]->Actions & CSO_setupNormals)
        {
        if (havetoComputeNormals(pCtx))
                    this->computeNormals(GLDesc[pCtx]->VertexData,GLDesc[pCtx]->VNormalDir);

/* debug */
        zbs::ZArray<Vertice_type>* wNormals=toFlatNormals(GLDesc[pCtx]->VertexData,GLDesc[pCtx]->Indexes);
        glGenBuffers(1, &GLDesc[pCtx]->NormVBO);
        glBindBuffer(GL_ARRAY_BUFFER, GLDesc[pCtx]->NormVBO);
        glBufferData(GL_ARRAY_BUFFER, wNormals->usedSize(), wNormals->data(), GL_STATIC_DRAW);
        int wNorm=wShader->getNamedAttributeLocation(__NORMAL_ATR_NAME__,false);
        if (wNorm<0)
                wNorm=cst_defaultNormalLocation;
        glVertexAttribPointer(wNorm, 3, GL_FLOAT, GL_FALSE, (GLsizei)sizeof(Vertice_type), (void*)0);
        glEnableVertexAttribArray(wNorm);
        delete wNormals;
/* end debug */
//        glVertexAttribPointer(aNormal, 3, GL_FLOAT, GL_FALSE, (GLsizei)getStride(), (void*)normalOffset);
//        glEnableVertexAttribArray(aNormal);
        }

/* texture coordinates */

    if (GLDesc[pCtx]->Actions & CSO_setupTextures)
        {
        if (havetoComputeTexCoords())
                    this->computeTexCoords(GLDesc[pCtx]->VertexData);
        /* debug */
            zbs::ZArray<TexCoords_type>* wTexCoords=toFlatTexCoords(GLDesc[pCtx]->VertexData,GLDesc[pCtx]->Indexes);
            glGenBuffers(1, &GLDesc[pCtx]->TexVBO);
            glBindBuffer(GL_ARRAY_BUFFER, GLDesc[pCtx]->TexVBO);
            glBufferData(GL_ARRAY_BUFFER, wTexCoords->usedSize(), wTexCoords->data(), GL_STATIC_DRAW);
            int wTex=wShader->getNamedAttributeLocation(__TEXCOORDS_ATR_NAME__,false);
            if (wTex<0)
                    wTex=cst_defaultTexCoordsLocation;
            glVertexAttribPointer(wTex, 3, GL_FLOAT, GL_FALSE, (GLsizei)sizeof(TexCoords_type), (void*)0);
            glEnableVertexAttribArray(wTex);
            delete wTexCoords;
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

    wShader->release();
    delete wShader;
} // _setupGL_ObjectArray
#endif // __COMMENT__

void
ZObject::_setupGL_ObjectElement(const DrawContext_type pCtx,uint16_t pAction)
{
    ZShader* wShader=GLResources->getActiveShaderPtr();
    if (wShader==nullptr)
            {
            fprintf(stderr,"ZObject::_setupGL_ObjectElement-E-NOSHD No active shader while setting up GL environment for context <%s>.No action made.\n",
                    decodeShdCtx(pCtx));
            return;
            }

    zbs::ZArray<ZVertice>* wVertexData=GLDesc[pCtx]->VertexData;
    if (!wVertexData)
            {
            fprintf (stderr,"ZObject::_setupGL_ObjectElement-W-VRTXNULL For context <%s> VertexData is null. Replacing it with one from Draw context.\n",
                     decodeShdCtx(pCtx));
            wVertexData=GLDesc[Draw]->VertexData;
            if (!wVertexData)
                {
                fprintf (stderr,"ZObject::_setupGL_ObjectElement-F-VRTXNULL For context <%s> VertexData is null and has no replacement from Draw context.\n",
                         decodeShdCtx(pCtx));
                exit (EXIT_FAILURE);
                }
            }

    /* Normal coordinates may be computed but not set : we could need them for computing shape coordinate */
    if (pAction & CSO_ComputeNormals)
            {
            this->computeNormals(GLDesc[pCtx]->VertexData,GLDesc[pCtx]->VNormalDir);
            GLDesc[pCtx]->NormalComputed=true;
            }
    if (pAction & CSO_ComputeTextures)
                this->computeTexCoords(wVertexData);


/* GL buffers setup */
    // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s)

    glGenVertexArrays(1, &GLDesc[pCtx]->VAO);
    glGenBuffers(1, &GLDesc[pCtx]->VBO);
    glGenBuffers(1, &GLDesc[pCtx]->EBO);

    glBindVertexArray(GLDesc[pCtx]->VAO);
    glBindBuffer(GL_ARRAY_BUFFER, GLDesc[pCtx]->VBO);
//    glBufferData(GL_ARRAY_BUFFER, GLDesc[pCtx]->VertexData->usedSize(), GLDesc[Draw]->VertexData->data(), GL_STATIC_DRAW);
    glBufferData(GL_ARRAY_BUFFER,
                 wVertexData->usedSize(),
                 wVertexData->data(),
                 GL_STATIC_DRAW);



// bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
/*    glBindVertexArray(GLDescriptor->VAO);

    glBindBuffer(GL_ARRAY_BUFFER, GLDescriptor->VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.usedSize(), vertices.data(), GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, GLDescriptor->EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, Indices.usedSize(),Indices.data(), GL_STATIC_DRAW); // same indices as vertices
*/
    // remember: do NOT unbind the EBO while a VAO is active as the
/* point position */
    int wPos=wShader->getNamedAttributeLocation(__SHD_POSITION_ATTR__,false);
    if (wPos<0)
            wPos=cst_defaultPositionLocation;
    glVertexAttribPointer(wPos,
                          3,
                          GL_FLOAT,
                          GL_FALSE,
                          (GLsizei)getStride(),
                          (void*)verticeOffset);
    glEnableVertexAttribArray(wPos);


/* normals */


    if (pAction & CSO_setupNormals)
            {
            int wNorm=wShader->getNamedAttributeLocation(__SHD_NORMAL_ATTR__,false);
            if (wNorm<0)
                    wNorm=cst_defaultNormalLocation;
            glVertexAttribPointer(wNorm,
                                  3,
                                  GL_FLOAT,
                                  GL_FALSE,
                                  (GLsizei)getStride(),
                                  (void*)normalOffset);
            glEnableVertexAttribArray(wNorm);
            }
/* texture coordinates */
    if (pAction & CSO_setupTextures)
            {
            int wTex=wShader->getNamedAttributeLocation(__SHD_TEXCOORDS_ATTR__,false);
            if (wTex<0)
                    wTex=cst_defaultTexCoordsLocation;
            glVertexAttribPointer(wTex, 3, GL_FLOAT, GL_FALSE, (GLsizei)getStride(), (void*)textureOffset);
            glEnableVertexAttribArray(wTex);
            }

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, GLDesc[pCtx]->EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, GLDesc[pCtx]->Indexes->usedSize(),GLDesc[pCtx]->Indexes->data(), GL_STATIC_DRAW);

    //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    // You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
    // VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
        // unbind
    // note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
        glBindBuffer(GL_ARRAY_BUFFER, 0);

    // unbind
        glBindVertexArray(0);
        wShader->release();
        delete wShader;
} // _setupGL_ObjectElement

void
ZObject::setupGL(const DrawContext_type pCtx, uint16_t pAction)
{



    if (!GLDesc[pCtx])
            GLDesc[pCtx]=new ZGLObjDescriptor; /* if exists : release GL internal resources before reallocating */

    if (pCtx==NormVisu)
        {
        generateNormVisuFromCtx(NormVisuHeight,Draw);
        DrawFigure[NormVisu]=GL_LINES;
        }

    if (ShaderContext[pCtx])
        {
        ShaderContext[pCtx]->bindShader();
        }

    if (hasIndices(pCtx))
        _setupGL_ObjectElement(pCtx,pAction);
    else
        _setupGL_ObjectArray(pCtx,pAction);
    return;

}



void
ZObject::setupGLNormVisu(zbs::ZArray<ZVertice> *pVertex)
{

    ZShader* wShader=GLResources->getActiveShaderPtr();
    if (wShader==nullptr)
            {
            fprintf(stderr,"ZObject::setupGLNormVisu-E-NOSHD No active shader while setting up GL environment.No action made.\n");
            return;
            }

//    if (!GLDesc[NormVisu])
//            GLDesc[NormVisu]=new ZGLObjDescriptor;

    generateNormVisu(GLDesc[NormVisu]->VertexData,NormVisuHeight);


    glGenVertexArrays(1, &GLDesc[NormVisu]->VAO);
    glGenBuffers(1, &GLDesc[NormVisu]->VBO);

// bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
    glBindVertexArray(GLDesc[NormVisu]->VAO);

    glBindBuffer(GL_ARRAY_BUFFER, GLDesc[NormVisu]->VBO);
    glBufferData(GL_ARRAY_BUFFER, GLDesc[NormVisu]->VertexData->usedSize(), GLDesc[NormVisu]->VertexData->data(), GL_STATIC_DRAW);

    int wPos=wShader->getNamedAttributeLocation(__SHD_POSITION_ATTR__,false);
    if (wPos<0)
            wPos=cst_defaultPositionLocation;
    glVertexAttribPointer(wPos, 3, GL_FLOAT, GL_FALSE, (GLsizei)sizeof(Vertice_type), (void*)verticeOffset);
    glEnableVertexAttribArray(wPos);

    glBindBuffer(GL_ARRAY_BUFFER, 0);

// unbind
    glBindVertexArray(0);
    wShader->release();
    delete wShader;
}

void
ZObject::setupGLUserDefined(zbs::ZArray<ZVertice> *pVertex,uint16_t pAction)
{


    if (!GLDesc[UserDefined])
            GLDesc[UserDefined]=new ZGLObjDescriptor;

    ZShader* wShader=GLResources->getActiveShaderPtr();
    if (wShader==nullptr)
            {
            fprintf(stderr,"ZObject::setupGLUserDefined-E-NOSHD No active shader while setting up GL environment.No action made.\n");
            return;
            }

    glGenVertexArrays(1, &GLDesc[UserDefined]->VAO);
    glGenBuffers(1, &GLDesc[UserDefined]->VBO);

// bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
    glBindVertexArray(GLDesc[UserDefined]->VAO);

    glBindBuffer(GL_ARRAY_BUFFER, GLDesc[UserDefined]->VBO);
    glBufferData(GL_ARRAY_BUFFER, GLDesc[UserDefined]->VertexData->usedSize(), GLDesc[UserDefined]->VertexData->data(), GL_STATIC_DRAW);
    int wPos=wShader->getNamedAttributeLocation(__SHD_POSITION_ATTR__,false);
    if (wPos<0)
            wPos=cst_defaultPositionLocation;
    glVertexAttribPointer(wPos, 3, GL_FLOAT, GL_FALSE, (GLsizei)sizeof(Vertice_type), (void*)verticeOffset);
    glEnableVertexAttribArray(wPos);

    glBindBuffer(GL_ARRAY_BUFFER, 0);

// unbind
    glBindVertexArray(0);
    wShader->release();
    delete wShader;
}


void
ZObject::setupGLShape(zbs::ZArray<ZVertice>*pVertex)
{

    if (GLDesc[Shape])
            delete GLDesc[Shape];
    GLDesc[Shape]=new ZGLObjDescriptor;

    if (ShaderContext[Shape])
        {
        ShaderContext[Shape]->bindShader();
        }
    ZShader* wShader=GLResources->getActiveShaderPtr();
    if (wShader==nullptr)
            {
            fprintf(stderr,"ZObject::setupGLShape-E-NOSHD No active shader while setting up GL environment.No action made.\n");
            return;
            }
    glGenVertexArrays(1, &GLDesc[Shape]->VAO);
    glGenBuffers(1, &GLDesc[Shape]->VBO);
    glGenBuffers(1, &GLDesc[Shape]->EBO);

// bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
    glBindVertexArray(GLDesc[Shape]->VAO);

    glBindBuffer(GL_ARRAY_BUFFER, GLDesc[Shape]->VBO);
    glBufferData(GL_ARRAY_BUFFER, GLDesc[Shape]->VertexData->usedSize(), GLDesc[Shape]->VertexData->data(), GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, GLDesc[Shape]->EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, GLDesc[Shape]->VertexData->usedSize(),GLDesc[Shape]->VertexData->data(), GL_STATIC_DRAW); // same indices as vertices

  // remember: do NOT unbind the EBO while a VAO is active as the
/* point position */
    int wPos=wShader->getNamedAttributeLocation(__SHD_POSITION_ATTR__,false);
    if (wPos<0)
            wPos=cst_defaultPositionLocation;
    glVertexAttribPointer(wPos, 3, GL_FLOAT, GL_FALSE, (GLsizei)getStride(), (void*)verticeOffset);
    glEnableVertexAttribArray(wPos);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

// unbind
    glBindVertexArray(0);
    wShader->release();
}//setupGLShape



//----------------Rendering-------------------------------


void
ZObject::_preprocessGLMatrices(ZShader* wShader)
{
    if (!MatCtx)        /* No Matrix context : return without doing anything */
            return;
    MatCtx->_preprocessGL(wShader);
}//_preprocessMatrices

void ZObject::setAllMatricesValue (ZMat4* pModel,ZMat4* pView,ZMat4* pProjection, ZMat4* pNormal)
{
    if (!MatCtx)
        return;
    MatCtx->setAllMatricesValue(pModel,pView,pProjection,pNormal);
}


void ZObject::createMatrices (uint8_t pFlag)
{
    if (!MatCtx)
        MatCtx=new ZMatCtx;
    MatCtx->createMatrices(pFlag);
}

void ZObject::createAllMatrices()
{
    if (!MatCtx)
        MatCtx=new ZMatCtx;
    MatCtx->createMatrices(MAT_All);
//    MatricesSetUp=true;
}//createAllMatrices

void ZObject::createModel ()
{
    if (!MatCtx)
            MatCtx=new ZMatCtx;
    MatCtx->createModel();
}
void ZObject::createView ()
{
    if (!MatCtx)
            MatCtx=new ZMatCtx;
    MatCtx->createView();
}
void ZObject::createProjection ()
{
    if (!MatCtx)
            MatCtx=new ZMatCtx;
    MatCtx->createProjection();
}
void ZObject::createNormal ()
{
    if (!MatCtx)
            MatCtx=new ZMatCtx;
    MatCtx->createNormal();
}


void
ZObject::drawGLByContext(const DrawContext_type pCtx)
{

  if (pCtx== UserDefined)
    {
      if (drawGLUserDefined==nullptr)
      {
        fprintf(stderr,"ZObject::drawGL-E-USDFNULL drawGLUserDefined is null will requesting to drawGL user defined environment.\n");
        return;
      }
      (*drawGLUserDefined)();
      return;
    }

    drawGLGeneric(pCtx);
    return;

}


/* rendering object for context <Draw>
    Remark : Shader Context might be omitted (nullptr) in this case, shader set up should have been already done by upper instance (meta object, ZCandy, ZSphere etc.)
*/
void
ZObject::drawGLDraw()
{
/*     Apply rules from Shader context to shader only if required (not nullptr) : must be first */

    if (ShaderContext[Draw])
            ShaderContext[Draw]->applyRules(); /* if shader context exists, then Shader has to be viable */
    ZShader* wShader=GLResources->getActiveShaderPtr();
    if (wShader==nullptr)
            {
            fprintf(stderr,"ZObject::drawGL-E-NOSHD No active shader while setting up GL environment.No action made.\n");
            return;
            }

/* second : set up matrices : model rotation and translation, projection and view
    Remark: destination shader must be current (ZShader::use()
    Could be done either locally if Shader Context exists or at an upper level
*/
    _preprocessGLMatrices(wShader);

    if (ShaderContext[Draw])
        if (ShaderContext[Draw]->hasTexture())
                    ShaderContext[Draw]->bindTexture();

/* third : set up internal GL buffers */

    glBindBuffer(GL_ARRAY_BUFFER, GLDesc[Draw]->VBO);

    if (hasIndices())
    {
 //       glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, GLDescriptor->EBO);  // not necessary : VAO takes care of it
        glBindVertexArray(GLDesc[Draw]->VAO);

        glDrawElements(DrawFigure[Draw], GLDesc[Draw]->Indexes->count(), GL_UNSIGNED_INT, 0);
    }
    else
    {
        glBindVertexArray(GLDesc[Draw]->VAO);
        glDrawArrays(DrawFigure[Draw], 0 , GLDesc[Draw]->VertexData->count());
    }
    delete wShader; /* can safely delete wShader : will also unshare it */
    return;
} // drawGL

void
ZObject::drawGLShape()
{
float wLineWidth;
    glGetFloatv(GL_LINE_WIDTH,&wLineWidth); /* get current line width */
 /*   if (ShapeShader==nullptr)
        {
        fprintf (stderr,"ZObject::drawGLShape-F-MissShader   Shader definition is missing for ZObject <%s>\n",Name);
        exit (EXIT_FAILURE);
        }
*/
    if (ShaderContext[Shape])
            {
            ShaderContext[Shape]->applyRules(); // bind and apply shader context rules to shader
            }
    ZShader* wShader=GLResources->getActiveShaderPtr();
    if (wShader==nullptr)
            {
            fprintf(stderr,"ZObject::drawGL-E-NOSHD Object <%s> No active shader while setting up GL environment.No action made.\n",
                    Name);
            return;
            }
    _preprocessGLMatrices(wShader);
    glLineWidth(2.0f);
//    ShapeShader->use();
//    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, GLShapeDesc->EBO);
    glBindVertexArray(GLDesc[Shape]->VAO);

    glDrawElements(GL_LINE_LOOP, GLDesc[Shape]->Indexes->count(), GL_UNSIGNED_INT, 0);
    glLineWidth(wLineWidth);/* restore line width to its previous value */
    return;
} // drawGLNormalVisu
/* rendering object for context <Draw>
    Remark : Shader Context might be omitted (nullptr) in this case, shader set up should have been already done by upper instance (meta object, ZCandy, ZSphere etc.)
*/



void
ZObject::drawGLGeneric(const DrawContext_type pCtx)
{
/*     Apply rules from Shader context to shader only if required (not nullptr) : must be first */

    if (ShaderContext[pCtx])
            ShaderContext[pCtx]->applyRules(); /* if shader context exists, then Shader has to be viable */
    ZShader* wShader=GLResources->getActiveShaderPtr();
    if (wShader==nullptr)
            {
            fprintf(stderr,"ZObject::drawGL-E-NOSHD Object <%s> - context <%s> : No active shader while drawing object.No action made.\n",
                    decodeShdCtx(pCtx),
                    Name);
            return;
            }

    if (GLDesc[pCtx]==nullptr)
            {
            fprintf(stderr,"ZObject::drawGL-E-NOGLDESC Object <%s> - context <%s> : No active GL descriptor while drawing object.No action made.\n",
                    decodeShdCtx(pCtx),
                    Name);
            return;
            }
/* second : set up matrices : model rotation and translation, projection and view
    Remark: destination shader must be current (ZShader::use()
    Could be done either locally if Shader Context exists or at an upper level
*/
    _preprocessGLMatrices(wShader);

    if (ShaderContext[pCtx])
        if (ShaderContext[pCtx]->hasTexture())
                    ShaderContext[pCtx]->bindTexture();

/* third : bind  GL buffers (VAO) */

//    glBindBuffer(GL_ARRAY_BUFFER, GLDesc[pCtx]->VBO); // not necessary : VAO takes care of it

    if (hasIndices())
    {
//        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, GLDesc[pCtx]->EBO);  // not necessary : VAO takes care of it
        glBindVertexArray(GLDesc[pCtx]->VAO);

        glDrawElements(DrawFigure[pCtx], GLDesc[pCtx]->Indexes->count(), GL_UNSIGNED_INT, 0);
    }
    else
    {
        glBindVertexArray(GLDesc[pCtx]->VAO);
        glDrawArrays(DrawFigure[pCtx], 0 , GLDesc[pCtx]->VertexData->count());
    }

    if (ShaderContext[pCtx])
            ShaderContext[pCtx]->postProcess();

    delete wShader; /* can safely delete wShader : will also unshare it */
    return;
} // drawGLArrayGeneric


void
ZObject::drawGLNormVisu()
{
    ZShader* wShader=GLResources->getActiveShaderPtr();
    if (wShader==nullptr)
            {
            fprintf(stderr,"ZObject::drawGL-E-NOSHD Object <%s> No active shader while setting up GL environment.No action made.\n",
                    Name);
            return;
            }
    _preprocessGLMatrices(wShader);
/*    if (NormVisuShader==nullptr)
        {
        fprintf (stderr,"ZObject::drawGLNormalVisu-F-MissShader   Shader definition is missing for ZObject <%s>\n",Name);
        abort();
        }
    NormVisuShader->use();
*/
//    glBindBuffer(GL_ARRAY_BUFFER, GLDesc[NormVisu]->VBO);
    glBindVertexArray(GLDesc[NormVisu]->VAO);
    glDrawArrays(GL_LINES, 0 , GLDesc[Draw]->VertexData->count());
    delete wShader;
    return;
} // drawGLNormalVisu



zbs::ZArray<Vertice_type>*
ZObject::toRawVertices(zbs::ZArray<ZVertice> *pVertices)
{
    if (!pVertices)
        return nullptr;
    zbs::ZArray<Vertice_type>* wFlat;
    for (size_t wi=0;wi<pVertices->size();wi++)
        {
        wFlat->push_back(pVertices->Tab[wi].point);
        }
    return wFlat;
}//toRawVertices

zbs::ZArray<Vertice_type>*
ZObject::toFlatVertices(zbs::ZArray<ZVertice>* pVertices,zbs::ZArray<unsigned int>* pIndices)
{
    if (!pVertices)
        return nullptr;
    zbs::ZArray<Vertice_type>* wFlat=new zbs::ZArray<Vertice_type>;
    if (pIndices)
        {
        for (size_t wi=0;wi<pIndices->size();wi++)
        {
            wFlat->push_back(pVertices->Tab[pIndices->Tab[wi]].point);
        }
        return wFlat;
        }

    for (size_t wi=0;wi<pVertices->size();wi++)
    {
        wFlat->push_back(pVertices->Tab[wi].point);
    }
    return wFlat;
}//toFlatVertices

zbs::ZArray<Vertice_type>*
ZObject::toRawNormals(zbs::ZArray<ZVertice> *pVertices)
{
    if (!pVertices)
        return nullptr;
    zbs::ZArray<Vertice_type>* wFlat=new zbs::ZArray<Vertice_type>;
    for (size_t wi=0;wi<pVertices->size();wi++)
        {
        wFlat->push(pVertices->Tab[wi].normal);
        }
    return wFlat;
}//toRawNormals

zbs::ZArray<Vertice_type>*
ZObject::toFlatNormals(zbs::ZArray<ZVertice> *pVertices, zbs::ZArray<unsigned int> *pIndices)
{
    if (!pVertices)
        return nullptr;
    zbs::ZArray<Vertice_type>* wFlat=new zbs::ZArray<Vertice_type>;
    if (pIndices)
    {
        for (size_t wi=0;wi<pIndices->size();wi++)
        {
            wFlat->push(pVertices->Tab[pIndices->Tab[wi]].normal);
        }
        return wFlat;
    }

    for (size_t wi=0;wi<pVertices->size();wi++)
    {
        wFlat->push(pVertices->Tab[wi].normal);
    }
    return wFlat;
}//toFlatNormals


zbs::ZArray<glm::vec2> *ZObject::toFlatTexCoords(zbs::ZArray<ZVertice> *pVertices, zbs::ZArray<unsigned int> *pIndices)
{
    zbs::ZArray<TexCoords_type>* wFlat=new zbs::ZArray<glm::vec2>;
    if (pIndices)
        {
        for (size_t wi=0;wi<pIndices->size();wi++)
        {
            wFlat->push_back(pVertices->Tab[pIndices->Tab[wi]].texcoords);
        }
        return wFlat;
        }

    for (size_t wi=0;wi<pVertices->size();wi++)
        {
        wFlat->push_back(pVertices->Tab[wi].texcoords);
        }
    return wFlat;
}//toFlatTexCoords

void
ZObject::print(int pLimit,FILE* pOutput) const
{
    long wVCount=0;
    long wICount=0;
    long wNVCount=0;

    fprintf (pOutput,
             "\nObject <%s>\n",
             Name==nullptr?"No Name":Name);

    for (int wj=0;wj<MaxShaderContext;wj++)
                _printContext(wj,pLimit,pOutput);


}// print

void
ZObject::printContext(int pCtx, int pLimit,FILE* pOutput) const
{
    fprintf (pOutput,
             "\nObject <%s>\n",
             Name==nullptr?"No Name":Name);
    _printContext(pCtx,pLimit,pOutput);
    return;
}

void
ZObject::_printContext(int pCtx, int pLimit,FILE* pOutput) const
{
    long wVCount=0;
    long wICount=0;
    long wNVCount=0;

    fprintf (pOutput,
             "        Context <%s> \n",decodeShdCtx(pCtx));
    if (!GLDesc[pCtx])
         {
         fprintf (pOutput,"            no data for this context \n");
         return;
        }

    char wLimitChar[20];
    if (pLimit<0)
        strcpy(wLimitChar,"All data");
    else
        sprintf(wLimitChar,"First %d",pLimit);


        if (!GLDesc[pCtx]->VertexData)
            fprintf (pOutput,"               Vertex data is missing.\n");
        else
        {
        wVCount=GLDesc[pCtx]->VertexData->count();
        if (wVCount>pLimit)
                    wVCount=pLimit;

        fprintf (pOutput,"Vertices count <%ld> displayed <%ld> <%s>\n"
                         "-------------------------------------------------\n",
                 GLDesc[pCtx]->VertexData->count(),
                 wVCount,
                 wLimitChar);
            fprintf (pOutput,
                     "                  Vertices                            Normals                         Texture \n"
                     "     -----------------------------------------------------------------------------------------------------\n");
        for (long wi=0; wi < wVCount;wi++)
            {
            const char* wName ="NoName";
            if (GLDesc[pCtx]->VName)
                {
                if (wi < GLDesc[pCtx]->VName->count())
                    wName=GLDesc[pCtx]->VName->Tab[wi];
                }
            fprintf (pOutput,"%s x:% 3f y:% 3f z:% 3f  x:% 3f y:% 3f z:% 3f s:% 3f t:% 3f \n",
                    wName,
                    GLDesc[pCtx]->VertexData->Tab[wi].point.x,
                    GLDesc[pCtx]->VertexData->Tab[wi].point.y,
                    GLDesc[pCtx]->VertexData->Tab[wi].point.z,
                    GLDesc[pCtx]->VertexData->Tab[wi].normal.x,
                    GLDesc[pCtx]->VertexData->Tab[wi].normal.y,
                    GLDesc[pCtx]->VertexData->Tab[wi].normal.z,
                    GLDesc[pCtx]->VertexData->Tab[wi].texcoords.x,
                    GLDesc[pCtx]->VertexData->Tab[wi].texcoords.y);
            }
        }// else

        if (!GLDesc[pCtx]->Indexes)
            fprintf (pOutput,"             Indexes data is missing.\n");
        else
        {
        wICount=GLDesc[pCtx]->Indexes->count();
        if (wICount>pLimit)
                    wICount=pLimit;
        fprintf (pOutput,"Indices count <%ld> displayed <%ld> <%s>\n"
                         "-------------------------------------------------------\n",
                 GLDesc[pCtx]->Indexes->count(),
                 wICount,
                 wLimitChar);
        if (GLDesc[pCtx]->Indexes->count()==0)
            fprintf (pOutput," No indices\n");
        else
        for (long wi=0; wi < wICount;wi+=3)
                fprintf (pOutput,"%u %u %u\n",
                         GLDesc[pCtx]->Indexes->Tab[wi],
                         GLDesc[pCtx]->Indexes->Tab[wi+1],
                        GLDesc[pCtx]->Indexes->Tab[wi+2]);
        }//else
}// _printContext

zbs::ZArray<glm::vec3>*
ZObject::generateNormals(zbs::ZArray<ZVertice>*pVertices,zbs::ZArray<NormalDirection>*VNormalDir)
{
    computeNormals(pVertices,VNormalDir);
    return toFlatNormals(pVertices);
}

void
ZObject::computeNormals(zbs::ZArray<ZVertice>* pVertices,zbs::ZArray<NormalDirection>*VNormalDir)
{
    glm::vec3 wNormal;
    glm::vec3 wTriangle[3];

    bool wHaveToCompute=false;
//    zbs::ZArray<Vertice_type> pReturn;
    long wNormIdx=0;
    NormalDirection wND=Compute ; /* default : compute normals */
    for (unsigned int wi=0;wi<pVertices->size();wi=wi+3)
       {
        if ((pVertices->size()-wi)<3)
        {
          fprintf (stderr,"ZObject::computeNormals-W-NOTTRG Object <%s> : Vertices object are not grouped per triangle. Some normals may be missing.\n",
                   Name);
          return ;
        }
        if (!VNormalDir)
            wHaveToCompute=true;
        else
            {
            if (VNormalDir->size()==0)
                    wHaveToCompute=true;
            if (wNormIdx>VNormalDir->size())
                    wHaveToCompute=true;
            }

        if (wHaveToCompute)
            {
            wNormal=computeFaceNormal(pVertices->Tab[wi].point,pVertices->Tab[wi+1].point,pVertices->Tab[wi+2].point);

//            wTriangle[0]=pVertices->Tab[wi].point;/* compute normal for a triangle A B C */
//            wTriangle[1]=pVertices->Tab[wi+1].point;
//            wTriangle[2]=pVertices->Tab[wi+2].point;
//            wNormal= glm::normalize(glm::cross(wTriangle[2] - wTriangle[0], wTriangle[1] - wTriangle[0]));

            pVertices->Tab[wi].normal=wNormal;
            pVertices->Tab[wi+1].normal=wNormal;
            pVertices->Tab[wi+2].normal=wNormal;
            continue;
            }


        if (wNormIdx>=VNormalDir->size())
                {
                fprintf (stderr,
                         " computeNormal-E-IVNORM Normal directions are not homogeneous per triangles for object <%s>\n"
                         " May be normals have to be forced   (setComputeNormals(false))\n",
                         Name);
                wNormIdx++;
                wND=Compute;
                }
            else
                wND=VNormalDir->Tab[wNormIdx];

//       switch (VNormalDir[wNormIdx])
        switch (wND)
           {
           case Front:
               wNormal= NDirFront;
           break;
           case Back:
               wNormal= NDirBack;
           break;
           case Top:
               wNormal= NDirTop;
           break;
           case Bottom:
               wNormal= NDirBottom;
           break;
           case Left:
               wNormal= NDirLeft;
           break;
           case Right:
               wNormal= NDirRight;
           break;
           default: /* case ZObject::Compute or unknow */
           wTriangle[0]=pVertices->Tab[wi].point;/* compute normal for a triangle A B C */
           wTriangle[1]=pVertices->Tab[wi+1].point;
           wTriangle[2]=pVertices->Tab[wi+2].point;

           wNormal= glm::normalize(glm::cross(wTriangle[2] - wTriangle[0], wTriangle[1] - wTriangle[0]));
     //      wNormal=CalculateSurfaceNormal(wTriangle);
           } // switch
       pVertices->Tab[wi].normal=wNormal;
       pVertices->Tab[wi+1].normal=wNormal;
       pVertices->Tab[wi+2].normal=wNormal;

       wNormIdx++;
       }// for loop

    return ;
}//ZObject::computeNormals()

int ZObject::createNormVisuContextByRank(const long pShaderRank)
{
    if (ShaderContext[NormVisu])
            delete ShaderContext[NormVisu];
    if (createShaderContextByRank(NormVisu,pShaderRank) < 0)
                        return -1;
    zbs::ZArray<glm::vec3> * wNormVisu=generateNormVisu(GLDesc[Draw]->VertexData,NormVisuHeight);
    if (wNormVisu==nullptr)
            return -1;
    if (GLDesc[NormVisu])
            delete GLDesc[NormVisu];
    GLDesc[NormVisu]=new ZGLObjDescriptor(wNormVisu);
    return 0;
}
int ZObject::createNormVisuContextByName(const char* pShaderName)
{
    if (ShaderContext[NormVisu])
            delete ShaderContext[NormVisu];
    if (createShaderContextByName(NormVisu,pShaderName) < 0)
                        return -1;
    zbs::ZArray<glm::vec3> * wNormVisu=generateNormVisu(GLDesc[Draw]->VertexData,NormVisuHeight);
    if (wNormVisu==nullptr)
            return -1;
    if (GLDesc[NormVisu])
            delete GLDesc[NormVisu];
    GLDesc[NormVisu]=new ZGLObjDescriptor(wNormVisu);
    return 0;
}

zbs::ZArray<glm::vec3> * ZObject::generateNormVisu(zbs::ZArray<ZVertice>* pVertices, const float pNormVisuHeight)
{
    zbs::ZArray<glm::vec3> * wZANormVisu = new zbs::ZArray<Vertice_type>;
 //   ZANormVisu.clear();

 //   Vertice_type wTriangle[3];
    Vertice_type wCenter;
    Vertice_type wTarget;
    for (long wi=0;wi<pVertices->count();wi+=3)
        {
        wCenter=calculateCenter(pVertices->Tab[wi].point,
                                pVertices->Tab[wi+1].point,
                                pVertices->Tab[wi+2].point);

        wZANormVisu->push_back(wCenter);
        wTarget = pVertices->Tab[wi].normal * pNormVisuHeight;
        wTarget = wTarget+ wCenter;
        wZANormVisu->push_back(wTarget);
        }
    return wZANormVisu;
}//generateNormVisu

void ZObject::generateNormVisuFromCtx(const float pNormVisuHeight, const int pCtx)
{
    if (GLDesc[NormVisu]->VertexData)
            delete GLDesc[NormVisu]->VertexData;

    GLDesc[NormVisu]->VertexData= new zbs::ZArray<ZVertice>;

    if (!GLDesc[pCtx]->NormalComputed)
                this->computeNormals(GLDesc[pCtx]->VertexData,GLDesc[pCtx]->VNormalDir);
 //   ZANormVisu.clear();

 //   Vertice_type wTriangle[3];
    glm::vec3 wCenter;
    glm::vec3  wTarget;
    for (long wi=0;wi<GLDesc[pCtx]->VertexData->count();wi+=3)
        {
        wCenter=calculateCenter(GLDesc[pCtx]->VertexData->Tab[wi].point,
                                GLDesc[pCtx]->VertexData->Tab[wi+1].point,
                                GLDesc[pCtx]->VertexData->Tab[wi+2].point);

        GLDesc[NormVisu]->VertexData->push_back(wCenter);
        wTarget = GLDesc[pCtx]->VertexData->Tab[wi].normal * pNormVisuHeight;
        wTarget = wTarget + wCenter;
        GLDesc[NormVisu]->VertexData->push_back(wTarget);
        }
    return ;
}//generateNormVisu


/* matrices see _preprocessGL */
/*
void ZObject::_setupMatrices()
{
    Model =  glm::translate(GLResources->Camera->getModel(), Position);
    View  =  GLResources->Camera->GetViewMatrix();

    Projection = glm::perspective(glm::radians(GLResources->Camera->Zoom),
                                                GLResources->getWindowRatio(),
                                                0.1f,
                                                100.0f);
}
*/
/*void ZObject::setModelRotation (float pAngle,glm::vec3 pAxis) {RotationAngle=pAngle;RotationAxis=pAxis;}

void ZObject::rotate90 () {RotationAngle = glm::radians(90.0f); RotationAxis= glm::vec3(0.0,0.0,1.0);}
void ZObject::rotate270 () {RotationAngle = glm::radians(270.0f); RotationAxis= glm::vec3(0.0,0.0,1.0);}
*/

ZGLObjDescriptor*
ZObject::getGLDescriptor(const int pCtx)
{
    if (GLDesc[pCtx]!=nullptr)
            return GLDesc[pCtx];
    fprintf(stderr,"ZObject-F-GLDESCNULL Object descriptor for <%s> context has not been initialized for object <%s>.\n"
            "    invoke setupGL() prior using GLDescriptor.\n",
            decodeShdCtx(pCtx),
            Name);
    exit(EXIT_FAILURE);
}
/*
ZGLObjDescriptor*
ZObject::getGLDescriptor()
{
    if (GLDesc[Draw]!=nullptr)
            return GLDesc[Draw];
    fprintf(stderr,"ZObject-F-GLDESCNULL Object descriptor for Draw context has not been initialized for object <%s>.\n"
            "    invoke setupGL() prior using GLDescriptor.\n",Name);
    exit(EXIT_FAILURE);
//        exit (EXIT_FAILURE);
}
ZGLObjDescriptor*
ZObject::getGLShapeDescriptor()
{
    if (GLDesc[Shape]!=nullptr)
            return GLDesc[Shape];
    fprintf(stderr,"ZObject-F-GLDESCNULL Object descriptor Shape context has not been initialized for object <%s>.\n"
            "    invoke setupGLShape() prior using GLDescriptor.\n",Name);
    exit(EXIT_FAILURE);
//        exit (EXIT_FAILURE);
}
ZGLObjDescriptor*
ZObject::getGLNormVisuDescriptor()
{
    if (GLDesc[NormVisu]!=nullptr)
            return GLDesc[NormVisu];
    fprintf(stderr,"ZObject-F-GLDESCNULL Object descriptor Normal visu  context has not been initialized for object <%s>.\n"
            "    invoke setupGLNormalVisu() prior using GLDescriptor.\n",Name);
    exit(EXIT_FAILURE);
//        exit (EXIT_FAILURE);
}
*/

/*---Matrices---------------*/


int ZObject::createShaderContextByName(const int pCtx,const char* pShaderName )
{
    ZShader* wSh=GLResources->getShaderByNamePtr(pShaderName);
    if (!wSh)
            return -1;
    if (ShaderContext[pCtx]==nullptr)
            ShaderContext[pCtx]=new ZShaderContext(wSh);
        else
            fprintf (stderr,"ZObject::createShaderContextByName-I-SHDEXIST Shader context for <%s> already exists while trying to create it.\n",decodeShdCtx(pCtx));
    ShaderContext[pCtx]->reset();
    return 0;
}
int ZObject::createShaderContextByRank(const int pCtx, const int pShaderRank)
{
    ZShader* wSh=GLResources->getShaderByRankPtr(pShaderRank);
    if (!wSh)
            return -1;
    if (ShaderContext[pCtx]==nullptr)
            ShaderContext[pCtx]=new ZShaderContext(wSh);
    ShaderContext[pCtx]->reset();
    return 0;
}
void ZObject::createShaderContextFromCopy(const int pCtxDest,const int pCtxSrc)
{
    if (ShaderContext[pCtxDest]==nullptr)
            ShaderContext[pCtxDest]=new ZShaderContext(*ShaderContext[pCtxSrc]);
}

//=============Matrices========================


void _preprocessGLMatrices(ZShader* wShader,
                           ZMatCtx* pMatCtx)
{

    if (!pMatCtx)
         return;
//    ZShader* wShader=GLResources->getActiveShaderPtr();
    if (wShader==nullptr)
            {
            fprintf(stderr,"ZObject::_preprocessGLMatrices-E-NOSHD No valid shader while preprocessing GL environment.No action made.\n");
            return;
            }
/* setup matrices and transmit it to shader */

    /* if there is a request for local object translation : compute Model locally , if not, use what has been computed by meta object */
    if (pMatCtx->Model)
        {
        if (pMatCtx->hasPosition() ) /* object has a local position */
                pMatCtx->Model->stdTranslate(pMatCtx->getPosition());

        if (pMatCtx->hasRotation())
            pMatCtx->Model->rotate(pMatCtx->RotationAngle,pMatCtx->getRotationAxis());

        wShader->setMat4(__MODEL_SHADER_NAME__,pMatCtx->Model->get());
        }//if (Model)


    /* if ZObject belongs to a ZMetaObject, then other matrices (view and projection) are supposed to be computed at
     * ZMetaObject level , then are given */
    if (pMatCtx->View!=nullptr)
        {
        pMatCtx->View->setValue(GLResources->Camera->GetViewMatrix());
        wShader->setMat4(__VIEW_SHADER_NAME__,pMatCtx->View->get());
        }


    if (pMatCtx->Projection)
        {
        pMatCtx->Projection->setValue( glm::perspective(glm::radians(GLResources->Camera->Zoom),
                                                    GLResources->getWindowRatio(),
                                                    0.1f,
                                                    100.0f));
        wShader->setMat4(__PROJECTION_SHADER_NAME__,pMatCtx->Projection->get());
        }

    if (pMatCtx->Normal)
        {
        pMatCtx->Normal->setValue( glm::transpose(glm::inverse(GLResources->Camera->GetViewMatrix()*GLResources->Camera->getModel())));
        wShader->setMat4(__NORMAL_SHADER_NAME__,pMatCtx->Normal->get());
        }

}//_preprocessMatrices



//========================================
const char*
ZObject::getFatherName()
{
    switch (FatherType)
    {
    case None:
        return "None";
    case Sphere:
        return Father.SPH->Name;
    case MetaObject:
        return Father.MTO->Name;
    default:
        return Father.OBJ->Name;
    }
}


const char* decodeObjectType(ZObject::ObjType pType)
{
    switch (pType)
    {

    case ZObject::Openbox:
            return "Openbox";

    case ZObject::Box:
            return "Box";
    case ZObject::Arc:
            return "Arc";
    case ZObject::ArcStrip:
            return "ArcStrip";
    case ZObject::Pipe:
            return "Pipe";
    case ZObject::Sphere:
            return "Sphere";
    case ZObject::Circle:
            return "Circle";
    case ZObject::MetaObject:
            return "MetaObject";
    case ZObject::Candy:
            return "Candy";
    default:
            return "Unknown";
    }
}
