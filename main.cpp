#ifdef __USE_GLAD__
#include <glad/glad.h>
#else
#include <GL/glew.h>
#endif

#include </usr/include/GLFW/glfw3.h>

#include <iostream>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/perpendicular.hpp>

#include <camera.h>
#include <zshader.h>

#include <zmaterial.h>

#include <zglresource.h>

#include <zobject.h>
#include <zobjectfunctions.h>

#include <zcandy.h>

#include <zcandytext.h>

#include <zsphere.h>
#include <ztexture.h>
/* text rendering */
#include <zglunicode.h>

#include <zglconstants.h>


void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow *window);

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);

// settings
const unsigned int SCR_WIDTH = 1600;
const unsigned int SCR_HEIGHT = 1200;

// camera
ZCamera camera(Vertice_type(0.0f, 0.0f, 3.0f),glm::vec3(1.0f, 0.2f, 1.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

// timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;
bool err65539Once=false;
void errorCallback_Glfw(int pErr,const char* pErrString)
{
    if (pErr==65539L)
    {
        if (!err65539Once)
            {
            fprintf(stderr,"GLFW-ERR Some glfw errors raised with id <%d> <%s> \n"
                    "          Following errors of same id (<%d>) will be ignored...\n",pErr,pErrString,pErr);
            err65539Once = true;
            }
            return;
    }
    fprintf(stderr,"GLFW-ERR glfw error <%d> <%s> \n",pErr,pErrString);
}

bool GLAbortOnFailure=false;

void GLAPIENTRY
MessageCallback( GLenum source,
                 GLenum type,
                 GLuint id,
                 GLenum severity,
                 GLsizei length,
                 const GLchar* message,
                 const void* userParam )
{
  fprintf( stderr, "GL CALLBACK: %s type = 0x%x, severity = 0x%x, message = %s\n",
           ( type == GL_DEBUG_TYPE_ERROR ? "** GL ERROR **" : "" ),
            type, severity, message );
  return;
}

Vertice_type objectLocation=glm::vec3(0.0f, 0.0f, 0.0f);
GLFWcursor* DefaultCursor=nullptr;
GLFWcursor* HandCursor=nullptr;
GLFWcursor* CurrentCursor=nullptr;
int main()
{

 //   _linuxListFonts();

    GLResources->_linuxSearchFonts((const utf8_t*)"liberation");


    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


    glfwSetErrorCallback(errorCallback_Glfw);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // uncomment this statement to fix compilation on OS X
#endif

    // glfw window creation
    // --------------------
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", nullptr, nullptr);
    if (window == nullptr)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);


    glfwSetMouseButtonCallback(window, mouse_button_callback);



    HandCursor=glfwCreateStandardCursor(GLFW_HAND_CURSOR);
    DefaultCursor=glfwCreateStandardCursor(GLFW_ARROW_CURSOR);
    CurrentCursor=DefaultCursor;
#if __USE_GLAD__
    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        glfwTerminate();
        abort();
    }
#else
    glewExperimental = GL_TRUE;
    GLenum err = glewInit();
    if (err != GLEW_OK )
    {
      /* Problem: glewInit failed, something is seriously wrong. */
      fprintf(stderr, "Glew-Init Error: %s\n", glewGetErrorString(err));
      glfwTerminate();
      abort();
    }
    if (!GLEW_VERSION_2_0) {
        fprintf(stderr, "Glew-Init Error : No support for OpenGL 2.0 found\n");
        abort();
    }
#endif
#ifdef __USE_GLAD__
    printf("Using OpenGL version %d.%d\n", GLVersion.major, GLVersion.minor);
#else
    printf("Using OpenGL glew version %d.%d OpenGL version %d.%d\n", GLEW_VERSION_MAJOR, GLEW_VERSION_MINOR,GL_MAJOR_VERSION,GL_MINOR_VERSION);
#endif


    GLResources->registerGLWindow(window);

    GLResources->registerZCamera(&camera);

    // During init, enable debug output
    glEnable              ( GL_DEBUG_OUTPUT );

    GLAbortOnFailure=true;
    glDebugMessageCallback( (GLDEBUGPROC) MessageCallback, &GLAbortOnFailure );



    // configure global opengl state
    // -----------------------------
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

/*
    glEnable( GL_POLYGON_SMOOTH );
    glHint( GL_LINE_SMOOTH_HINT, GL_DONT_CARE );
    glHint( GL_POLYGON_SMOOTH_HINT, GL_NICEST );
*/
    // Define the viewport dimensions
    glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);

//----------Shaders----------------------------------

