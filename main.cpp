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

#define __USE_ZOBJECT__

#include <zresource.h>

#include <zobject.h>
#include <zobjectfunctions.h>

//#include <zcandy.h>

#include <zsphere.h>
#include <ztexture.h>

#define __CANDY_SHADER__  materialShader
#define __COLOR_SHADER__    colorShader
#define __SPHERE_SHADER__   materialShader
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


//#ifdef __COMMENT__
//    zbs::ZArray<Vertice_type> wMids=boxIndexSetup(&wBox,ZBlueColor,wHigh,wWidth,wDepth);
    ZBoxComponents wBoxComponents;
//    ZObject wBox =openboxSetup(wHigh,wWidth,wDepth,wBoxComponents,"Openbox");

    ZObject wBox =openboxSetup(wHigh,wWidth,wDepth,wBoxComponents,"Openbox");
    wBox.setDefaultPosition(ZModelOrigin);

    ZBoxComponents wBox1Components;
    ZObject wBox1 =boxSetup(wHigh,wWidth,wDepth,wBox1Components,"Box1");
    wBox1.setDefaultPosition(Vertice_type(0.8,0.8,0.0));
    wBox1Components.generateShape(wBox);


    float wRadius= wHigh/2.0f;

//    ZObject wArcFL = generate_Arc(wMids[0],wRadius,10,ZObject::DirLeft,ZObject::Front,"ArcFL");  /* generate arc reverse (left) */
//    ZObject wArcBL = generate_Arc(wMids[1],wRadius,10,ZObject::DirLeft,ZObject::Back,"ArcBL");/* generate arc reverse (left) */

//    ZObject wArcFR = generate_Arc(wMids[2],wRadius,10,ZObject::DirRight,ZObject::Front,"ArcFR");/* generate front arc forward (right) */
//    ZObject wArcBR = generate_Arc(wMids[3],wRadius,10,ZObject::DirRight,ZObject::Back,"ArcBR");/* generate bottom arc forward (right) */

    ZObject wArcFL = generate_Arc(wBoxComponents.FLMid,wRadius,10,ZObject::DirLeft,ZObject::Front,"ArcFL");  /* generate arc reverse (left) */
    ZObject wArcBL = generate_Arc(wBoxComponents.BLMid,wRadius,10,ZObject::DirLeft,ZObject::Back,"ArcBL");/* generate arc reverse (left) */
    ZObject wArcFR = generate_Arc(wBoxComponents.FRMid,wRadius,10,ZObject::DirRight,ZObject::Front,"ArcFR");/* generate front arc forward (right) */
    ZObject wArcBR = generate_Arc(wBoxComponents.BRMid,wRadius,10,ZObject::DirRight,ZObject::Back,"ArcBR");/* generate bottom arc forward (right) */

//    ZObject wArcStripsLeft = generate_ArcStripsLeft(wArcFL,wArcBL);

    ZObject wArcStripsLeft = generate_ArcStripsLeft(wArcFL,wArcBL,"ArcStripLeft");
    ZObject wArcStripsRight = generate_ArcStripsRight(wArcFR,wArcBR,"ArcStripRight");
//#endif //__COMMENT__
#ifdef __COMMENT__
    ZCandy wCandy = generateCandy(wHigh,wWidth,wDepth,"Candy");

    wCandy.printLineShape();
#endif //__COMMENT__
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
//    glEnable( GL_POLYGON_SMOOTH );

    // build and compile our shader zprogram
    // ------------------------------------
