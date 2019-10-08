#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/perpendicular.hpp>

#include <camera.h>
#include <zshader.h>

#include <zmaterial.h>

#define __USE_ZOBJECT__

#include <zresource.h>

#include <zobject.h>
#include <zobjectfunctions.h>

#include <zsphere.h>
#include <ztexture.h>

#define __SHADER__  lightingShader

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


const Vertice_type lightPosition = Vertice_type(0.0f, 0.0f, 1.0f);

const glm::vec3 lightAmbient = glm::vec3(0.3f, 0.3f, 0.3f);
const glm::vec3 lightDiffuse  = glm::vec3(0.7f, 0.7f, 0.7f);
const glm::vec3 lightSpecular = glm::vec3(1.0f, 1.0f, 1.0f);

Vertice_type objectLocation=glm::vec3(0.0f, 0.0f, 0.0f);


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


    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

//=================================Objects Vertices creation ============================================

    ZObject wBox ("OpenBox");
    ZObject wLamp("Lamp");

    const float wHigh=0.5f;
    const float wWidth=0.5f;
    const float wDepth=0.2f;

//    zbs::ZArray<Vertice_type> wMids=boxIndexSetup(&wBox,ZBlueColor,wHigh,wWidth,wDepth);

    zbs::ZArray<Vertice_type> wMids;
    wBox=openboxSetup(wHigh,wWidth,wDepth,&wMids,"Openbox");

    wLamp=boxIndexSetup(wHigh,wWidth,wDepth,nullptr,"Lamp"); /* same shape as wBox (but scaled down ) */

    float wRadius= wHigh/2.0f;

    ZObject wArcFL = generate_Arc(wMids[0],wRadius,10,ZObject::DirLeft,ZObject::Front,"ArcFL");  /* generate arc reverse (left) */
    ZObject wArcBL = generate_Arc(wMids[1],wRadius,10,ZObject::DirLeft,ZObject::Back,"ArcBL");/* generate arc reverse (left) */

    ZObject wArcFR = generate_Arc(wMids[2],wRadius,10,ZObject::DirRight,ZObject::Front,"ArcFR");/* generate front arc forward (right) */
    ZObject wArcBR = generate_Arc(wMids[3],wRadius,10,ZObject::DirRight,ZObject::Back,"ArcBR");/* generate bottom arc forward (right) */

//    ZObject wArcStripsLeft = generate_ArcStripsLeft(wArcFL,wArcBL);

    ZObject wArcStripsLeft = generate_ArcStripsLeft(wArcFL,wArcBL,"ArcStripLeft");
    ZObject wArcStripsRight = generate_ArcStripsRight(wArcFR,wArcBR,"ArcStripRight");

    Vertice_type wC1=Vertice_type(0.8f,0.9f,0.0f);

    Vertice_type wC2=Vertice_type(0.8f,0.4f,0.0f);

    Vertice_type wC3=Vertice_type(-0.8f,0.8f,0.0f);

//    ZObject wCircle3=generate_Circle(ZGreyColor, wC3,0.2f,50,ZObject::Flat,ZObject::Top);

    ZObject wPipe = generateRegularCylinder(ZGreyColor,wC1,wC2,0.05f,50,ZObject::Flat,"Pipe");

    ZSphere wSphere(0.10f,18,9,false, "Sphere");



//=============================================================================

    // configure global opengl state
    // -----------------------------
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // build and compile our shader zprogram
    // ------------------------------------
//    ZShader lightingShader(Resources.getShaderPath("zbasic_lighting.vs").c_str(), Resources.getShaderPath("zbasic_lighting.fs").c_str());

    ZShader lampShader("zlamp.vs", "zlamp.fs", "LampShader");

    ZShader sphereShader("zsphere.vs", "zsphere.fs","SphereShader");

    ZShader lightingShader("zlighting.vs", "zlighting.fs","LightingShader");

    ZShader BlightingShader("zbasic_lighting.vs", "zbasic_lighting.fs","BasicLightingShader");


    // lamp object
    camera.setLightPosition(glm::vec3(1.0f, 0.2f, 1.0f));


//==============GL Objects setup======================================

/* process box */
//    unsigned int BoxVBO, BoxVAO, BoxNormVBO;/* Box GL identifiers */

    ZTexture wTexWoodFloor("wood.png");

    wBox.setDefaultColor(ZBlueColor);
    wBox.setDefaultAlpha(0.5f);

    wBox.setupGL(&__SHADER__,
                 ZObject::SetupVertices+ZObject::setupNormals+ZObject::setupTextures,
                 GL_TRIANGLES,
                 &wTexWoodFloor);