//   ZShader sphereShader("zsphere.vs", "zsphere.fs","SphereShader");

    /* various shaders according lighting effects / texture / material as requested */

   long wTextureShader=GLResources->loadShader("zlighting.vs", "ztexture.fs","TextureShader");
   long wLampShader=GLResources->loadShader("zlamp.vs", "zlamp.fs","LampShader");
   long wColorShader=GLResources->loadShader("zlighting.vs", "zcolor.fs","ColorShader");
   long wMaterialShader=GLResources->loadShader("zlighting.vs", "zmaterial.fs","MaterialShader");

   /* for text & text boxes */
   GLResources->loadShader("zgltext.vs", "zgltext.fs",__TEXT_SHADER__);
   GLResources->loadShader("zlamp.vs", "zlamp.fs",__TEXTBOX_SHADER_SHAPE__);
//   GLResources->loadShader("ztextboxtexture.vs", "ztextboxtexture.fs",__TEXTBOX_SHADER_FILL__);
   GLResources->loadShader("zlighting.vs", "ztexture.fs",__TEXTBOX_SHADER_FILL__);


//----------Textures---------------------------------

    long wWood=GLResources->loadTexture("wood.png","wood",GL_TEXTURE1);
    long wTissueGrey=GLResources->loadTexture("tissuegrey.jpeg","tissuegrey",GL_TEXTURE1);
    long wTissueBlue=GLResources->loadTexture("tissueblue.png","tissueblue",GL_TEXTURE1);
    long wTissueBrown=GLResources->loadTexture("tissuebrownbure.jpeg","tissuebrownbure",GL_TEXTURE1);
    long wMetal=GLResources->loadTexture("metal.png","metal",GL_TEXTURE1);
    long wMoon=GLResources->loadTexture("moon1024.bmp","moon",GL_TEXTURE1);
    long wEarth=GLResources->loadTexture("earth2048.bmp","earth",GL_TEXTURE1);


/*    ZTexture wTexWoodFloor("wood.png",GL_TEXTURE1);

    ZTexture wTexTissueGrey("tissuegrey.jpeg",GL_TEXTURE1);
    ZTexture wTexTissueBluePale("tissueblue.png",GL_TEXTURE1);
    ZTexture wTexTissueBrown("tissuebrownbure.jpeg",GL_TEXTURE1);
    ZTexture wTexMetal("metal.png",GL_TEXTURE1);

    ZTexture wTexMoon("moon1024.bmp",GL_TEXTURE1);
    ZTexture wTexEarth("earth2048.bmp",GL_TEXTURE1);
*/
//---------------Fonts-----------------------------------------------

    long wRet=GLResources->addFont("LiberationMono-Regular","LiberationMono",FLOC_Sytem);
    wRet=GLResources->addFont("Architex","Architex",FLOC_User);
    wRet=GLResources->addFont("UbuntuMono-R","UbuntuMono",FLOC_Sytem);

/*-------------Text Objects-------------------------*/

    ZGLTextProfile wUWriter(GL_TEXTURE0);

 //   wUWriter.newBoxShader(Draw,__TEXTBOX_SHADER_FILL__);
    wUWriter.newBoxShaderByRank(Draw,wTextureShader);
    wUWriter.newBoxShaderByRank(Shape,wLampShader);

    wUWriter.setBoxShader(Draw)->setTextureByRank(wTissueBlue);
    wUWriter.setBoxShader(Draw)->addVec3(__SHD_LIGHT_POSITION_UN__,&camera.LightPosition);
    wUWriter.setBoxShader(Draw)->addVec3(__SHD_VIEW_POSITION_UN__,&camera.CameraPosition);

/*    wUWriter.setBoxShader(Draw)->addMaterial(&ZCopper);
    wUWriter.setBoxShader(Draw)->addBool(__SHD_USE_TEXTURE_UN__,false);
    wUWriter.setBoxShader(Draw)->addVec3(__SHD_LIGHT_POSITION_UN__,&camera.LightPosition);
    wUWriter.setBoxShader(Draw)->addVec3(__SHD_VIEW_POSITION_UN__,&camera.CameraPosition);
*/
 //   wUWriter.setBoxShader(Draw)->addVec3("DefaultColor",&ZBlueColor);