//    ZShader lightingShader(Resources.getShaderPath("zbasic_lighting.vs").c_str(), Resources.getShaderPath("zbasic_lighting.fs").c_str());
    ZBoxComponents wLampComponents;
    ZObject wLamp =boxSetup(0.05f,0.05f,0.05f,wLampComponents,"Lamp"); /* same shape as wBox (but scaled down ) */
    wLampComponents.generateShape(wLamp);

    printf ("Lamp Indices\n");
    wLamp.print(20);


    ZShader lampShader("zlamp.vs", "zlamp.fs", "LampShader");
    /* process lamp object */

    wLamp.setDefaultColor(ZYellowBright);
    wLamp.setDefaultAlpha(1.0f);
    wLamp.setupGL(&lampShader,
                   ZObject::SetupVertices,
                   GL_TRIANGLES);

    lampShader.setVec3("DefaultColor",ZYellowBright);

    wLamp.setupGLShape(&lampShader);


 //   ZShader sphereShader("zsphere.vs", "zsphere.fs","SphereShader");

//    ZShader textureShader("zlighting.vs", "ztexture_initial.fs","TextureShader");

    ZShader colorShader("zlighting.vs", "zcolor_lighting.fs","ColorShader");

    ZShader normvisuShader("znormvisu.vs", "znormvisu.fs","NormVisuShader");

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
//#ifdef __COMMENT__



//    wBox.setDefaultPosition(ZModelOrigin);
    wBox.setDefaultColor(ZBlueColor);
//    wBox.setMaterial(ZChrome);
    wBox.setDefaultAlpha(0.5f);
    wBox.setComputeNormals(true);
    wBox.setComputeTexCoords(true);

    wBox.setupGL(&__COLOR_SHADER__,
                 ZObject::SetupVertices+ZObject::setupNormals+ZObject::setupTextures,
//                 ZObject::SetupVertices,
                 GL_TRIANGLES,
                 &wTexTissueBluePale);
    wBox.setupGLNormalVisu(&lampShader);

    wBox1.setupGL(&__COLOR_SHADER__,
                 ZObject::SetupVertices+ZObject::setupNormals+ZObject::setupTextures,
//                 ZObject::SetupVertices,
                 GL_TRIANGLES,
                 &wTexTissueBluePale);

    wBox1.setupGLNormalVisu(&lampShader);
    wBox1.setupGLShape(&lampShader);


    wBox1.print();
//#endif // __COMMENT__
#ifdef __COMMENT_
    wCandy.setMaterial(ZChrome);
//    wCandy.setDefaultColor(ZBlueColor);
//    wCandy.setDefaultAlpha(0.5f);


    wCandy.setupGL(&__CANDY_SHADER__,
                 ZObject::SetupVertices+ZObject::setupNormals+ZObject::setupTextures,
                 &wTexTissueBluePale);

#endif //__COMMENT_

//    wBox.setTexture("wood.png");
    __COLOR_SHADER__.use();
    __COLOR_SHADER__.setInt("TextureSampler",0);
    __COLOR_SHADER__.setBool ("BlinnPhong",true);


//    __SPHERE_SHADER__.use();
//    __SPHERE_SHADER__.setInt("TextureSampler",0);
//    __TEXTURE_SHADER__.setBool ("BlinnPhong",true);

//    wBox.print();

//#ifdef __COMMENT__
/* process arc front left */
    wArcFL.setDefaultPosition(ZModelOrigin);
    wArcFL.setDefaultColor(ZBlueColor);
    wArcFL.setDefaultAlpha(0.5f);
    wArcFL.setComputeNormals(true);
    wArcFL.setComputeTexCoords(true);
    wArcFL.setupGL(&__COLOR_SHADER__,
                   ZObject::SetupVertices+ZObject::setupNormals+ZObject::setupTextures,
                   GL_TRIANGLE_FAN,
                   wBox.getTexture());
 //           (apos,anormal,&__TEXTURE_SHADER__,ZBlueColor,Alpha,GL_TRIANGLE_FAN);

    wArcFL.setupGLNormalVisu(&__COLOR_SHADER__);

    wArcFL.setupGLNormalVisu(&lampShader);