//    wBox.setTexture("wood.png");
    __SHADER__.use();
    __SHADER__.setInt("TextureSampler",0);
    __SHADER__.setBool ("BlinnPhong",true);

//    wBox.print();


/* process arc front left */
    wArcFL.setDefaultColor(ZBlueColor);
    wArcFL.setDefaultAlpha(0.5f);
    wArcFL.setupGL(&__SHADER__,
                   ZObject::SetupVertices+ZObject::setupNormals+ZObject::setupTextures,
                   GL_TRIANGLE_FAN,
                   &wTexWoodFloor);
 //           (apos,anormal,&__SHADER__,ZBlueColor,Alpha,GL_TRIANGLE_FAN);

/* process arc front right */

    wArcFR.setDefaultColor(ZBlueColor);
    wArcFR.setDefaultAlpha(0.5f);
    wArcFR.setupGL(&__SHADER__,
                   ZObject::SetupVertices+ZObject::setupNormals+ZObject::setupTextures,
                   GL_TRIANGLE_FAN,
                   &wTexWoodFloor);
//            (apos,anormal,&__SHADER__,ZBlueColor,Alpha,GL_TRIANGLE_FAN);
/* process arc bottom left */
    wArcBL.setDefaultColor(ZBlueColor);
    wArcBL.setDefaultAlpha(0.5f);
    wArcBL.setupGL(&__SHADER__,
                   ZObject::SetupVertices+ZObject::setupNormals,
                   GL_TRIANGLE_FAN);


/* process arc bottom right */
    wArcBR.setDefaultColor(ZBlueColor);
    wArcBR.setDefaultAlpha(0.5f);
    wArcBR.setupGL(&__SHADER__,
                   ZObject::SetupVertices+ZObject::setupNormals,
                   GL_TRIANGLE_FAN);

/* process arc strips right */
    wArcStripsRight.setDefaultColor(ZBlueColor);
    wArcStripsRight.setDefaultAlpha(0.5f);
    wArcStripsRight.setupGL(&__SHADER__,
                   ZObject::SetupVertices+ZObject::setupNormals,
                   GL_TRIANGLES);

/* process arc strips leftt */
    wArcStripsLeft.setDefaultColor(ZBlueColor);
    wArcStripsLeft.setDefaultAlpha(0.5f);
    wArcStripsLeft.setupGL(&__SHADER__,
                   ZObject::SetupVertices+ZObject::setupNormals,
                   GL_TRIANGLES);


#ifdef __COMMENT__
/* process circles */
    wCircle1.setupGL(apos,anormal);
    wCircle2.setupGL(apos,anormal);

    wCircle3.setDefaultColor(ZGreyColor);
    wCircle3.setDefaultAlpha(0.5f);
//    wCircle3.setupGL(apos,anormal,&__SHADER__,ZGreyColor,Alpha,GL_TRIANGLE_FAN);
    wCircle3.setupGL(&sphereShader,
                   ZObject::SetupVertices+ZObject::setupNormals,
                   GL_TRIANGLES);
#endif//__COMMENT__
/* Pipe GL set-up */

    wPipe.setDefaultColor(ZGreyColor);
    wPipe.setDefaultAlpha(0.5f);
    wPipe.setupGL(&__SHADER__,
                   ZObject::SetupVertices+ZObject::setupNormals,
                   GL_TRIANGLES);


/* process lamp object */

    wLamp.setDefaultColor(Color_type(1.0f,1.0f,1.0f));
    wLamp.setDefaultAlpha(1.0f);
    wLamp.setupGL(&lampShader,
                   ZObject::SetupVertices,
                   GL_TRIANGLES);


    wSphere.setDefaultColor(ZBlueColor);
    wSphere.setDefaultAlpha(0.5f);
    wSphere.setDefaultPosition(Vertice_type(-0.6f,0.6f,0.0f));

    wSphere.setupGL(&__SHADER__,
                   ZObject::SetupVertices+ZObject::setupNormals+ZObject::setupTextures,
                   GL_TRIANGLES,
                    &wTexWoodFloor);


 //   wSphere.print(50); /* print 50 first data */




    // uncomment this call to draw in wireframe polygons.
//    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
//    glEnable(GL_CULL_FACE);

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

