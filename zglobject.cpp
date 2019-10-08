#include <zglobject.h>

//#include <zworld.h>
//#include <zcamera.h>

#include <zshader.h>

#include <ztoolset/zdatabuffer.h>


static float VArray[]= {
     0.5f,  0.5f,  0.5f,
    -0.5f,  0.5f,  0.5f,
    -0.5f, -0.5f,  0.5f,
    0.5f, -0.5f,  0.5f,
    0.5f, -0.5f,  0.5f
    };

void ZObject::initializeGL()
{

    ZGLStatus wSt=ZGL_SUCCESS;
//bool wCompletion=false;

// Set global information
//  glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
//    glClearColor (ZGreyColor.Red,ZGreyColor.Green,ZGreyColor.Blue,ZGreyColor.Alpha);

    // Create Shader (Do not release until VAO is created)

    while (true) /* convenience infinite loop */
    {
/*    if ((wSt=loadShadersFromFile("/home/gerard/Development/ui3dg/shaders/ui3d-shader.vert","/home/gerard/Development/ui3dg/shaders/ui3d-shader.frag"))!=ZGL_SUCCESS)
            break;
 */
    if ((wSt=Shader->link())!=ZGL_SUCCESS)
        {
            fprintf (stderr,"Shader link failed \n%s",Shader->log());
            break;
        }
#ifdef __COMMENT__
    UniformWorld=Shader->getUniformHandle("Model");
    UniformView=Shader->getUniformHandle("View");
    UniformProjection=Shader->getUniformHandle("Projection");
#endif

/*
    unsigned int VBO, VAO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // texture coord attribute
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    */

    glGenVertexArrays(1, &VaoHandle);
    glBindVertexArray(VaoHandle);

    glGenBuffers(1, &VboHandle);
    glBindBuffer(GL_ARRAY_BUFFER, VboHandle);
    glBufferData(GL_ARRAY_BUFFER, getVertexSize(), getVertexPtr(), GL_STATIC_DRAW);

    /* for indices */
    glGenBuffers(1, &EboHandle);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EboHandle);  /*for indices */
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, getIndexSize(), getIndexPtr(), GL_STATIC_DRAW); /* for indices */

    /* object position */
//        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glVertexAttribPointer(0, ZVertex::PositionTupleSize, GL_FLOAT, GL_FALSE, ZVertex::getStride(), (void*)0);
    glEnableVertexAttribArray(0);

    /* object color */
    glVertexAttribPointer(1, ZVertex::ColorTupleSize, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)ZVertex::getColorOffset());
    glEnableVertexAttribArray(1);

       /* debug */
        int wStride= ZVertex::getStride();
        int WTuplesize=ZVertex::PositionTupleSize;
        int wOffset= ZVertex::getPositionOffset();

        wStride= sizeof(VArray)*sizeof(float);
        WTuplesize=3;
        wOffset= 0;
    break; /* everything is ok : exit the loop */
    }// while true

    // Release (unbind) all
/*
    if (hasIndex())
            Vao->release();
    Ebo->release();
    Vbo->release();
    */

    Shader->release();

}//initializeGL


/*
void ZGLObject::initGL()
{

    VboDesc=QOpenGLFunctions::glGenBuffers((GLsizei)1,&VboDesc );
    QOpenGLFunctions::glBindBuffer(GL_ARRAY_BUFFER,VboDesc);
    QOpenGLFunctions::glBufferData(GL_ARRAY_BUFFER, _VertexArray::size()*sizeof(ZVertex), VertextoFlat(), Usage);

    QOpenGLFunctions::glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, ZVertex::getStride(), (void*)0);
    GLTestError();
    QOpenGLFunctions::glEnableVertexAttribArray(0);

    if (hasIndex())
        {
        EboDesc=QOpenGLFunctions::glGenBuffers((GLsizei)1,&EboDesc );
        QOpenGLFunctions::glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,EboDesc);
        QOpenGLFunctions::glBufferData(GL_ELEMENT_ARRAY_BUFFER, _IndexArray::size()*sizeof(ZIndex), IndicetoFlat(), Usage);
        GLTestError();
        IndexSetup=true;
        }
  }
*/