//    wUWriter.setBoxShader(Shape)->setLineWidth(1.5);
//    wUWriter.setBoxShader(Shape)->addFloat(__SHD_ALPHA_UN__,1.0);


    ZGLText* wUText=wUWriter.newText();
    wRet=wUText->setText((utf32_t*)U"Жди меня, и я вернусь.","LiberationMono",24);
    if (wRet<0)
            {
            wUText->printLastError();
            exit(EXIT_FAILURE);
            }
     ZGLText* wBText=wUWriter.newText();

     wRet=wBText->setText((utf32_t*)U"Жди меня, и я вернусь.Только очень жди, Жди, когда наводят грусть Желтые дожди, Жди, когда снега метут, Жди, когда жара,\
Жди, когда других не ждут,Позабыв вчера.Жди, когда из дальних мест Писем не придет, Жди, когда уж надоест Всем, кто вместе ждет.","LiberationMono",24);


     if (wRet<0)
         {
          wBText->printLastError();
          exit(EXIT_FAILURE);
         }



#ifdef __COMMENT__
     ZGLUnicodeText* wNLText=wUWriter.newText();


     wRet=wNLText->setText((utf32_t*)U"Жди меня, и я вернусь.\n\
Только очень жди,\n\n\n\n\n\
Жди, когда наводят грусть\n\
Желтые дожди,\n\
Жди, когда снега метут,\n\
Жди, когда жара,\n\
Жди, когда других не ждут,\n\
Позабыв вчера.\n\
Жди, когда из дальних мест\n\
Писем не придет,\n\
Жди, когда уж надоест\n\
Всем, кто вместе ждет.\n","LiberationMono",24);

     if (wRet<0)
         {
         wNLText->printLastError();
         exit(1);
         }

     wNLText->setBox(700.0,350.0,ZYellowBright,RBP_Center|RBP_WordWrap|RBP_TruncChar, true,1.0,2.0);

     wNLText->setPosition(0.5f,0.5f,0.0f);

     wNLText->rotate270();

     wNLText->setupGL();


     ZGLUnicodeText* w90Text=wUWriter.newText();


     wRet=w90Text->setText((utf32_t*)U"Жди меня, и я вернусь.\n\
Только очень жди,\n\n\n\n\n\
Жди, когда наводят грусть\n\
Желтые дожди,\n\
Жди, когда снега метут,\n\
Жди, когда жара,\n\
Жди, когда других не ждут,\n\
Позабыв вчера.\n\
Жди, когда из дальних мест\n\
Писем не придет,\n\
Жди, когда уж надоест\n\
Всем, кто вместе ждет.\n","UbuntuMono",32);

     if (wRet<0)
         {
         w90Text->printLastError();
         exit(1);
         }

//     w90Text->setBox(250.0,500.0,ZYellowBright,RBP_TopJust|RBP_WordWrap|RBP_TruncChar, true,1.0,
//                     5.0,5.0,5.0,5.0);

     w90Text->setBoxDimensions(250,500);
     w90Text->setBoxLineSize(1.0);
     w90Text->setBoxVisible(true);
     w90Text->setBoxMarginAll(15);
     w90Text->setBoxFlag(RBP_Center|RBP_WordWrap|RBP_TruncChar);

     w90Text->setPosition(-0.8f,0.8f,0.0f);

     w90Text->setupGL();
#endif


 //=============================================================================

     ZSphere wSphere(0.10f,18,9,true, "Sphere"); /* generate object and its vertex data */

     wSphere.createShaderContextByRank(Draw,wTextureShader);
     wSphere.ShaderContext[Draw]->setTextureByRank(wTissueGrey);
     wSphere.ShaderContext[Draw]->addBool(__SHD_BLINNPHONG_UN__,true);
     wSphere.ShaderContext[Draw]->addVec3(__SHD_LIGHT_POSITION_UN__,&camera.LightPosition);
     wSphere.ShaderContext[Draw]->addVec3(__SHD_VIEW_POSITION_UN__,&camera.CameraPosition);
     wSphere.ShaderContext[Draw]->addVec3(__SHD_DEFAULTCOLOR_UN__,&ZBlueColor);
 //    wSphere.ShaderContext[Draw]->addFloat("DefaultAlpha",0.5f);
//     wSphere.ShaderContext[Draw]->addBool("useDefaultAlpha",true);
 //    wSphere.ShaderContext[Draw]->addMaterial(&ZGold);

//     wSphere.setAction(Draw,setupVertices);

     wSphere.DrawFigure[Draw]=GL_TRIANGLES;

     wSphere.createAllMatrices();

     wSphere.setPosition(glm::vec3(-0.5f,0.5f,0.2f));

//     wSphere.setComputeNormals(Draw,false); /* normal coords are given by ZPhere generation and not computed during setupGL */
//     wSphere.setComputeTexCoords(Draw,false); /* same for texture coordinates */

     wSphere.setupGL(Draw,CSO_setupAll | CSO_DeleteDataAfter); /* Only set values to GL buffer NB: Normals and Textures coordinates are computed separately */

 //    wSphere.print(20); /* list 20 max element of ZObject */