//        glClearColor(0.8f, 0.8f, 0.8f, 1.0f);

        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT| GL_DEPTH_BUFFER_BIT);




        // render lamp object
        lampShader.use();
        lampShader.setVec3("Color", ZYellowBright);

        glm::mat4 lampModel = glm::mat4(1.0f);
        lampModel = glm::translate(lampModel, camera.LightPosition);
        lampModel = glm::scale(lampModel, glm::vec3(0.2f)); // a smaller cube - no scaling

        glm::mat4 lampView = camera.GetViewMatrix();

        glm::mat4 lampProjection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);

        lampShader.setMat4("mProjection", lampProjection);
        lampShader.setMat4("mView", lampView);
        lampShader.setMat4("mModel", lampModel);

        wLamp.drawGL(GL_TRIANGLES);

        // render objects
        // ------

        __SHADER__.use();
/* light color parameters */

        __SHADER__.setLight(ZLight(ZYellowAmbient,ZYellowDiffuse,ZYellowSpecular));
        // pass projection matrix to shader (note that in this case it could change every frame)
        glm::mat4 mProjection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        __SHADER__.setMat4("mProjection", mProjection);

        // camera/view transformation
        glm::mat4 mView = camera.GetViewMatrix();
        __SHADER__.setMat4("mView", mView);


        // calculate the model matrix for each object and pass it to shader before drawing
//        glm::mat4 model = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
//        model = glm::translate(model, objectLocation);
//        if (camera.RotationAngle > 0.0f) /* if any rotation requested */
//                model = glm::rotate(model, (float)camera.RotationAngle, camera.RotationAxis);

//        glm::mat4 mModel = glm::translate(camera.getModel(), objectLocation);
//        sphereShader.setMat4("mModel", mModel);


        // mat4 normalMatrix = transpose(inverse(modelView));
        glm::mat4 mNormal = glm::transpose(glm::inverse(mView));
        __SHADER__.setMat4("mNormal", mNormal);

//        __SHADER__.setVec3("Light.position", lightPos);
//        __SHADER__.setVec3("lightPos", lightPos);
//        __SHADER__.setVec3("lightPos", camera.LightPosition);

        __SHADER__.setVec3("viewPosition", camera.CameraPosition);

        // light properties
        __SHADER__.setVec3("light.Position",camera.LightPosition);

/*        __SHADER__.setVec3("light.Ambient", lightAmbient); // not used
        __SHADER__.setVec3("light.Diffuse", lightDiffuse);  // not used
        __SHADER__.setVec3("light.Specular", lightSpecular);    // not used

        __SHADER__.setVec3("light.Color",1.0f,1.0f,1.0f);
*/
        __SHADER__.setVec3("InColor", ZBlueColor);
        __SHADER__.setFloat("InAlpha", 0.8f);
        // material properties
 /*       sphereShader.setVec3("material.Ambient", ZChrome.ambiant);
        sphereShader.setVec3("material.Diffuse", ZChrome.diffuse);
        sphereShader.setVec3("material.Specular", ZChrome.specular);
        sphereShader.setFloat("material.Shininess", ZChrome.shininess);
        sphereShader.setFloat("material.DiffuseAlpha", 1.0f);
 */
//        __SHADER__.use();
        __SHADER__.setMaterial(ZSphereMaterial);


        wBox.setUseTexture(true);
        wBox.setUseDefaultColor(false);
        wBox.setUseDefaultAlpha(false);

// calculate the model matrix for each object and pass it to shader before drawing
//        glm::mat4 model = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
//        model = glm::translate(model, objectLocation);
//        if (camera.RotationAngle > 0.0f) /* if any rotation requested */
//                model = glm::rotate(model, (float)camera.RotationAngle, camera.RotationAxis);

        glm::mat4 mModel = glm::translate(camera.getModel(), wBox.DefaultPosition);
        __SHADER__.setMat4("mModel", mModel);

        wBox.drawGL(GL_TRIANGLES);


        wArcFL.drawGL(GL_TRIANGLE_FAN);
        wArcFR.drawGL(GL_TRIANGLE_FAN);
        wArcBR.drawGL(GL_TRIANGLE_FAN);
        wArcBL.drawGL(GL_TRIANGLE_FAN);

        wArcStripsRight.drawGL(GL_TRIANGLES);
        wArcStripsLeft.drawGL(GL_TRIANGLES);

//        wCircle1.drawGL(GL_TRIANGLE_FAN);
//        wCircle2.drawGL(GL_TRIANGLE_FAN);
//        wCircle3.drawGL(GL_TRIANGLE_FAN);


        wPipe.drawGL(GL_TRIANGLES);

        mModel = glm::translate(camera.getModel(), wSphere.DefaultPosition);
        __SHADER__.setMat4("mModel", mModel);

        wSphere.setUseTexture(false);
        wSphere.drawGL(GL_TRIANGLES);