/* process arc front right */
    wArcFR.setDefaultPosition(ZModelOrigin);
    wArcFR.setDefaultColor(ZBlueColor);
    wArcFR.setDefaultAlpha(0.5f);
    wArcFR.setComputeNormals(true);
    wArcFR.setComputeTexCoords(true);
    wArcFR.setupGL(&__COLOR_SHADER__,
                   ZObject::SetupVertices+ZObject::setupNormals+ZObject::setupTextures,
                   GL_TRIANGLE_FAN,
                   wBox.getTexture());
    wArcFR.setupGLNormalVisu(&lampShader);

/* process arc bottom left */
    wArcBL.setDefaultPosition(ZModelOrigin);
    wArcBL.setDefaultColor(ZBlueColor);
    wArcBL.setDefaultAlpha(0.5f);
    wArcBL.setComputeNormals(true);
    wArcBL.setComputeTexCoords(true);
    wArcBL.setupGL(&__COLOR_SHADER__,
                   ZObject::SetupVertices+ZObject::setupNormals,
                   GL_TRIANGLE_FAN);
    wArcBL.setupGLNormalVisu(&lampShader);

/* process arc bottom right */
    wArcBR.setDefaultPosition(ZModelOrigin);
    wArcBR.setDefaultColor(ZBlueColor);
    wArcBR.setDefaultAlpha(0.5f);
    wArcBR.setComputeNormals(true);
    wArcBR.setComputeTexCoords(true);
    wArcBR.setupGL(&__COLOR_SHADER__,
                   ZObject::SetupVertices+ZObject::setupNormals,
                   GL_TRIANGLE_FAN);
    wArcBR.setupGLNormalVisu(&lampShader);

/* process arc strips right */
    wArcStripsRight.setDefaultPosition(ZModelOrigin);
    wArcStripsRight.setDefaultColor(ZBlueColor);
    wArcStripsRight.setDefaultAlpha(0.5f);
    wArcStripsRight.setComputeNormals(true);
    wArcStripsRight.setComputeTexCoords(true);
    wArcStripsRight.setupGL(&lampShader,
                   ZObject::SetupVertices+ZObject::setupNormals,
                   GL_TRIANGLES);
    wArcStripsRight.setupGLNormalVisu(&lampShader);
/* process arc strips leftt */
    wArcStripsLeft.setDefaultPosition(ZModelOrigin);
    wArcStripsLeft.setDefaultColor(ZBlueColor);
    wArcStripsLeft.setDefaultAlpha(0.5f);
    wArcStripsLeft.setComputeNormals(true);
    wArcStripsLeft.setComputeTexCoords(true);
    wArcStripsLeft.setupGL(&__COLOR_SHADER__,
                   ZObject::SetupVertices+ZObject::setupNormals,
                   GL_TRIANGLES);
    wArcStripsLeft.setupGLNormalVisu(&lampShader);

    wArcStripsLeft.print();

 //   wArcStripsLeft.print();

//#endif // __COMMENT__

#ifdef __COMMENT__
/* process circles */
    wCircle1.setupGL(apos,anormal);
    wCircle2.setupGL(apos,anormal);

    wCircle3.setDefaultColor(ZGreyColor);
    wCircle3.setDefaultAlpha(0.5f);
//    wCircle3.setupGL(apos,anormal,&__TEXTURE_SHADER__,ZGreyColor,Alpha,GL_TRIANGLE_FAN);
    wCircle3.setupGL(&sphereShader,
                   ZObject::SetupVertices+ZObject::setupNormals,
                   GL_TRIANGLES);
#endif//__COMMENT__
/* Pipe GL set-up */
    wPipe.setDefaultPosition(Vertice_type(0.5f,0.5f,0.5f));
    wPipe.setDefaultColor(ZGreyColor);
    wPipe.setDefaultAlpha(0.5f);
    wPipe.setComputeNormals(true);
    wPipe.setComputeTexCoords(true);
    wPipe.setupGL(&__COLOR_SHADER__,
                   ZObject::SetupVertices+ZObject::setupNormals+ZObject::setupTextures,
                   GL_TRIANGLES,
                  &wTexMetal);




    wSphere.setDefaultPosition(Vertice_type(-0.8f,0.8f,0.0f));
    wSphere.setDefaultColor(ZBlueColor);
    wSphere.setDefaultAlpha(0.5f);