/*------------Lamp (light) object -------------------___*/

// Create vertices : vertices and indexes are created within boxSetup


    ZBoxComponents wLampComponents;
    ZObject* wLamp =boxSetup(0.05f,0.05f,0.05f,wLampComponents,"Lamp"); /* same shape as wBox (but scaled down ) */

// then Shape data is generated hereafter

    wLampComponents.generateShape(*wLamp); /* update ZObject wLamp with Shape vertexes */

//    wLamp->print(20);
    camera.setLightPosition(glm::vec3(1.0f, 0.2f, 1.0f));


//--------Create and setup shader context rules per context-----------

// Draw : main context
    wLamp->createShaderContextByName(Draw,"LampShader");
    wLamp->ShaderContext[Draw]->addVec3(__SHD_DEFAULTCOLOR_UN__,&ZYellowBright);
    wLamp->DrawFigure[Draw]=GL_TRIANGLES; // by default

// Shape : only line enclosing the object

    wLamp->createShaderContextByName(Shape,"LampShader");
    wLamp->ShaderContext[Shape]->addVec3(__SHD_DEFAULTCOLOR_UN__,&ZYellowAmbient);
    wLamp->DrawFigure[Shape]=GL_LINE_LOOP;

//  ---------Create matrices------------------

/* remark : no normal matrix for lampShader */
//    wLamp->createAllMatrices();

    wLamp->createMatrices(MAT_Model | MAT_View | MAT_Projection); /* creates matrix context and Model,View,Projection (Normal is not created)*/

//    wLamp->setAction(Draw,setupVertices);   // not really necessary : setupVertices is default
//    wLamp->setAction(Shape,setupVertices);  // not really necessary : setupVertices is default

// create the GL buffers for both contexts Draw and Shape

    wLamp->setupGL(Draw,CSO_setupVertices | CSO_DeleteDataAfter );
    wLamp->setupGL(Shape,CSO_setupVertices | CSO_DeleteDataAfter);

//==============Candy Object setup======================================
    const float wHigh=0.5f;
    const float wWidth=0.5f;
    const float wDepth=0.2f;


    ZCandy wCandy = generateCandy(wHigh,wWidth,wDepth,true,"Candy");

    /* create shader uniform values context (Draw context) that will be triggered before glDraw() operation
     * Remark: uniform values will be assigned to shader in this order
    */
    wCandy.createShaderContextByName(Draw,"ColorShader");

//    wCandy.ShaderContext[Draw]->setTextureByName("wood");/* nb: texture is exclusive from material */
    wCandy.ShaderContext[Draw]->addVec3(__SHD_LIGHT_POSITION_UN__,&camera.LightPosition);
    wCandy.ShaderContext[Draw]->addVec3(__SHD_VIEW_POSITION_UN__,&camera.CameraPosition);
    wCandy.ShaderContext[Draw]->addBool(__SHD_BLINNPHONG_UN__,false);
    wCandy.ShaderContext[Draw]->addVec3(__SHD_DEFAULTCOLOR_UN__,&ZBlueColor);

    wCandy.createShaderContextByName(Shape,"LampShader");

    wCandy.ShaderContext[Shape]->addVec3(__SHD_DEFAULTCOLOR_UN__,&ZYellowSpecular);
    wCandy.ShaderContext[Shape]->setLineWidth(1.5f);

//    wCandy.ShaderContext[Shape]->addBool("UseDefaultAlpha",true);

    wCandy.DrawFigure[Shape]=GL_LINE_LOOP;

    wCandy.createMatrices(MAT_Model | MAT_View | MAT_Projection | MAT_Normal);

    wCandy.setPosition(glm::vec3(0.2,0.2,0.2));

    wCandy.createShaderContextByRank(NormVisu,wLampShader);
    wCandy.ShaderContext[NormVisu]->addVec3(__SHD_DEFAULTCOLOR_UN__,&ZYellowSpecular);
    wCandy.ShaderContext[NormVisu]->setLineWidth(2.0f);
    wCandy.DrawFigure[NormVisu]=GL_LINE;

    wCandy.setupGLByContext(Draw,CSO__setAndComputeAll);
    wCandy.setupGLByContext(Shape,CSO_setupVertices);
    wCandy.setupGLByContext(NormVisu,CSO_setupVertices);

    /* get text box dimensions according candy dimensions and desired text box shape */
    TextZone wTZ    = wCandy.getTextZoneThin();
