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

#include <zresource.h>

#include <zobject.h>
#include <zobjectfunctions.h>

#include <zcandy.h>

#include <zsphere.h>
#include <ztexture.h>

#include <ztextrenderer.h>
#include <zgltext.h>

#define __CANDY_SHADER__    colorShader
#define __COLOR_SHADER__    colorShader
#define __SPHERE_SHADER__   colorShader
//#define __SPHERE_SHADER__   textureShader
#define __PIPE_SHADER__     colorShader

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow *window);

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);

// settings
const unsigned int SCR_WIDTH = 1600;
const unsigned int SCR_HEIGHT = 1200;

// camera
Camera camera(Vertice_type(0.0f, 0.0f, 3.0f),glm::vec3(1.0f, 0.2f, 1.0f));
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

    // During init, enable debug output
    glEnable              ( GL_DEBUG_OUTPUT );

    GLAbortOnFailure=true;
    glDebugMessageCallback( (GLDEBUGPROC) MessageCallback, &GLAbortOnFailure );


//=================================Objects Vertices creation ============================================
    const float wHigh=0.5f;
    const float wWidth=0.5f;
    const float wDepth=0.2f;


    ZCandy wCandy = generateCandy(wHigh,wWidth,wDepth,true,"Candy");

    Vertice_type wC1=Vertice_type(0.8f,0.9f,0.0f);

    Vertice_type wC2=Vertice_type(0.8f,0.4f,0.0f);

    Vertice_type wC3=Vertice_type(-0.8f,0.8f,0.0f);

//    ZObject wCircle3=generate_Circle(ZGreyColor, wC3,0.2f,50,ZObject::Flat,ZObject::Top);

    ZObject* wPipe = generateRegularCylinder(ZGreyColor,wC1,wC2,0.05f,50,ZObject::Flat,"Pipe");

    ZSphere wSphere(0.10f,18,9,true, "Sphere");

//    wSphere.print(20); /* list 20 max element of ZObject */

//=============================================================================

    // configure global opengl state
    // -----------------------------
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
//    glEnable( GL_POLYGON_SMOOTH );


    // Define the viewport dimensions
    glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);


    ZTextRenderer wText(GL_TEXTURE0);

    ZGLText wTextArchi(GLResources->getGLWindowSize().x,
                             GLResources->getGLWindowSize().y,
                             GL_TEXTURE0);
    ZGLText wTextFT(GLResources->getGLWindowSize().x,
                          GLResources->getGLWindowSize().y,
                          GL_TEXTURE0);
//    wText.LoadFont("FreeSans.ttf", 12);

    wText.addFont("FreeSans.ttf",12,"AtlasFreeSans12");

    wText.addFont("FreeSans.ttf",48,"AtlasFreeSans48");

    wText.addFont("FreeSans.ttf",24,"AtlasFreeSans24");

    wText.addFont("DroidSansMono.ttf",24,"DroidSansMono24");

    wText.addFont("Architex.ttf",24,"AtlasArchitex24");

    wText.addFont("SCRIPTIN.ttf",48,"AtlasScriptina48");

    wTextArchi.LoadFont("Architex.ttf",48,"Architex48");
    wTextFT.LoadFont("FreeSans.ttf",48,"FreeSans48");

    // build and compile our shader zprogram
    // ------------------------------------
//    ZShader lightingShader(Resources.getShaderPath("zbasic_lighting.vs").c_str(), Resources.getShaderPath("zbasic_lighting.fs").c_str());
    ZBoxComponents wLampComponents;
    ZObject* wLamp =boxSetup(0.05f,0.05f,0.05f,wLampComponents,"Lamp"); /* same shape as wBox (but scaled down ) */
    wLampComponents.generateShape(*wLamp);

