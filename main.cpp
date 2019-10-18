#include <glad/glad.h>
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

#define __CANDY_SHADER__  textureShader
#define __COLOR_SHADER__    colorShader
#define __SPHERE_SHADER__   textureShader
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

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        glfwTerminate();
        abort();
    }
    printf("Using OpenGL version %d.%d\n", GLVersion.major, GLVersion.minor);
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

    wSphere.print(20); /* list 20 max element of ZObject */

//=============================================================================

    // configure global opengl state
    // -----------------------------
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
//    glEnable( GL_POLYGON_SMOOTH );

    // build and compile our shader zprogram
    // ------------------------------------
//    ZShader lightingShader(Resources.getShaderPath("zbasic_lighting.vs").c_str(), Resources.getShaderPath("zbasic_lighting.fs").c_str());
    ZBoxComponents wLampComponents;
    ZObject* wLamp =boxSetup(0.05f,0.05f,0.05f,wLampComponents,"Lamp"); /* same shape as wBox (but scaled down ) */
    wLampComponents.generateShape(*wLamp);

    printf ("Lamp Indices\n");
//    wLamp->print(20);


    ZShader lampShader("zlamp.vs", "zlamp.fs", "LampShader");
    /* process lamp object */

    wLamp->setDefaultColor(ZYellowBright);
    wLamp->setDefaultAlpha(1.0f);
    wLamp->setupGL(&lampShader,
                   ZObject::setupVertices,
                   GL_TRIANGLES);

    lampShader.setVec3("DefaultColor",ZYellowBright);

    wLamp->setupGLShape(&lampShader);


 //   ZShader sphereShader("zsphere.vs", "zsphere.fs","SphereShader");

    ZShader textureShader("zlighting.vs", "ztexture.fs","TextureShader");

    ZShader colorShader("zlighting.vs", "zcolor_lighting.fs","ColorShader");

    ZShader materialShader("zlighting.vs", "zmaterial_lighting.fs","MaterialShader");

    // lamp object
    camera.setLightPosition(glm::vec3(1.0f, 0.2f, 1.0f));


//==============GL Objects setup======================================

/* process box */
//    unsigned int BoxVBO, BoxVAO, BoxNormVBO;/* Box GL identifiers */

    ZTexture wTexWoodFloor("wood.png");

    ZTexture wTexTissueGrey("tissuegrey.jpeg");
    ZTexture wTexTissueBluePale("tissueblue.png");
    ZTexture wTexTissueBrown("tissuebrownbure.jpeg");
    ZTexture wTexMetal("metal.png");

    ZTexture wTexMoon("moon1024.bmp");
    ZTexture wTexEarth("earth2048.bmp");

    wCandy.setupGL(&__CANDY_SHADER__,
                   ZObject::setupAll,
                   &wTexWoodFloor);

    wCandy.setMaterialAll(ZChrome);
    wCandy.setDefaultColorAll(ZBlueColor);
    wCandy.setDefaultAlphaAll(0.5f);

    wCandy.setUseTextureAll(true);


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
    wSphere.setComputeNormals(false); /* normals are given by creation algo */
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

        // render lamp object
        lampShader.use();

        glm::mat4 lampModel = glm::mat4(1.0f);
        lampModel = glm::translate(lampModel, camera.LightPosition);
        lampModel = glm::rotate(lampModel,(float)glfwGetTime(),glm::vec3(1.0f, 0.5f, 0.2f));

        glm::mat4 lampView = camera.GetViewMatrix();

        glm::mat4 lampProjection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);

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

        __CANDY_SHADER__.setInt("TextureSampler",0);
        __CANDY_SHADER__.setBool ("BlinnPhong",true);

        __CANDY_SHADER__.setMat4("mProjection", mProjection);
        __CANDY_SHADER__.setMat4("mNormal", mNormal);
        __CANDY_SHADER__.setMat4("mView", mView);

        __CANDY_SHADER__.setMat4("mModel", mModel);

        __CANDY_SHADER__.setVec3("light.Position",camera.LightPosition);

        __CANDY_SHADER__.setVec3("viewPosition", camera.CameraPosition);

        __CANDY_SHADER__.setVec3("DefaultColor", ZBlueColor);
        __CANDY_SHADER__.setFloat("DefaultAlpha", 0.5f);
        wTexWoodFloor.bind();
        wCandy.drawGL(&__CANDY_SHADER__);

        lampShader.use();
        lampShader.setMat4("mModel", mModel);
        lampShader.setMat4("mProjection", mProjection);
//        lampShader.setMat4("mNormal", mNormal);
        lampShader.setMat4("mView", mView);
        lampShader.setVec3("DefaultColor", ZGreySilver);
//        wCandy.drawGLShape(&lampShader);

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

        wPipe->drawGL(&__PIPE_SHADER__,GL_TRIANGLES);

/* sphere */

        __SPHERE_SHADER__.use();

        glm::mat4 mSphereModel=glm::translate(camera.getModel(), wSphere.DefaultPosition);
        mSphereModel=glm::rotate(mSphereModel,(float)glm::radians(270.0f),glm::vec3(1.0,0.0,0.0));

        mSphereModel=glm::rotate(mSphereModel,(float)glfwGetTime(),glm::vec3(0.0,0.0,1.0));

        __SPHERE_SHADER__.setMat4("mModel", mSphereModel);

        __SPHERE_SHADER__.setMat4("mProjection", mProjection);

        mNormal=glm::rotate(mNormal,(float)glfwGetTime(),glm::vec3(0.0,0.0,1.0));


        __SPHERE_SHADER__.setMat4("mNormal", mNormal);
        __SPHERE_SHADER__.setMat4("mView", camera.GetViewMatrix());

        __SPHERE_SHADER__.setVec3("light.Position",camera.LightPosition);

        __SPHERE_SHADER__.setVec3("viewPosition", camera.CameraPosition);

        __SPHERE_SHADER__.setVec3("DefaultColor", ZBlueColor);
        __SPHERE_SHADER__.setFloat("DefaultAlpha", 0.5f);

        __SPHERE_SHADER__.setMaterial(ZGold);

        __SPHERE_SHADER__.setInt("TextureSampler",0);
        __SPHERE_SHADER__.setBool ("BlinnPhong",true);

//        __SPHERE_SHADER__.setFloat("material.DiffuseAlpha",1.0f);


//        wSphere.setupShaderMaterial(&__COLOR_SHADER__);

        wSphere.setUseTexture(false);
        wSphere.setUseDefaultColor(false);
        wSphere.setUseDefaultAlpha(false);
        wTexEarth.bind();
        wSphere.drawGL(&__SPHERE_SHADER__,GL_TRIANGLES);

        glBindVertexArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);

        camera.cancelRedraw();

        glfwPollEvents();
    }

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