/*    fprintf (stdout,"*** Center <x:%f y:%f z:%f>  Width <%f> Height <%f>ToTopLeft  <x:%f y:%f z:%f>  \n"
                    " wCandy position <x:%f y:%f z:%f>  \n\n",
             wTZ.Center.x,wTZ.Center.y,wTZ.Center.z,
             wTZ.Width,
             wTZ.Height,
             wTZ.ToTopLeft.x,wTZ.ToTopLeft.y,wTZ.ToTopLeft.z,
             wCandy.getPosition().x,wCandy.getPosition().y,wCandy.getPosition().z);

*/
    /* get textbox position from effective candy center position */

    glm::vec3 wTPos = wCandy.getPosition() + wTZ.ToTopLeft;

//    glm::vec3 wTPos = vAdd(wCandy.getPosition() , wTZ.ToTopLeft);

//    fprintf (stdout,"*** wTpos <x:%f y:%f z:%f>  \n\n",
//             wTPos.x,wTPos.y,wTPos.z);


    wBText->setBoxGLDimensions(wTZ.Width,wTZ.Height);
    wBText->setBoxMarginsAll(5.0);
    wBText->setBoxBorderColor(ZYellowBright);
    wBText->setBoxBorderAlpha(1.0);
    wBText->setBoxBorderWidth(1.5);
    wBText->setBoxVisible(true);
    wBText->setBoxFill(true);
    wBText->setBoxBorder(true);

    wTPos.z += 0.001;
    wBText->setPosition(wTPos);

    wBText->setBoxFillAlpha(1.0);

//     wBText->setBoxFill(false);
    wBText->setTextFlag((uint16_t)RBP_Center|(uint16_t)RBP_WordWrap);
//     wBText->setBoxTexture("tissuegrey.jpeg");

/*    if (wBText->setBoxTextureByName("metal") < 0)
           {
           fprintf(stderr,"Failed to assign texture <%s> to text box \n","metal");
           exit (EXIT_FAILURE);
           }
    wBText->setBoxFlag(RBP_BoxVisible|RBP_BoxTexture|RBP_BoxShape);
    */
//     wBText->setBoxFlag(RBP_BoxVisible|RBP_BoxShape);
//    wBText->setBoxFillColor(ZBlueColor);


    _printTextBoxFlag(wBText->getBoxFlag());

    fprintf (stdout,"wBText -----Shader Context [Draw]----\n");
    wBText->printBoxShaderContext(Draw);
    fprintf (stdout,"wBText -----Shader Context [Shape]----\n");
    wBText->printBoxShaderContext(Shape);

    wBText->setupGL();


/*    fprintf (stdout,"*** Center <x:%f y:%f z:%f>  Width <%f> Height <%f> ToTopLeft  <x:%f y:%f z:%f>  \n"
                    " wCandy position <x:%f y:%f z:%f>  wTpos <x:%f y:%f z:%f>  \n\n",
             wTZ.Center.x,wTZ.Center.y,wTZ.Center.z,
             wTZ.Width,
             wTZ.Height,
             wTZ.ToTopLeft.x,wTZ.ToTopLeft.y,wTZ.ToTopLeft.z,
             wCandy.getPosition().x,wCandy.getPosition().y,wCandy.getPosition().z,
             wTPos.x,wTPos.y,wTPos.z);
*/


    Vertice_type wC1=Vertice_type(0.8f,0.9f,0.0f);

//    Vertice_type wC2=Vertice_type(0.8f,0.4f,0.0f);

    Vertice_type wC2=Vertice_type(0.7f,-0.1f,0.7f);


    Vertice_type wC3=Vertice_type(0.8f,0.8f,0.0f);

//    ZObject wCircle3=generate_Circle(ZGreyColor, wC3,0.2f,50,ZObject::Flat,ZObject::Top);

//    ZObject* wPipe = generateRegularCylinder(wC1,wC2,0.05f,50,ZObject::Flat,"Pipe");

    ZObject* wPipe = generateCylinderPointToPoint (wC1,wC2,0.05,50,"Pipe");

    wPipe->createMatrices(MAT_All); // idem to MAT_Model | MAT_View | MAT_Projection | MAT_Normal

    wPipe->createShaderContextByName(Draw,"TextureShader");
    wPipe->ShaderContext[Draw]->addBool(__SHD_BLINNPHONG_UN__,true);
    wPipe->ShaderContext[Draw]->addVec3(__SHD_LIGHT_POSITION_UN__,&camera.LightPosition);
    wPipe->ShaderContext[Draw]->addVec3(__SHD_VIEW_POSITION_UN__,&camera.CameraPosition);

    wPipe->ShaderContext[Draw]->setTextureByName("metal");