//    wLamp->print(20);


    ZShader lampShader("zlamp.vs", "zlamp.fs", "LampShader");
    /* process lamp object */

    wLamp->setDefaultColor(ZYellowBright);
    wLamp->setDefaultAlpha(1.0f);
    wLamp->setupGL(&lampShader,
                   ZObject::setupVertices,
                   GL_TRIANGLES);

    lampShader.use();
    lampShader.setVec3("DefaultColor",ZYellowBright);

    wLamp->setupGLShape(&lampShader);


 //   ZShader sphereShader("zsphere.vs", "zsphere.fs","SphereShader");

    ZShader textureShader("zlighting.vs", "ztexture.fs","TextureShader");

    ZShader colorShader("zlighting.vs", "zcolor.fs","ColorShader");

    ZShader materialShader("zlighting.vs", "zmaterial.fs","MaterialShader");

    // lamp object
    camera.setLightPosition(glm::vec3(1.0f, 0.2f, 1.0f));


//==============GL Objects setup======================================

/* process box */
//    unsigned int BoxVBO, BoxVAO, BoxNormVBO;/* Box GL identifiers */    glBindVertexArray(VAO);

    ZTexture wTexWoodFloor("wood.png",GL_TEXTURE1);

    ZTexture wTexTissueGrey("tissuegrey.jpeg",GL_TEXTURE1);
    ZTexture wTexTissueBluePale("tissueblue.png",GL_TEXTURE1);
    ZTexture wTexTissueBrown("tissuebrownbure.jpeg",GL_TEXTURE1);
    ZTexture wTexMetal("metal.png",GL_TEXTURE1);

    ZTexture wTexMoon("moon1024.bmp",GL_TEXTURE1);
    ZTexture wTexEarth("earth2048.bmp",GL_TEXTURE1);

    wCandy.setupGL(&__CANDY_SHADER__,
                   ZObject::setupAll,
                   &wTexWoodFloor);

    wCandy.setMaterialAll(ZChrome);
    wCandy.setDefaultColorAll(ZBlueColor);
    wCandy.setDefaultAlphaAll(0.5f);

    wCandy.setUseTextureAll(false);


    wCandy.setupGLNormalVisu(&lampShader);
    wCandy.setupGLShape(&lampShader);

/* Pipe GL set-up */
    wPipe->setDefaultPosition(Vertice_type(0.5f,0.5f,0.5f));
    wPipe->setDefaultColor(ZGreyColor);
    wPipe->setDefaultAlpha(0.5f);
    wPipe->setComputeNormals(true);
    wPipe->setComputeTexCoords(true);
    wPipe->setupGL(&__PIPE_SHADER__,
                   ZObject::setupAll,
                   GL_TRIANGLES,
                  &wTexMetal);

    wSphere.setDefaultPosition(Vertice_type(-0.8f,0.5f,0.0f));
    wSphere.setDefaultColor(ZBlueColor);
    wSphere.setDefaultAlpha(0.5f);
    wSphere.setMaterial(ZEmerald);
    wSphere.setComputeNormals(true); /* normals are given by creation algo */
    wSphere.setComputeTexCoords(false);/* texture coords are given by creation algo */

    wSphere.setupGL(&__SPHERE_SHADER__,
                    ZObject::setupAll,
                    GL_TRIANGLES,
                    &wTexEarth);


 //   wSphere.print(50); /* print 50 first data */

    // render loop
    // -----------

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
        glClear(GL_COLOR_BUFFER_BIT| GL_DEPTH_BUFFER_BIT);

//        wText.RenderText("This is sample text", 25.0f, 25.0f, 1.0f, glm::vec3(0.5, 0.8f, 0.2f));


        wText.renderTextByName("This is a sample text AtlasScriptina48",
                               -0.8f,0.2f,
                               ZBlueColor,
                               "AtlasScriptina48");


        wText.renderTextByName("abcdefghijklmnopqrstuvwxyz ABCDEFGHIJKLMN AtlasArchitex48",
                               -0.8f,0.8f,
                               ZBlueColor,
                               "AtlasFreeSans48");


        wText.renderTextByName("abcdefghijklmnopqrstuvwxyz ABCDEFGHIJKLMN AtlasFreeSans24",
                               -0.9f,0.0f,
                               ZRedMedium,
                               "AtlasFreeSans24");


        wText.renderTextByName("abcdefghijklmnopqrstuvwxyz ABCDEFGHIJKLMN DroidSansMono24",-0.9f,-0.2f,ZYellowSpecular,"DroidSansMono24");


        glm::mat4 textModel = glm::mat4(1.0f);
        textModel =  glm::translate(camera.getModel(), glm::vec3(0.0f, 0.0f, 0.0f));
        textModel = glm::rotate(textModel,glm::radians(180.0f),glm::vec3(1.0f, 0.0f, 0.0f)); /* to do : avoid rotation */

        glm::mat4 textView = camera.GetViewMatrix();
        glm::mat4 textProjection = glm::perspective(glm::radians(camera.Zoom),
                                                    (float)SCR_WIDTH / (float)SCR_HEIGHT,
                                                    0.1f,
                                                    100.0f);
        wTextArchi.TextShader->use();
        wTextArchi.TextShader->setMat4("mModel", textModel);
        wTextArchi.TextShader->setMat4("mView", textView);
        wTextArchi.TextShader->setMat4("mProjection", textProjection);