/*
        glBindVertexArray(ArcFLVAO); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized
        glBindBuffer(GL_ARRAY_BUFFER, ArcFLVBO);
        glBindBuffer(GL_ARRAY_BUFFER, ArcFLNormVBO);
        glDrawArrays(GL_TRIANGLE_FAN, 0 , wArcFLFlat.size());
*/


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

    GLResources->cleanAll(); /* clean all objects' OpenGL contexts before quitting */

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}

float accelerator=0.005f;

// ---------------------------------------------------------------------------------------------------------
// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window)
{

    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    /* mouse activation - desactivation */

    if (glfwGetKey(window, GLFW_KEY_INSERT )== GLFW_PRESS)   /* <Insert> switches mouse on / off  */
        {
        camera.setMouseOnOff();
        return;
        }

    if (glfwGetKey(window, GLFW_MOD_SHIFT)==GLFW_PRESS)
            {
            camera.setShiftOn();
            return;
            }
    if (glfwGetKey(window, GLFW_MOD_SHIFT)==GLFW_RELEASE)
            {
            camera.setShiftOff();
//            return;
            }

    if (glfwGetKey(window, GLFW_KEY_KP_8)==GLFW_PRESS)
                {
                camera.rotateModel(0.05,glm::vec3(1.0f,0.0f,0.0f));
                camera.scheduleRedraw();
                return;
                }
    if (glfwGetKey(window, GLFW_KEY_KP_2)==GLFW_PRESS)
                {
                camera.rotateModel(-0.05,glm::vec3(1.0f,0.0f,0.0f));
                camera.scheduleRedraw();
                return;
                }
    if (glfwGetKey(window, GLFW_KEY_KP_6)==GLFW_PRESS)
                {
                camera.rotateModel(+0.05,glm::vec3(0.0f,1.0f,0.0f));
                camera.scheduleRedraw();
                return;
                }
    if (glfwGetKey(window, GLFW_KEY_KP_4)==GLFW_PRESS)
                {
                camera.rotateModel(-0.05,glm::vec3(0.0f,1.0f,0.0f));
                camera.scheduleRedraw();
                return;
                }
     if ((glfwGetKey(window, GLFW_KEY_KP_5)==GLFW_PRESS)||
        (glfwGetKey(window, GLFW_KEY_TAB)==GLFW_PRESS))
        {
            camera.RotationAngle=0.0f;
            camera.RotationAxis=glm::vec3(0.0f);
            camera.Rotation=false;
            camera.setModelIdentity();
            return;
        }

    /* camera movements */
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        {
        camera.ProcessKeyboard(FORWARD, accelerator);
        return;
        }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        {
        camera.ProcessKeyboard(BACKWARD, accelerator);
        return;
        }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        {
        camera.ProcessKeyboard(LEFT, accelerator);
        return;
        }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        {
        camera.ProcessKeyboard(RIGHT, accelerator);
        return;
        }
    /* light movement */

    if (glfwGetKey(window, GLFW_KEY_U) == GLFW_PRESS)
        {
        camera.ProcessKeyboard(LIGHTFORWARD, accelerator);
        return;
        }
    if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS)
        {
        camera.ProcessKeyboard(LIGHTBACKWARD, accelerator);
        return;
        }
    if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS)
        {
        camera.ProcessKeyboard(LIGHTRIGHT, accelerator);
        return;
        }
    if (glfwGetKey(window, GLFW_KEY_H) == GLFW_PRESS)
        {
        camera.ProcessKeyboard(LIGHTLEFT, accelerator);
        return;
        }
    if (glfwGetKey(window, GLFW_KEY_M) == GLFW_PRESS)
        {
        camera.ProcessKeyboard(LIGHTHIGH, accelerator);
        return;
        }
    if (glfwGetKey(window, GLFW_KEY_N )== GLFW_PRESS)
        {
        camera.ProcessKeyboard(LIGHTLOW, accelerator);
        return;
        }
    if (glfwGetKey(window, GLFW_KEY_L )== GLFW_PRESS)   /* L for polygon lines */
        {
        camera.ProcessKeyboard(POLYGONLINES, accelerator);
        return;
        }
    if (glfwGetKey(window, GLFW_KEY_P )== GLFW_PRESS)   /* P for plain (no polygon line - fill) */
        {
        camera.ProcessKeyboard(POLYGONFILL, accelerator);
        return;
        }
    if (glfwGetKey(window, GLFW_KEY_HOME )== GLFW_PRESS)   /* Home resets camera position */
        {
        camera.ResetPositions();
        return;
        }



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