//    wPipe->ShaderContext[Draw]->addMaterial(&ZChrome);

    wPipe->createShaderContextByName(Shape,"LampShader");

    wPipe->ShaderContext[Shape]->addVec3(__SHD_DEFAULTCOLOR_UN__,&ZYellowSpecular);
    wPipe->ShaderContext[Shape]->setLineWidth(1.5f);

    wPipe->DrawFigure[Draw]=GL_TRIANGLES;

    wPipe->setupGL(Draw,CSO_setupAll | CSO_DeleteDataAfter );

    wPipe->setupGL(Shape,CSO_setupAll | CSO_DeleteDataAfter );

 //   wSphere.print(50); /* print 50 first data */





    // render loop
    // -----------

//    GLResources->listRegistratedShaders();





    while (!glfwWindowShouldClose(window))
    {
        // input
        // -----
        processInput(window);

        if (!camera.redrawRequested())
            {
            glfwPollEvents();
            continue;
            }

        if (camera.usePolygonLines)
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        else
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

        if (camera.useGammaCorrection)
                glEnable(GL_FRAMEBUFFER_SRGB);
        else {
                glDisable(GL_FRAMEBUFFER_SRGB);
        }

        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

#ifdef __USE_STENCIL__
        /* using stencil buffer to pick up click on object */
        glClearStencil(0); // this is the default value

        glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT|GL_STENCIL_BUFFER_BIT);

#else
        glClear(GL_COLOR_BUFFER_BIT| GL_DEPTH_BUFFER_BIT);
#endif




        wBText->renderToBox(ZBlue1Color);

//        wNLText->renderToBox(ZGreyColor);
//        w90Text->renderToBoxVertical(ZBlueColor);

        // render lamp object

/* Object position and matrices are not contextual to shader (not part of ZShaderContext) */

        wLamp->setPosition(camera.LightPosition);
        wLamp->setModelRotation((float)glfwGetTime(),glm::vec3(1.0f, 0.5f, 0.2f));

        wLamp->drawGLByContext(Draw);
        wLamp->drawGLByContext(Shape);

//#define __TEXT_ONLY__

#ifdef __TEXT_ONLY__

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);

        camera.cancelRedraw();

        glfwPollEvents();
        continue;
#endif //__TEXT_ONLY__



        // render objects
        // ------

/*
 Common matrices :

    View matrix is common to all objects
    Projection matrix is common to all objects
    Normal matrix is computed from matrix product (camera Model * camera View) matrices  (modelView)
    Normal formula --> mat4 normalMatrix = transpose(inverse(modelView));

    Normal matrix is common to all objects

Per object matrix:

    Model matrix is computed object per object
*/
        // camera/view transformation
//        glm::mat4 mView = camera.GetViewMatrix();
        // pass projection matrix to shaders (note that in this case it could change every frame)
//        glm::mat4 mProjection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        /* compute normal matrix */
//        glm::mat4 mNormal = camera.GetViewMatrix()*camera.getModel();
//        mNormal = glm::transpose(glm::inverse(mNormal));

/* light color parameters */
 //       __COLOR_SHADER__.setLight(ZLight(ZYellowAmbient,ZYellowDiffuse,ZYellowSpecular));



//        wCandy.setUseDefaultColorAll(false);
//        wCandy.setUseDefaultAlphaAll(false);

//        wCandy.setupShaderMaterial();

// calculate the model matrix for each object and pass it to shader before drawing
//        glm::mat4 model = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
//        model = glm::translate(model, objectLocation);
//        if (camera.RotationAngle > 0.0f) /* if any rotation requested */
//                model = glm::rotate(model, (float)camera.RotationAngle, camera.RotationAxis);

        wCandy.drawGLByContext(Draw);
//        wCandy.drawGLByContext(Shape);


/*        lampShader.use();
        lampShader.setMat4("mModel", mModel);
        lampShader.setMat4("mProjection", mProjection);
//        lampShader.setMat4("mNormal", mNormal);
        lampShader.setMat4("mView", mView);
        lampShader.setVec3("DefaultColor", ZGreySilver);
//        wCandy.drawGLShape(&lampShader);

 //       wText.renderText("This is a new sample text",-0.6f,-0.6f,ZBlueColor,0);
*/

        if (camera.drawNormalVectors)
            {

//            wCandy.NormVisuShader->setVec3("DefaultColor",ZYellowSpecular);
//            wCandy.setDefaultColor(ZYellowSpecular);
//            lampShader.setVec3("DefaultColor",ZYellowSpecular);

            /* Everything is already parametrized within ShaderContext rules & Matrices for [NormVisu] */
            wCandy.drawGLByContext(NormVisu);
            }

        wPipe->drawGLByContext(Draw);
        wSphere.drawGLByContext(Draw);


  //      wText.RenderText("This is sample text", 25.0f, 25.0f, 1.0f, glm::vec3(0.5, 0.8f, 0.2f));

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);

        camera.cancelRedraw();

        glfwPollEvents();

    }//while (!glfwWindowShouldClose(window))

    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------