void ZObject::clearGL()
{


/*
    if (Vao!=nullptr)
            {
            Vao->destroy();
            delete Vao;
            }
    if (Vbo!=nullptr)
        {
        Vbo->destroy();
        delete Vbo;
        }
    if (Ebo!=nullptr)
        {
        Ebo->destroy();
        delete Ebo;
        }
*/
    if(Shader!=nullptr)
            delete Shader;
    if (VaoHandle)
            glDeleteVertexArrays(1, &VaoHandle);
    if (VboHandle)
        glDeleteBuffers(1, &VboHandle);
    if (EboHandle)
        glDeleteBuffers(1, &EboHandle);
}// clearGL

ZGLStatus ZObject::loadShadersFromFile(const char* pVertexFile,const char* pFragmentFile)
{
    ZGLStatus wSt;
    if (Shader!=nullptr)
            delete Shader;
    Shader=new ZShader;
    if ((wSt=Shader->loadVertexFromFile(pVertexFile))!=ZGL_SUCCESS)
            return wSt;

    return Shader->loadFragmentFromFile(pFragmentFile);
}
ZGLStatus ZObject::loadShadersFromQResource(const char* pVertexResource,const char* pFragmentResource)
{
    ZGLStatus wSt;
    if (Shader!=nullptr)
            delete Shader;
    Shader=new ZShader;
    if ((wSt=Shader->loadVertexFromQResource(pVertexResource))!=ZGL_SUCCESS)
            return wSt;

    return Shader->loadFragmentFromQResource(pFragmentResource);
}

void ZObject::setFrontDrawMode(const GLDrawMode pDM )
{
    FrontDrawMode = GLDM_Nothing;
    FrontDrawMode = pDM;
}

void ZObject::setBackDrawMode(const GLDrawMode pDM )
{
    BackDrawMode = GLDM_Nothing;
    BackDrawMode = pDM;
}

void ZObject::setupDrawMode()
{
    if (Shader==nullptr)
            initializeGL();

//    return;
    switch (FrontDrawMode)
    {
        case GLDM_Fill:
            glPolygonMode(GL_FRONT,GL_FILL);
            break;
        case GLDM_Line:
            glPolygonMode(GL_FRONT,GL_LINE);
            break;
        case GLDM_Point:
            glPolygonMode(GL_FRONT,GL_POINT);
            break;
        default:
            break;
    }// switch

    switch (BackDrawMode)
    {
        case GLDM_Fill:
            glPolygonMode(GL_BACK,GL_FILL);
            break;
        case GLDM_Line:
            glPolygonMode(GL_BACK,GL_LINE);
            break;
        case GLDM_Point:
            glPolygonMode(GL_BACK,GL_POINT);
            break;
        default:
            break;
    }// switch

}//setupDrawMode