//    wSphere.setMaterial(ZBronze);
    wSphere.setComputeNormals(false);
    wSphere.setComputeTexCoords(false);

    wSphere.setupGL(&__COLOR_SHADER__,
                   ZObject::SetupVertices+ZObject::setupNormals+ZObject::setupTextures,
                   GL_TRIANGLES,
                    &wTexEarth);


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

        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT| GL_DEPTH_BUFFER_BIT);
//#ifdef __COMMENT__
        // render lamp object
        lampShader.use(); /* access violation */
        lampShader.setVec3("DefaultColor", ZYellowBright);


        glm::mat4 lampModel = glm::mat4(1.0f);
        lampModel = glm::translate(lampModel, camera.LightPosition);
//        lampModel = glm::scale(lampModel, glm::vec3(0.2f)); // a smaller cube - no scaling
        lampModel = glm::rotate(lampModel,(float)glfwGetTime(),glm::vec3(1.0f, 0.5f, 0.2f));


        glm::mat4 lampView = camera.GetViewMatrix();

        glm::mat4 lampProjection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);

        lampShader.setMat4("mProjection", lampProjection);
        lampShader.setMat4("mView", lampView);
        lampShader.setMat4("mModel", lampModel);
        lampShader.setVec3("DefaultColor", ZYellowBright);

        wLamp.drawGL(&lampShader,GL_TRIANGLES);

        lampShader.setVec3("DefaultColor", ZGreySilver);

        wLamp.drawGLShape(&lampShader);


//#endif //__COMMENT__
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


        glm::mat4 mNormal = camera.GetViewMatrix()*camera.getModel();


        mNormal = glm::transpose(glm::inverse(mNormal));

        __COLOR_SHADER__.setMat4("mNormal", mNormal);

        __COLOR_SHADER__.setVec3("viewPosition", camera.CameraPosition);
/* light properties  */

        __COLOR_SHADER__.setVec3("light.Position",camera.LightPosition);
/* light color parameters */
        __COLOR_SHADER__.setLight(ZLight(ZYellowAmbient,ZYellowDiffuse,ZYellowSpecular));

/* Object color properties */
        __COLOR_SHADER__.setVec3("DefaultColor", ZBlueColor);
        __COLOR_SHADER__.setFloat("DefaultAlpha", 0.5f);


//#ifdef __COMMENT__
        wBox.setUseTexture(true);
        wBox.setUseDefaultColor(false);
        wBox.setUseDefaultAlpha(false);

//        wBox.setupShaderMaterial(&__COLOR_SHADER__);
//#endif // __COMMENT__
#ifdef __COMMENT__
        wCandy.setUseTexture(true);
        wCandy.setUseDefaultColor(false);
        wCandy.setUseDefaultAlpha(false);

        wCandy.setupShaderMaterial();

#endif // __COMMENT__

// calculate the model matrix for each object and pass it to shader before drawing
//        glm::mat4 model = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
//        model = glm::translate(model, objectLocation);
//        if (camera.RotationAngle > 0.0f) /* if any rotation requested */
//                model = glm::rotate(model, (float)camera.RotationAngle, camera.RotationAxis);
#ifdef __COMMENT__
//__COMMENT__
        glm::mat4 mModel = glm::translate(camera.getModel(), wCandy.DefaultPosition);
        __CANDY_SHADER__.setMat4("mModel", mModel);

//        wBox.drawGL(GL_TRIANGLES);

        wCandy.drawGL();
        glfwSwapBuffers(window);

        camera.cancelRedraw();

        glfwPollEvents();
        continue;