/*    glDeleteVertexArrays(1, &BoxVAO);
    glDeleteBuffers(1, &BoxVBO);
//    glDeleteBuffers(1, &ObjectEBO);
    glDeleteBuffers(1, &BoxNormVBO);

    glDeleteVertexArrays(1, &lampVAO);
    glDeleteBuffers(1, &lampVBO);
    glDeleteBuffers(1, &lampEBO);
*/



    GLResources->listTextures();

    GLResources->listShaders();

    GLResources->listObjects();

    delete wLamp;
    delete wPipe;

    GLResources->cleanAll(); /* clean all objects' OpenGL contexts before quitting */

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}

float accelerator=0.001f;

// ---------------------------------------------------------------------------------------------------------
// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------

struct ZAntiRebounce{
    int         LastKey;
    double      LastTime;
} AntiRebounce = {0 , 0.0};
const double          AntiRebounceDelay=1.0; /* 0.5 second delay for anti-rebounce */
bool testRebounce (int pKey)
{
    double wTime=glfwGetTime();
    if (pKey== AntiRebounce.LastKey)
        {
        if ((wTime-AntiRebounce.LastTime) < AntiRebounceDelay)
                {
//                AntiRebounce.LastTime=wTime;
                return false;
                }
        }
    AntiRebounce.LastKey=pKey;
    AntiRebounce.LastTime=wTime;
    return true;
}

bool testKeyNoRebound(GLFWwindow *window,int pKey)
{
   if (glfwGetKey(window, pKey) == GLFW_PRESS)
    {
//       return true;
       return testRebounce(pKey);
    }
   return false;
}
void resetKeyEntered()
{
    AntiRebounce.LastKey=0;
    AntiRebounce.LastTime=0;
}
inline bool testKeyEntered(GLFWwindow *window,int pKey)
{
   return (glfwGetKey(window, pKey) == GLFW_PRESS);
}

#define _TESTKEY_(__Key__) testKeyEntered(window,__Key__)

#define _TESTKEY_NOREBOUND_(__Key__) testKeyNoRebound(window,__Key__)

void processInput(GLFWwindow *window)
{

    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);


    if (glfwGetKey(window, GLFW_MOD_SHIFT)==GLFW_PRESS)
            {
            camera.setShiftOn();
            }
    if (glfwGetKey(window, GLFW_MOD_SHIFT)==GLFW_RELEASE)
            {
            camera.setShiftOff();
            }

    /* KeyPad actions */

    if (_TESTKEY_(GLFW_KEY_KP_7))
                {
                camera.rotateModel(-0.05,glm::vec3(1.0f,1.0f,0.0f));
                camera.scheduleRedraw();
                return;
                }
    if (_TESTKEY_(GLFW_KEY_KP_9))
                {
                camera.rotateModel(0.05,glm::vec3(1.0f,1.0f,0.0f));
                camera.scheduleRedraw();
                return;
                }
    if (_TESTKEY_(GLFW_KEY_KP_1))
                {
                camera.rotateModel(-0.05,glm::vec3(1.0f,-1.0f,0.0f));
                camera.scheduleRedraw();
                return;
                }
    if (_TESTKEY_(GLFW_KEY_KP_3))
                {
                camera.rotateModel(0.05,glm::vec3(1.0f,-1.0f,0.0f));
                camera.scheduleRedraw();
                return;
                }
    if (_TESTKEY_(GLFW_KEY_KP_8))
                {
                camera.rotateModel(0.05,glm::vec3(1.0f,0.0f,0.0f));
                camera.scheduleRedraw();
                return;
                }
    if (_TESTKEY_( GLFW_KEY_KP_2))
                {
                camera.rotateModel(-0.05,glm::vec3(1.0f,0.0f,0.0f));
                camera.scheduleRedraw();
                return;
                }
    if (_TESTKEY_(GLFW_KEY_KP_6))
                {
                camera.rotateModel(+0.05,glm::vec3(0.0f,1.0f,0.0f));
                camera.scheduleRedraw();
                return;
                }
    if (_TESTKEY_( GLFW_KEY_KP_4))
                {
                camera.rotateModel(-0.05,glm::vec3(0.0f,1.0f,0.0f));
                camera.scheduleRedraw();
                return;
                }
     if ((_TESTKEY_(GLFW_KEY_KP_5))||
        (_TESTKEY_(GLFW_KEY_TAB)))
        {
            camera.RotationAngle=0.0f;
            camera.RotationAxis=glm::vec3(0.0f);
            camera.Rotation=false;
            camera.setModelIdentity();
            return;
        }