//        glm::mat4 textProjection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);

//        wTextArchi.TextShader->setMat4("mProjection", textProjection);

        wTextArchi.render("New more longer text with s letter rendered.",
                           -0.6f,-0.4f,
                           1.0f,
                           ZRedMedium);

        wTextFT.TextShader->use();
        wTextFT.TextShader->setMat4("mModel", textModel);
        wTextFT.TextShader->setMat4("mView", textView);
        wTextFT.TextShader->setMat4("mProjection", textProjection);
//        textModel =  glm::translate(glm::mat4(1.0f), glm::vec3(-0.8f, -0.2f, 0.2f));
//        textModel = glm::mat4(1.0f);
//        wTextFT.TextShader->setMat4("mModel", textModel);
        wTextFT.render("New text with gpwhdlq letter rendered.",
                           0.0f,0.0f,
                           1.0f,
                           ZBlueColor);


        wText.renderTextByName("This is a sample text THIS IS A SAMPLE TEXT 1234567890  FreeSans12",
                               -0.2f,-0.8f,
                               ZWhiteColor,
                               "AtlasFreeSans12");


        // render lamp object
        lampShader.use();

        glm::mat4 lampModel = glm::mat4(1.0f);
        lampModel = glm::translate(lampModel, camera.LightPosition);
        lampModel = glm::rotate(lampModel,(float)glfwGetTime(),glm::vec3(1.0f, 0.5f, 0.2f));

        glm::mat4 lampView = camera.GetViewMatrix();
        glm::mat4 lampProjection = glm::perspective(glm::radians(camera.Zoom),
                                                    (float)SCR_WIDTH / (float)SCR_HEIGHT,
                                                    0.1f,
                                                    100.0f);

        /* remark : no normal matrix for lampShader */
        lampShader.setMat4("mProjection", lampProjection);
        lampShader.setMat4("mView", lampView);
        lampShader.setMat4("mModel", lampModel);
        lampShader.setVec3("DefaultColor", ZYellowBright);

        wLamp->drawGL(&lampShader,GL_TRIANGLES);

        lampShader.setVec3("DefaultColor", ZGreySilver);

        wLamp->drawGLShape(&lampShader);

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
        glm::mat4 mView = camera.GetViewMatrix();
        // pass projection matrix to shaders (note that in this case it could change every frame)
        glm::mat4 mProjection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        /* compute normal matrix */
        glm::mat4 mNormal = camera.GetViewMatrix()*camera.getModel();
        mNormal = glm::transpose(glm::inverse(mNormal));

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


        glm::mat4 mModel = glm::translate(camera.getModel(), wCandy.DefaultPosition);

        __CANDY_SHADER__.use();

        __CANDY_SHADER__.setInt("TextureSampler",wCandy.getTexture()->getTextureEngineNumber());
        __CANDY_SHADER__.setBool ("BlinnPhong",false);

        __CANDY_SHADER__.setMat4("mProjection", mProjection);
        __CANDY_SHADER__.setMat4("mNormal", mNormal);
        __CANDY_SHADER__.setMat4("mView", mView);

        __CANDY_SHADER__.setMat4("mModel", mModel);

        __CANDY_SHADER__.setVec3("light.Position",camera.LightPosition);

        __CANDY_SHADER__.setVec3("viewPosition", camera.CameraPosition);

        __CANDY_SHADER__.setVec3("DefaultColor", ZBlueColor);
        __CANDY_SHADER__.setFloat("DefaultAlpha", 0.2f);
        __CANDY_SHADER__.setBool("UseDefaultAlpha", true);

        __CANDY_SHADER__.setMaterial(ZSilver);

        wTexWoodFloor.bind();
 //       wCandy.drawGL(&__CANDY_SHADER__);

        lampShader.use();
        lampShader.setMat4("mModel", mModel);
        lampShader.setMat4("mProjection", mProjection);