#endif // __COMMENT__
//        wBox.setShader(&__COLOR_SHADER__);
        __COLOR_SHADER__.use();
        __COLOR_SHADER__.setMat4("mProjection", mProjection);
        __COLOR_SHADER__.setMat4("mNormal", mNormal);
        __COLOR_SHADER__.setMat4("mView", mView);

        __COLOR_SHADER__.setVec3("light.Position",camera.LightPosition);

        __COLOR_SHADER__.setVec3("viewPosition", camera.CameraPosition);

        __COLOR_SHADER__.setVec3("DefaultColor", ZBlueColor);
        __COLOR_SHADER__.setFloat("DefaultAlpha", 0.5f);


        glm::mat4 mModel = glm::mat4(1.0f);
//        mModel =        glm::translate(camera.getModel(), wBox.DefaultPosition);
        mModel=camera.getModel();
        __COLOR_SHADER__.setMat4("mModel", mModel);
        wBox.drawGL(&__COLOR_SHADER__,GL_TRIANGLES);

    // add time component to geometry shader in the form of a uniform
//    normvisuShader.setFloat("time",glfwGetTime());
    if (camera.useNormalVectors)
        {
        lampShader.use();
        lampShader.setMat4("mModel", mModel);
        lampShader.setMat4("mProjection", mProjection);
        lampShader.setMat4("mNormal", mNormal);
        lampShader.setMat4("mView", mView);

        lampShader.setVec3("DefaultColor",ZYellowSpecular);
        wBox.drawGLNormalVisu(&lampShader);
        }




//#ifdef __COMMENT__
 //        mModel = glm::translate(camera.getModel(), wArcFL.DefaultPosition);
//        __COLOR_SHADER__.setMat4("mModel", mModel);
//        lampShader.setMat4("mModel", mModel);
        wArcFL.drawGL(&__COLOR_SHADER__,GL_TRIANGLE_FAN);
        if (camera.useNormalVectors)
            wArcFL.drawGLNormalVisu(&lampShader);

//        mModel = glm::translate(camera.getModel(), wArcFR.DefaultPosition);
//        __COLOR_SHADER__.setMat4("mModel", mModel);
        wArcFR.drawGL(&__COLOR_SHADER__,GL_TRIANGLE_FAN);
        if (camera.useNormalVectors)
            wArcFR.drawGLNormalVisu(&lampShader);
//        mModel = glm::translate(camera.getModel(), wArcBR.DefaultPosition);
//        __COLOR_SHADER__.setMat4("mModel", mModel);
        wArcBR.drawGL(&__COLOR_SHADER__,GL_TRIANGLE_FAN);
        if (camera.useNormalVectors)
            wArcBR.drawGLNormalVisu(&lampShader);
//        mModel = glm::translate(camera.getModel(), wArcBL.DefaultPosition);
//        __COLOR_SHADER__.setMat4("mModel", mModel);
//        wArcBL.drawGL(&__COLOR_SHADER__,GL_TRIANGLE_FAN);
        if (camera.useNormalVectors)
            wArcBL.drawGLNormalVisu(&lampShader);

//        mModel = glm::translate(camera.getModel(), wArcStripsRight.DefaultPosition);
//        __COLOR_SHADER__.setMat4("mModel", mModel);
        wArcStripsRight.drawGL(&__COLOR_SHADER__,GL_TRIANGLES);
        if (camera.useNormalVectors)
            wArcStripsRight.drawGLNormalVisu(&lampShader);

//        mModel = glm::translate(camera.getModel(), wArcStripsLeft.DefaultPosition);
//        __COLOR_SHADER__.setMat4("mModel", mModel);

        wArcStripsLeft.drawGL(&__COLOR_SHADER__,GL_TRIANGLES);
        if (camera.useNormalVectors)
            wArcStripsLeft.drawGLNormalVisu(&lampShader);