/* Keyboard actions */

    /* camera movements */
    if (_TESTKEY_( GLFW_KEY_X) )
        {
        camera.ProcessKeyboard(FORWARD, accelerator);
        return;
        }
    if (_TESTKEY_( GLFW_KEY_C) )
        {
        camera.ProcessKeyboard(BACKWARD, accelerator);
        return;
        }
    if (_TESTKEY_( GLFW_KEY_A) )
        {
        camera.ProcessKeyboard(LEFT, accelerator);
        return;
        }
    if (_TESTKEY_( GLFW_KEY_D) )
        {
        camera.ProcessKeyboard(RIGHT, accelerator);
        return;
        }
    if (_TESTKEY_(GLFW_KEY_W) )
        {
        camera.ProcessKeyboard(UP, accelerator);
        return;
        }
    if (_TESTKEY_(GLFW_KEY_S) )
        {
        camera.ProcessKeyboard(DOWN, accelerator);
        return;
        }

    /* light movement */

    if (_TESTKEY_(GLFW_KEY_SCROLL_LOCK) )
        {
        camera.ProcessKeyboard(LIGHTHIGH, accelerator);
        return;
        }
    if (_TESTKEY_( GLFW_KEY_END) )
        {
        camera.ProcessKeyboard(LIGHTLOW, accelerator);
        return;
        }
    if (_TESTKEY_( GLFW_KEY_INSERT) )
        {
        camera.ProcessKeyboard(LIGHTLEFT, accelerator);
        return;
        }
    if (_TESTKEY_( GLFW_KEY_PAGE_UP) )
        {
        camera.ProcessKeyboard(LIGHTRIGHT, accelerator);
        return;
        }
    if (_TESTKEY_( GLFW_KEY_PAUSE) )
        {
        camera.ProcessKeyboard(LIGHTBACKWARD, accelerator);
        return;
        }
    if (_TESTKEY_( GLFW_KEY_PAGE_DOWN ))
        {
        camera.ProcessKeyboard(LIGHTFORWARD, accelerator);
        return;
        }
    if (_TESTKEY_( GLFW_KEY_DELETE ))
        {
        camera.ProcessKeyboard(LIGHTORIGIN, accelerator);
        return;
        }


    /* toggle keys */


    if (_TESTKEY_NOREBOUND_(GLFW_KEY_M ))   /* M for toggle mouse tracking on / off  */
        {
        camera.ProcessKeyboard(TOGGLE_MOUSE, accelerator);
        if (CurrentCursor==HandCursor)
                    CurrentCursor = DefaultCursor;
                else
                    CurrentCursor = HandCursor;

        glfwSetCursor(window,CurrentCursor) ;
        return;
        }

    if (_TESTKEY_NOREBOUND_(GLFW_KEY_L ))   /* L for toggle polygon lines */
        {
        camera.ProcessKeyboard(TOGGLE_POLYGONLINES, accelerator);
        return;
        }
    if (_TESTKEY_NOREBOUND_(GLFW_KEY_N ))   /* N for toggle normal vectors */
        {
        camera.ProcessKeyboard(TOGGLE_NORMALVECTORS, accelerator);
        return;
        }

    if (_TESTKEY_NOREBOUND_(GLFW_KEY_G ))   /* G for toggle gamma correction */
        {
        camera.ProcessKeyboard(TOGGLE_GAMMA, accelerator);
        return;
        }

/* special keys */
    if (_TESTKEY_( GLFW_KEY_HOME ))   /* Home resets camera position */
        {
        camera.ProcessKeyboard(RESET_POSITION, accelerator);
        return;
        }

//    resetKeyEntered();

    camera.resetKeyAccelerator();
    return;
}//processInput

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}


// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{

    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top


    lastX = xpos;
    lastY = ypos;

    camera.ProcessMouseMovement(xoffset, yoffset);
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(yoffset);
}
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS && ((mods & GLFW_MOD_SHIFT)==GLFW_MOD_SHIFT ) )
        {
        camera.Rotation=true;
        return;
        }
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE  )
    {
        camera.Rotation=false;
        return;
//        camera.RotationAxis=glm::vec3(0.0f);
    }
}