#define FPRINT(TXT) fprintf(pOutput,TXT);

 void ZObject::listAll(FILE* pOutput)
 {
     fprintf(pOutput,"Vertexes-----------\n");

     for (long wi=0;wi<vertexCount();wi++)
             fprintf(pOutput,"%4ld - Point % 6.4f % 6.4f % 6.4f Color % 6.4f % 6.4f % 6.4f % 6.4f\n",
                     wi,
                     vertex(wi).m_position.x,
                     vertex(wi).m_position.y,
                     vertex(wi).m_position.z,
 //                    vertex(wi).m_position.w,
                     vertex(wi).m_color.Red,
                     vertex(wi).m_color.Blue,
                     vertex(wi).m_color.Green,
                     vertex(wi).m_color.Alpha);



     fprintf(pOutput,"Indices-----------\n");

     if (indexCount()==0)
         fprintf(pOutput," no index \n");
        else
        for (long wi=0;wi<indexCount();wi++)
             fprintf(pOutput,"%4ld - %4d \n",
                     wi,
                     index(wi));
    fprintf (pOutput,"----------------\n");

 }
 void ZObject::codeList(FILE* pOutput)
 {
     if (!vertexCount())
     {
         fprintf (pOutput,"    // no vertex found \n");
         return;
     }
     long wi=0;
     fprintf(pOutput,"     float vertices[] = { \n");
     fprintf(pOutput,"    % 6.4ff , % 6.4ff , % 6.4ff , % 6.4ff , % 6.4ff , % 6.4ff , % 6.4ff ",
             vertex(wi).m_position.x,
             vertex(wi).m_position.y,
             vertex(wi).m_position.z,
//                    vertex(wi).m_position.w,
             vertex(wi).m_color.Red,
             vertex(wi).m_color.Blue,
             vertex(wi).m_color.Green,
             vertex(wi).m_color.Alpha);
     wi++;
     for (;wi<vertexCount();wi++)
             fprintf(pOutput,",\n    % 6.4ff , % 6.4ff , % 6.4ff , % 6.4ff , % 6.4ff , % 6.4ff , % 6.4ff ",
                     vertex(wi).m_position.x,
                     vertex(wi).m_position.y,
                     vertex(wi).m_position.z,
 //                    vertex(wi).m_position.w,
                     vertex(wi).m_color.Red,
                     vertex(wi).m_color.Blue,
                     vertex(wi).m_color.Green,
                     vertex(wi).m_color.Alpha);

     fprintf (pOutput," } ; \n");

     fprintf(pOutput,"//    Indices-----------\n");

     if (indexCount()==0)
     {
         fprintf(pOutput,"//   no index \n");
         return;
     }
    wi=0;
    fprintf(pOutput," %d ",
            index(wi));
    wi++;
    fprintf(pOutput,"     float indices[] = { \n");
    int wj=1;
    while (wi<indexCount())
        {
        for (;(wj<4)&&(wi<indexCount());wj++,wi++)
             fprintf(pOutput,", %d ",
                     index(wi));
        fprintf(pOutput,"\n");
        wj=0;
        }
    fprintf (pOutput," } ; \n");
    return;
 }
 void ZObject::codePositionList(FILE* pOutput)
 {
     if (!vertexCount())
     {
         fprintf (pOutput,"    // no vertex found \n");
         return;
     }
     long wi=0;
     const char* wName;
     fprintf(pOutput,"// Only position, no color \n");
     fprintf(pOutput,"     float vertices[] = { \n");
     for (;wi<(vertexCount()-1);wi++)
        {
         if (wi<VertexName.count())
                wName=VertexName[wi];
         else
             wName="Unknown";

         fprintf(pOutput,"    % 6.4ff , % 6.4ff , % 6.4ff  , // %s index %ld\n",
                 vertex(wi).m_position.x,
                 vertex(wi).m_position.y,
                 vertex(wi).m_position.z
                 ,wName,wi);

        }// for
     fprintf(pOutput,"    % 6.4ff , % 6.4ff , % 6.4ff  } ; // %s index %ld\n",
             vertex(wi).m_position.x,
             vertex(wi).m_position.y,
             vertex(wi).m_position.z
             ,wName,wi);


     fprintf(pOutput,"//    Indices-----------\n");

     if (indexCount()==0)
     {
         fprintf(pOutput,"//   no index \n");
         return;
     }

     char wIndiceDesc[50];
     wIndiceDesc[0]='\0';

    fprintf(pOutput,"     unsigned int indices[] = { \n");
    wi=0;
    fprintf(pOutput," %d ",
            index(wi));
    strcat(wIndiceDesc,VertexName[index(wi)]);
    strcat(wIndiceDesc," - ");
    wi++;
    int wj=1;
    while (wi<indexCount())
        {

        for (;(wj<3)&&(wi<indexCount());wj++,wi++)
        {
             fprintf(pOutput,", %d ",
                     index(wi));
             strcat(wIndiceDesc,VertexName[index(wi)]);
             strcat(wIndiceDesc," - ");
        }
        fprintf(pOutput," // %s \n",wIndiceDesc);
        wj=0;
        wIndiceDesc[0]='\0';
        }
    fprintf (pOutput," } ; \n");
    return;
 }