//        wCircle1.drawGL(&__COLOR_SHADER__,GL_TRIANGLE_FAN);
//        wCircle2.drawGL(&__COLOR_SHADER__,GL_TRIANGLE_FAN);
//        wCircle3.drawGL(&__COLOR_SHADER__,GL_TRIANGLE_FAN);

        __COLOR_SHADER__.use();

         mModel = glm::translate(camera.getModel(), wBox1.DefaultPosition);
        __COLOR_SHADER__.setMat4("mModel", mModel);
        wBox1.drawGL(&__COLOR_SHADER__);

        lampShader.use();
        lampShader.setMat4("mModel", mModel);
        if (camera.useNormalVectors)
            {
            wBox1.drawGLNormalVisu(&lampShader);
            }
        lampShader.setVec3("DefaultColor", ZGreySilver);
        wBox1.drawGLShape(&lampShader);



        __PIPE_SHADER__.use();
        mModel = glm::translate(camera.getModel(), wPipe.DefaultPosition);
        __PIPE_SHADER__.setMat4("mModel", mModel);

        __PIPE_SHADER__.setMat4("mProjection", mProjection);
        __PIPE_SHADER__.setMat4("mNormal", mNormal);
        __PIPE_SHADER__.setMat4("mView", camera.GetViewMatrix());

        __PIPE_SHADER__.setVec3("light.Position",camera.LightPosition);

        __PIPE_SHADER__.setVec3("viewPosition", camera.CameraPosition);

        wPipe.setUseTexture(false);
        wPipe.setUseDefaultColor(true);
        wPipe.setUseDefaultAlpha(true);
        wPipe.setDefaultColor(ZGreyColor);
        wPipe.setDefaultAlpha(0.5f);

        wPipe.drawGL(&__COLOR_SHADER__,GL_TRIANGLES);

/* sphere */

        __SPHERE_SHADER__.use();
        mModel = glm::translate(camera.getModel(), wSphere.DefaultPosition);

        __SPHERE_SHADER__.setMat4("mModel", mModel);

        __SPHERE_SHADER__.setMat4("mProjection", mProjection);
        __SPHERE_SHADER__.setMat4("mNormal", mNormal);
        __SPHERE_SHADER__.setMat4("mView", camera.GetViewMatrix());

        __SPHERE_SHADER__.setVec3("light.Position",camera.LightPosition);

        __SPHERE_SHADER__.setVec3("viewPosition", camera.CameraPosition);

        __SPHERE_SHADER__.setVec3("DefaultColor", ZBlueColor);
        __SPHERE_SHADER__.setFloat("DefaultAlpha", 0.5f);

 //       __SPHERE_SHADER__.setMaterial(ZBronze);
        __SPHERE_SHADER__.setFloat("material.DiffuseAlpha",1.0f);


//        wSphere.setupShaderMaterial(&__COLOR_SHADER__);

        wSphere.setUseTexture(false);
        wSphere.setUseDefaultColor(false);
        wSphere.setUseDefaultAlpha(false);
        wSphere.drawGL(&__COLOR_SHADER__,GL_TRIANGLES);
/*
        glBindVertexArray(ArcFLVAO); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized
        glBindBuffer(GL_ARRAY_BUFFER, ArcFLVBO);
        glBindBuffer(GL_ARRAY_BUFFER, ArcFLNormVBO);
        glDrawArrays(GL_TRIANGLE_FAN, 0 , wArcFLFlat.size());
*/
//#endif // __COMMENT__

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

float accelerator=0.001f;

