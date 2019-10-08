#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/perpendicular.hpp>

#include <stb_image.h>

#include <camera.h>
#include <shader.h>

#include <zmaterial.h>

#define __USE_ZOBJECT__

#include <zobject.h>
#include <zobjectfunctions.h>



void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow *window);

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);

unsigned int loadTexture(char const * path);


// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// camera
Camera camera(Vertice_type(0.0f, 0.0f, 3.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

// timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;


static class CResources
{
public:
    const char * ShaderRootPath ="/home/gerard/Development/TestOpenGl/";
    const char * TextureRootPath ="/home/gerard/Development/TestOpenGl/";

    std::string getShaderPath (const char*pName)
    {
        std::string wFullPath=ShaderRootPath ;
        wFullPath += pName;
        return wFullPath;
    }
    std::string getTexturePath (const char*pName)
    {
        std::string wFullPath=TextureRootPath ;
        wFullPath += pName;
        return wFullPath;
    }
private:
    char WorkArea [255];
} Resources;




Vertice_type objectLocation=glm::vec3(0.0f, 0.0f, 0.0f);
//Vertice_type objectrotationaxis=glm::vec3(0.0f, 0.0f, 0.0f);

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

    ZObject wBox;
    ZObject wLamp;

    const float wHigh=0.5f;
    const float wWidth=0.5f;
    const float wDepth=0.2f;

//    zbs::ZArray<Vertice_type> wMids=boxIndexSetup(&wBox,ZBlueColor,wHigh,wWidth,wDepth);

    zbs::ZArray<Vertice_type> wMids=openboxVerticeSetup(&wBox,ZBlueColor,wHigh,wWidth,wDepth);
    zbs::ZArray<Vertice_type> wBoxFlat = wBox.toFlatVertices();


    boxIndexSetup(&wLamp,ZBlueColor,wHigh,wWidth,wDepth); /* same shape as wBox (but scaled down ) */

    printf ("wBox\nVertices \n------------\n");
    for (long wi=0;wi<wBoxFlat.count();wi++)
        printf ("%s %f %f %f \n",wBox.VName[wi],wBoxFlat[wi].x,wBoxFlat[wi].y,wBoxFlat[wi].z);

    printf ("Mids \n------------\n");
    for (long wi=0;wi<wMids.count();wi++)
        printf ("%f %f %f \n",wMids[wi].x,wMids[wi].y,wMids[wi].z);

    float wRadius= wHigh/2.0f;
    printf ("\n------\nArcFL\n---------\n");
    ZObject wArcFL = generate_Arc(wMids[0],wRadius,10,ZObject::DirLeft,ZObject::Front);  /* generate arc reverse (left) */
    zbs::ZArray<Vertice_type> wArcFLFlat = wArcFL.toFlatVertices();
    printf ("\n------\nArcBL\n---------\n");
    ZObject wArcBL = generate_Arc(wMids[1],wRadius,10,ZObject::DirLeft,ZObject::Back);/* generate arc reverse (left) */
    zbs::ZArray<Vertice_type> wArcBLFlat = wArcBL.toFlatVertices();
    printf ("\n------\nArcFR\n---------\n");
    ZObject wArcFR = generate_Arc(wMids[2],wRadius,10,ZObject::DirRight,ZObject::Front);/* generate front arc forward (right) */
    zbs::ZArray<Vertice_type> wArcFRFlat = wArcFR.toFlatVertices();
    printf ("\n------\nArcBR\n---------\n");
    ZObject wArcBR = generate_Arc(wMids[3],wRadius,10,ZObject::DirRight,ZObject::Back);/* generate bottom arc forward (right) */
    zbs::ZArray<Vertice_type> wArcBRFlat = wArcBR.toFlatVertices();

    ZObject wArcStripsLeft = generate_ArcStrips(wArcFL,wArcBL,ZObject::Left);
    zbs::ZArray<Vertice_type> wArcStripsLeftFlat = wArcStripsLeft.toFlatVertices();

    ZObject wArcStripsRight = generate_ArcStrips(wArcFR,wArcBR,ZObject::Right);
    zbs::ZArray<Vertice_type> wArcStripsRightFlat = wArcStripsRight.toFlatVertices();


    Vertice_type wC1=Vertice_type(0.5f,0.8f,0.0f);

    Vertice_type wC2=Vertice_type(0.0f,0.8f,0.0f);

    Vertice_type wC3=Vertice_type(-0.5f,0.8f,0.0f);

    ZObject wCircle1=generate_Circle(wC1,0.3f,20,ZObject::Face,ZObject::Front);

    ZObject wCircle2=generate_Circle(wC2,0.3f,20,ZObject::Flat,ZObject::Top);

    ZObject wCircle3=generate_Circle(wC3,0.3f,20,ZObject::Sequent,ZObject::Left);


//=============================================================================
    // configure global opengl state
    // -----------------------------
    glEnable(GL_DEPTH_TEST);
    // build and compile our shader zprogram
    // ------------------------------------
    // build and compile our shader zprogram
    // ------------------------------------
    Shader lightingShader(Resources.getShaderPath("zbasic_lighting.vs").c_str(), Resources.getShaderPath("zbasic_lighting.fs").c_str());
//    Shader lampShader("4.2.lamp.vs", "4.2.lamp.fs");
    Shader lampShader(Resources.getShaderPath("zlamp.vs").c_str(), Resources.getShaderPath("zlamp.fs").c_str());

//    Shader ourShader(Resources.getShaderPath("zcamera.vs").c_str(), Resources.getShaderPath("zcamera.fs").c_str());

    glm::vec3 Color = glm::vec3(0.5f, 0.8f, 1.0f);
    float Alpha = 0.5f;
    lightingShader.use();
    lightingShader.setVec3("InColor",Color);
    lightingShader.setFloat("InAlpha",Alpha);

    // lamp object




    camera.setLightPosition(glm::vec3(1.0f, 0.2f, 1.0f));


//==============GL Objects setup======================================

    const unsigned int apos=0;      /* shader position 0 apos */
    const unsigned int anormal=1;     /* shader position 1 anormal */

/* process box */
//    unsigned int BoxVBO, BoxVAO, BoxNormVBO;/* Box GL identifiers */

/*    ZGLObjDescriptor wGLBox;
    createGL_ObjectArray(wBox,&wGLBox,apos,anormal) ;
*/
    wBox.setupGL(apos,anormal);

/* process arc front left */
//    unsigned int ArcFLVBO, ArcFLVAO, ArcFLNormVBO;
    printf ("\n------\nArcFL\n---------\n");
//    ZGLObjDescriptor wGLArcFL;                                  /* Arc FL GL identifiers */
//    createGL_ObjectArray(wArcFL,&wGLArcFL,apos,anormal) ;
    wArcFL.setupGL(apos,anormal);

/* process arc front right */
//    unsigned int ArcFRVBO, ArcFRVAO, ArcFRNormVBO;
    printf ("\n------\nArcFR\n---------\n");
//    ZGLObjDescriptor wGLArcFR;                                  /* Arc FR GL descriptor */
//    createGL_ObjectArray(wArcFR,&wGLArcFR,apos,anormal) ;
    wArcFR.setupGL(apos,anormal);
/* process arc bottom left */
     printf ("\n------\nArcBL\n---------\n");
//    ZGLObjDescriptor wGLArcBL;                                      /* Arc BL GL descriptor */
//    createGL_ObjectArray(wArcBL,&wGLArcBL,apos,anormal) ;
    wArcBL.setupGL(apos,anormal);

/* process arc bottom right */
     printf ("\n------\nArcBR\n---------\n");
//    ZGLObjDescriptor wGLArcBR;                                      /* Arc BR GL descriptor */
 //   createGL_ObjectArray(wArcBR,&wGLArcBR,apos,anormal) ;
    wArcBR.setupGL(apos,anormal);

/* process arc strips right */
//    ZGLObjDescriptor wGLArcStripsRight;                                      /* Arc BR GL descriptor */
//    createGL_ObjectArray(wArcStripsRight,&wGLArcStripsRight,apos,anormal) ;
    wArcStripsRight.setupGL(apos,anormal);

/* process arc strips leftt */
//    ZGLObjDescriptor wGLArcStripsLeft;                                      /* Arc BR GL descriptor */
//    createGL_ObjectArray(wArcStripsLeft,&wGLArcStripsLeft,apos,anormal) ;
    wArcStripsLeft.setupGL(apos,anormal);

/* process circles */
//    ZGLObjDescriptor wGLCircle1;                                      /* Arc BR GL descriptor */
//    createGL_ObjectArray(wCircle1,&wGLCircle1,apos,anormal) ;
    wCircle1.setupGL(apos,anormal);

//    ZGLObjDescriptor wGLCircle2;                                      /* Arc BR GL descriptor */
//    createGL_ObjectArray(wCircle2,&wGLCircle2,apos,anormal);
    wCircle2.setupGL(apos,anormal);

//    ZGLObjDescriptor wGLCircle3;                                      /* Arc BR GL descriptor */
//    createGL_ObjectArray(wCircle3,&wGLCircle3,apos,anormal);
    wCircle3.setupGL(apos,anormal);

/* process lamp object */



    const unsigned int lampapos=0; /* shader position 0 apos */

//    ZGLObjDescriptor wGLLamp;
 //   zbs::ZArray<Vertice_type> wLampFlat=wLamp.toRawVertices();
//    zbs::ZArray<Vertice_type> wLampFlat=wLamp.toFlatVertices();
//    createGL_ObjectElement(wLamp,&wGLLamp,lampapos,-1) ;
    wLamp.setupGL(lampapos,-1);


/*    printf ("Lamp\n-------\nVertices\n--------\n");
    for (long wi=0; wi < wLampFlat.count();wi++)
            printf ("%f %f %f\n",wLampFlat[wi].x,wLampFlat[wi].y,wLampFlat[wi].z);

    printf ("Indices\n--------\n");
    for (long wi=0; wi < wLamp.Indices.count();wi+=3)
            printf ("%ud %ud %ud\n",wLamp.Indices[wi],wLamp.Indices[wi+1],wLamp.Indices[wi+2]);

    printf ("Box\n-------\nVertices\n--------\n");
    for (long wi=0; wi < wBoxFlat.count();wi++)
            printf ("%f %f %f\n",wBoxFlat[wi].x,wBoxFlat[wi].y,wBoxFlat[wi].z);

    printf ("Indices\n--------\n");
    for (long wi=0; wi < wBox.Indices.count();wi+=3)
            printf ("%ud %ud %ud\n",wBox.Indices[wi],wBox.Indices[wi+1],wBox.Indices[wi+2]);
*/


    // uncomment this call to draw in wireframe polygons.
//    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

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

        // render
        // ------
        glClearColor(0.8f, 0.8f, 0.8f, 1.0f);
//        glClearColor(204.0f, 204.0f, 204.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT| GL_DEPTH_BUFFER_BIT);

        lightingShader.use();
/*        glBindVertexArray(BoxVAO); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized
        glBindBuffer(GL_ARRAY_BUFFER, BoxVBO);
        glBindBuffer(GL_ARRAY_BUFFER, BoxNormVBO);
*/
        // pass projection matrix to shader (note that in this case it could change every frame)
        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        lightingShader.setMat4("projection", projection);

        // camera/view transformation
        glm::mat4 view = camera.GetViewMatrix();
        lightingShader.setMat4("view", view);

        // calculate the model matrix for each object and pass it to shader before drawing
 //       glm::mat4 model = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first

        if (camera.RotationAngle > 0.0f) /* if any rotation requested */
                camera.model = glm::rotate(camera.model, (float)camera.RotationAngle, camera.RotationAxis);

        glm::mat4 wModel = glm::translate(camera.model, objectLocation);

        lightingShader.setMat4("model", wModel);

//        lightingShader.setVec3("Light.position", lightPos);
//        lightingShader.setVec3("lightPos", lightPos);
        lightingShader.setVec3("lightPos", camera.LightPosition);
        lightingShader.setVec3("viewPos", camera.Position);

        // light properties
        lightingShader.setVec3("Light.ambient", 0.2f, 0.2f, 0.2f);
        lightingShader.setVec3("Light.diffuse", 0.5f, 0.5f, 0.5f);
        lightingShader.setVec3("Light.specular", 1.0f, 1.0f, 1.0f);

        lightingShader.setVec3("lightColor",1.0f,1.0f,1.0f);


        lightingShader.setVec3("InColor", Color);
        lightingShader.setFloat("InAlpha", Alpha);


        // material properties
        lightingShader.setVec3("ambiant", ZChrome.ambiant);
        lightingShader.setVec3("diffuse", ZChrome.diffuse);
        lightingShader.setVec3("specular", ZChrome.specular);
        lightingShader.setFloat("shininess", ZChrome.shininess);



        wBox.drawGL();

  /*      wArcFL.drawGL();
        wArcFR.drawGL();
        wArcBR.drawGL();
        wArcBL.drawGL();

        wArcStripsRight.drawGL();
        wArcStripsLeft.drawGL();

        wCircle1.drawGL();
        wCircle2.drawGL();
        wCircle3.drawGL();*/

        // unbind both
        glBindVertexArray(0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

/*
        glBindVertexArray(ArcFLVAO); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized
        glBindBuffer(GL_ARRAY_BUFFER, ArcFLVBO);
        glBindBuffer(GL_ARRAY_BUFFER, ArcFLNormVBO);

        glDrawArrays(GL_TRIANGLE_FAN, 0 , wArcFLFlat.size());
*/


        // also draw the lamp object
        lampShader.use();
        lampShader.setMat4("projection", projection);
        lampShader.setMat4("view", view);
        wModel = glm::mat4(1.0f);
        wModel = glm::translate(wModel, camera.LightPosition);
        wModel = glm::scale(wModel, glm::vec3(0.2f)); // a smaller cube - no scaling
        lampShader.setMat4("model", wModel);

        wLamp.drawGL();

        // unbind all

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
    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}


// ---------------------------------------------------------------------------------------------------------
// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_MOD_SHIFT)==GLFW_PRESS)
            camera.setShiftOn();
    if (glfwGetKey(window, GLFW_MOD_SHIFT)==GLFW_RELEASE)
            camera.setShiftOff();

    if (glfwGetKey(window, GLFW_KEY_KP_6)==GLFW_PRESS)
                {
                if (camera.RotationAxis.x==0.0f)
 //               camera.RotationAxis.x=camera.RotationAxis.x + 0.01f;
 //               camera.RotationAngle += 0.05f;
                camera.RotationAxis=glm::vec3(1.0f,0.0f,0.0f);
                camera.RotationAngle=0.05f;
                camera.scheduleRedraw();
                return;
                }
    if (glfwGetKey(window, GLFW_KEY_KP_4)==GLFW_PRESS)
                {
                if (camera.RotationAxis.x==0.0f)
//                camera.RotationAxis.x=camera.RotationAxis.x - 0.01f;
//                camera.RotationAngle -= 0.05f;
                camera.RotationAxis=glm::vec3(1.0f,0.0f,0.0f);
                camera.RotationAngle=-0.05f;
                camera.scheduleRedraw();
                return;
                }
    if (glfwGetKey(window, GLFW_KEY_KP_8)==GLFW_PRESS)
                {
                if (camera.RotationAxis.y==0.0f)
//                camera.RotationAxis.y=camera.RotationAxis.y + 0.01f;
//                camera.RotationAngle += 0.05f;
                camera.RotationAxis=glm::vec3(0.0f,1.0f,0.0f);
                camera.RotationAngle=0.05f;
                camera.scheduleRedraw();
                return;
                }
    if (glfwGetKey(window, GLFW_KEY_KP_2)==GLFW_PRESS)
                {
                if (camera.RotationAxis.y==0.0f)
//                camera.RotationAxis.y=camera.RotationAxis.y - 0.01f;
//                camera.RotationAngle -= 0.05f;
                camera.RotationAxis=glm::vec3(0.0f,1.0f,0.0f);
                camera.RotationAngle=-0.05f;
                camera.scheduleRedraw();
                return;
                }
  //  if (glfwGetKey(window, GLFW_KEY_INSERT)==GLFW_PRESS)

    if (glfwGetKey(window, GLFW_KEY_TAB)==GLFW_PRESS)
        {
            camera.RotationAngle=0.0f;
            camera.RotationAxis=glm::vec3(0.0f);
            camera.model=glm::mat4(1.0f);
            camera.Rotation=false;
        }
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.ProcessKeyboard(FORWARD, deltaTime);
    else
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    else
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboard(LEFT, deltaTime);
    else
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard(RIGHT, deltaTime);
    else
    if (glfwGetKey(window, GLFW_KEY_U) == GLFW_PRESS)
        camera.ProcessKeyboard(LIGHTFORWARD, deltaTime);
    else
    if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS)
        camera.ProcessKeyboard(LIGHTBACKWARD, deltaTime);
    else
    if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS)
        camera.ProcessKeyboard(LIGHTRIGHT, deltaTime);
    else
    if (glfwGetKey(window, GLFW_KEY_H) == GLFW_PRESS)
        camera.ProcessKeyboard(LIGHTLEFT, deltaTime);
    else
    if (glfwGetKey(window, GLFW_KEY_M) == GLFW_PRESS)
        camera.ProcessKeyboard(LIGHTHIGH, deltaTime);
    else
    if (glfwGetKey(window, GLFW_KEY_N )== GLFW_PRESS)
        camera.ProcessKeyboard(LIGHTLOW, deltaTime);
    else
    if (glfwGetKey(window, GLFW_KEY_L )== GLFW_PRESS)   /* L for polygon lines */
        camera.ProcessKeyboard(POLYGONLINES, deltaTime);
    else
    if (glfwGetKey(window, GLFW_KEY_P )== GLFW_PRESS)   /* P for plain (no polygon line - fill) */
        camera.ProcessKeyboard(POLYGONFILL, deltaTime);
    return;
}

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
// utility function for loading a 2D texture from file
// ---------------------------------------------------
unsigned int loadTexture(char const * path)
{
    unsigned int textureID;


    int width, height, nrComponents;
    unsigned char *data = stbi_load(path, &width, &height, &nrComponents, 0);
    if (!data)
        {
        std::cout << "Texture failed to load at path: " << path << std::endl;
        stbi_image_free(data);
        return 0;
        }
    glGenTextures(1, &textureID);

    GLenum format=GL_RGB;
    if (nrComponents == 1)
        format = GL_RED;
    else if (nrComponents == 3)
        format = GL_RGB;
    else if (nrComponents == 4)
        format = GL_RGBA;

    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    stbi_image_free(data);

    return textureID;
}//loadTexture