ZMat4
ZObject::getTransformation()
{

//   ObjectToModel=World->MainScaleMatrix;  /* includes initial scaling to get -1.0, 0.0 ,1.0  referential for each object */

//   ObjectToModel.scale(World->Scale.x,World->Scale.y,World->Scale.z);   /* additional scaling if any */
//   ObjectToModel.rotate(RotationAngle,RotationAxis.toQVector3D());      /* rotate object if needed */

//   Position *= World->Scale;

   ObjectToModel.setToIdentity();

   ObjectToModel.translate(Position);                     /* move object to its position if required */

   return ObjectToModel;
}


ZGLStatus
ZObject::setPosition(float pX,float pY,float pZ)
{
ZGLStatus wSt=ZGL_SUCCESS;
    ZVect3 wPos(pX,pY,pZ);
    if (World!=nullptr)
    {

        if ((wSt=World->check(wPos))!=ZGL_SUCCESS)
                            return wSt;

    }
    Position=wPos;
    return ZGL_SUCCESS;
}


ZGLStatus
ZObject::draw(ZCamera* pCamera)
{

  // Clear
 // ZGLContext::glClear(GL_COLOR_BUFFER_BIT);

 //   setupDrawMode();

    /* 1- compute model matrix (object to model) :
     * set up the object with its dimensions, its rotation,  and its position vs 0,0,0 origin coords
     * 1.1- scale
     * 1.2- rotate
     * 1.3- translate
     *
glm::mat4 myModelMatrix = myTranslationMatrix * myRotationMatrix * myScaleMatrix;
glm::vec4 myTransformedVector = myModelMatrix * myOriginalVector;
     *
     * 2- compute Camera matrix

glm::mat4 CameraMatrix = glm::lookAt(
    cameraPosition, // the position of your camera, in world space
    cameraTarget,   // where you want to look at, in world space
    upVector        // probably glm::vec3(0,1,0), but (0,-1,0) would make you looking upside-down, which can be great too
);

     *
     */

    setupDrawMode();

    ZMat4 wObjectToModel= getTransformation(); /* specific to object : scale + translate object */


    pCamera->CameraMatrix.selfLookAt(pCamera->CameraPosition,
                                    pCamera->CameraTarget,
                                    pCamera->CameraHeading);

//    ZVertexArray* wVertex=(ZVertexArray*)pObject.ZVertexArray::clone();

//    wObjectToModel.setToIdentity();

    pCamera->ProjectionMatrix=pCamera->getProjectionMatrix();

    Shader->use();
#ifdef __COMMENT__
    Shader->setMat4ByHandle(UniformWorld,wObjectToModel);

 //   pObject.Shader->setUniformValue(pObject.UniformView,Camera->CameraMatrix);
    Shader->setMat4ByHandle(UniformView,pCamera->CameraMatrix);
    Shader->setMat4ByHandle(UniformProjection,pCamera->ProjectionMatrix);
#endif

    glBindVertexArray(VaoHandle);
    glBindBuffer(GL_ARRAY_BUFFER, VboHandle);
    if (hasIndex())
    {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EboHandle);
        glDrawElements(GL_TRIANGLES, (GLsizei)indexCount(),0,IndicetoFlat());
    }
    else
        glDrawArrays(GL_TRIANGLES, 0, (GLsizei)vertexCount());

    glfwSwapBuffers(pCamera->GLFWContext);
    Shader->release();
    ToBeDrawn=false;
    return ZGL_SUCCESS ;
}