// ---------------------------------------------------------------------------------------------------------
// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
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

    if (glfwGetKey(window, GLFW_KEY_KP_7)==GLFW_PRESS)
                {
                camera.rotateModel(-0.05,glm::vec3(1.0f,1.0f,0.0f));
                camera.scheduleRedraw();
                return;
                }
    if (glfwGetKey(window, GLFW_KEY_KP_9)==GLFW_PRESS)
                {
                camera.rotateModel(0.05,glm::vec3(1.0f,1.0f,0.0f));
                camera.scheduleRedraw();
                return;
                }
    if (glfwGetKey(window, GLFW_KEY_KP_1)==GLFW_PRESS)
                {
                camera.rotateModel(-0.05,glm::vec3(1.0f,-1.0f,0.0f));
                camera.scheduleRedraw();
                return;
                }
    if (glfwGetKey(window, GLFW_KEY_KP_3)==GLFW_PRESS)
                {
                camera.rotateModel(0.05,glm::vec3(1.0f,-1.0f,0.0f));
                camera.scheduleRedraw();
                return;
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

/* Keyboard actions */

    /* camera movements */
    if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS)
        {
        camera.ProcessKeyboard(FORWARD, accelerator);
        return;
        }
    if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS)
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
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        {
        camera.ProcessKeyboard(UP, accelerator);
        return;
        }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        {
        camera.ProcessKeyboard(DOWN, accelerator);
        return;
        }

    /* light movement */

    if (glfwGetKey(window, GLFW_KEY_SCROLL_LOCK) == GLFW_PRESS)
        {
        camera.ProcessKeyboard(LIGHTHIGH, accelerator);
        return;
        }
    if (glfwGetKey(window, GLFW_KEY_END) == GLFW_PRESS)
        {
        camera.ProcessKeyboard(LIGHTLOW, accelerator);
        return;
        }
    if (glfwGetKey(window, GLFW_KEY_INSERT) == GLFW_PRESS)
        {
        camera.ProcessKeyboard(LIGHTLEFT, accelerator);
        return;
        }
    if (glfwGetKey(window, GLFW_KEY_PAGE_UP) == GLFW_PRESS)
        {
        camera.ProcessKeyboard(LIGHTRIGHT, accelerator);
        return;
        }
    if (glfwGetKey(window, GLFW_KEY_PAUSE) == GLFW_PRESS)
        {
        camera.ProcessKeyboard(LIGHTBACKWARD, accelerator);
        return;
        }
    if (glfwGetKey(window, GLFW_KEY_PAGE_DOWN )== GLFW_PRESS)
        {
        camera.ProcessKeyboard(LIGHTFORWARD, accelerator);
        return;
        }

    /* toggle keys */


    if (glfwGetKey(window, GLFW_KEY_M )== GLFW_PRESS)   /* M for toggle mouse tracking on / off  */
        {
        camera.ProcessKeyboard(TOGGLE_MOUSE, accelerator);
        if (CurrentCursor==HandCursor)
                    CurrentCursor = DefaultCursor;
                else
                    CurrentCursor = HandCursor;

        glfwSetCursor(window,CurrentCursor) ;
        return;
        }
    if (glfwGetKey(window, GLFW_KEY_SCROLL_LOCK )== GLFW_PRESS)   /* L for toggle polygon lines */
        {
        camera.ProcessKeyboard(TOGGLE_POLYGONLINES, accelerator);
        return;
        }
    if (glfwGetKey(window, GLFW_KEY_L )== GLFW_PRESS)   /* L for toggle polygon lines */
        {
        camera.ProcessKeyboard(TOGGLE_POLYGONLINES, accelerator);
        return;
        }
    if (glfwGetKey(window, GLFW_KEY_N )== GLFW_PRESS)   /* N for toggle normal vectors */
        {
        camera.ProcessKeyboard(TOGGLE_NORMALVECTORS, accelerator);
        return;
        }

    if (glfwGetKey(window, GLFW_KEY_G )== GLFW_PRESS)   /* set or unset (if set)  gamma correction */
        {
        camera.ProcessKeyboard(TOGGLE_GAMMA, accelerator);
        return;
        }

/* special keys */
    if (glfwGetKey(window, GLFW_KEY_HOME )== GLFW_PRESS)   /* Home resets camera position */
        {
        camera.ProcessKeyboard(RESET_POSITION, accelerator);
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