//        lampShader.setMat4("mNormal", mNormal);
        lampShader.setMat4("mView", mView);
        lampShader.setVec3("DefaultColor", ZGreySilver);
//        wCandy.drawGLShape(&lampShader);

 //       wText.renderText("This is a new sample text",-0.6f,-0.6f,ZBlueColor,0);


        if (camera.useNormalVectors)
            {
            lampShader.setVec3("DefaultColor",ZYellowSpecular);
            wCandy.drawGLNormalVisu(&lampShader);
            }
/*
        glfwSwapBuffers(window);

        camera.cancelRedraw();

        glfwPollEvents();
        continue;
*/


        __PIPE_SHADER__.use();
        mModel = glm::translate(camera.getModel(), wPipe->DefaultPosition);
        __PIPE_SHADER__.setMat4("mModel", mModel);

        __PIPE_SHADER__.setMat4("mProjection", mProjection);
        __PIPE_SHADER__.setMat4("mNormal", mNormal);
        __PIPE_SHADER__.setMat4("mView", camera.GetViewMatrix());

        __PIPE_SHADER__.setVec3("light.Position",camera.LightPosition);

        __PIPE_SHADER__.setVec3("viewPosition", camera.CameraPosition);

        __PIPE_SHADER__.setVec3("DefaultColor", ZGreyColor);
        __PIPE_SHADER__.setFloat("DefaultAlpha", 0.5f);

        wPipe->setUseTexture(false);
        wPipe->setUseDefaultColor(true);
        wPipe->setUseDefaultAlpha(true);
        wPipe->setDefaultColor(ZGreyColor);
        wPipe->setDefaultAlpha(0.5f);

//        wPipe->drawGL(&__PIPE_SHADER__,GL_TRIANGLES);

/* sphere */

        __SPHERE_SHADER__.use();

        glm::mat4 mSphereModel=glm::translate(camera.getModel(), wSphere.DefaultPosition);
 //       mSphereModel=glm::rotate(mSphereModel,(float)glm::radians(270.0f),glm::vec3(1.0,0.0,0.0));

 //       mSphereModel=glm::rotate(mSphereModel,(float)glfwGetTime(),glm::vec3(0.0,0.0,1.0));

        __SPHERE_SHADER__.setMat4("mModel", mSphereModel);

        __SPHERE_SHADER__.setMat4("mProjection", mProjection);

 //       mNormal=glm::rotate(mNormal,(float)glfwGetTime(),glm::vec3(0.0,0.0,1.0));


        __SPHERE_SHADER__.setMat4("mNormal", mNormal);
        __SPHERE_SHADER__.setMat4("mView", camera.GetViewMatrix());

        __SPHERE_SHADER__.setVec3("light.Position",camera.LightPosition);

        __SPHERE_SHADER__.setVec3("viewPosition", camera.CameraPosition);

        __SPHERE_SHADER__.setVec3("DefaultColor", ZBlueColor);
        __SPHERE_SHADER__.setFloat("DefaultAlpha", 0.5f);

        __SPHERE_SHADER__.setMaterial(ZGold);

        __SPHERE_SHADER__.setInt("TextureSampler",wSphere.getTexture()->getTextureEngineNumber());
        __SPHERE_SHADER__.setBool ("BlinnPhong",false);

//        __SPHERE_SHADER__.setFloat("material.DiffuseAlpha",1.0f);


//        wSphere.setupShaderMaterial(&__COLOR_SHADER__);

        wSphere.setUseTexture(false);
        wSphere.setUseDefaultColor(false);
        wSphere.setUseDefaultAlpha(false);
        wTexEarth.bind();
//        wSphere.drawGL(&__SPHERE_SHADER__,GL_TRIANGLES);



        glBindVertexArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

        glBindTexture(GL_TEXTURE_2D, 0);

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


