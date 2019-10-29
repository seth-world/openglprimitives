#ifdef __USE_GLAD__
#include <glad/glad.h>
#else
#include <GL/glew.h>
#endif
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

#ifdef __USE_ZOBJECT__
#include <zobject.h>

std::vector<Vertice_type> boxIndexSetup (ZObject* pObject,const Color_type &pColor,const float pHigh,const float pWidth, const float pDepth);

ZObject generate_Arc(Vertice_type pCenter,float pRadiusLen, int pDirection, int pNumber);
ZObject generate_ArcStrips(ZObject &pArc1,ZObject &pArc2);

#else

typedef glm::vec3 Vertice_type;
typedef glm::vec3 Color_type;
typedef glm::vec2 TextCoords_type;

const Color_type ZBlueColor(0.5f, 0.8f, 1.0f);
const Color_type ZGreyColor(0.8, 0.8, 0.8);



#endif
std::vector<Vertice_type> perfect_arc_setup(Vertice_type pCenter,float pRadiusLen, int pDirection, int pNumber);

std::vector<glm::vec3> generateVNormal(std::vector<glm::vec3> &wVertex);

void perfect_arc_setup(Vertice_type pCenter,float pStart, float pEnd, int pDirection, int pNumber);


void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow *window);


unsigned int loadTexture(char const * path);

std::vector<glm::vec3> packVertex(float* pInput, int pVCount);

std::vector<glm::vec3> generateVertex(float* pInput, int pVCount, unsigned int *pIndice, int pICount);

std::vector<glm::vec3> generateVVertex(std::vector<glm::vec3> pInput,  std::vector<unsigned int> pIndice);

std::vector<glm::vec3> generateVNormal(std::vector<glm::vec3> &wVertex);
std::vector<glm::vec3> generateVNormalMod(std::vector<glm::vec3> &wVertex);

std::vector<glm::vec3> toNormal(float* pInput, int pCount);

std::vector<glm::vec3> toNormalIndices(std::vector<glm::vec3> pInput,unsigned int pIndices[], int pCount);
std::vector<glm::vec3> toVNormal(std::vector<glm::vec3>& pInput);




std::vector<glm::vec3> toNormal(float* pInput, int pSize);
std::vector<glm::vec3> toVNormal(std::vector<glm::vec3>& pInput);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// camera
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

// timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;

// lighting
//glm::vec3 lightPos(1.2f, 1.0f, -2.0f);

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


void createGL_Object(std::vector<glm::vec3> &ObjectVertices,/* in vertices */
//                     std::vector<glm::vec3> &ObjectNormal,  /* out normal vectors */
                     unsigned int *ObjectVBO,               /* out vbo identifier */
                     unsigned int *ObjectVAO,               /* out vao identifier */
                     unsigned int *ObjectNormVBO)           /* out normal vbo identifier */
;
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

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

//=============================================================================
#ifdef __USE_ZOBJECT__
    ZObject wBox;

    const float wHigh=0.5f;
    const float wWidth=0.5f;
    const float wDepth=0.2f;
    std::vector<Vertice_type> wMids=boxIndexSetup(&wBox,ZBlueColor,wHigh,wWidth,wDepth);
    std::vector<Vertice_type> wBoxFlat = wBox.toFlatVertices();

    float wRadius= wHigh/2.0f;
    ZObject wArcFL = generate_Arc(wMids[0],wRadius,-1,10);
    std::vector<Vertice_type> wArcFLFlat = wArcFL.toFlatVertices();


    ZObject wArcFR = generate_Arc(wMids[1],wRadius,-1,10);

    ZObject wArcBL = generate_Arc(wMids[1],wRadius,-1,10);

    ZObject wArcBR = generate_Arc(wMids[1],wRadius,-1,10);



#endif //__USE_ZOBJECT__





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
    float Alpha = 0.2f;
    lightingShader.use();
    lightingShader.setVec3("InColor",Color);
    lightingShader.setFloat("InAlpha",Alpha);

    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------
  /*  float vertices[] = {
         0.5f,  0.5f, 0.0f,  // top right
         0.5f, -0.5f, 0.0f,  // bottom right
        -0.5f, -0.5f, 0.0f,  // bottom left
        -0.5f,  0.5f, 0.0f   // top left
    };
    unsigned int indices[] = {  // note that we start from 0!
        0, 1, 3,  // first Triangle
        1, 2, 3   // second Triangle
    };
    */
    
    // Only position, no color
         float vertices[] = {
        -0.2500f ,  0.2500f ,  0.2000f  , // FTL index 0
         0.2500f ,  0.2500f ,  0.2000f  , // FTR index 1
        -0.2500f , -0.2500f ,  0.2000f  , // FLL index 2
         0.2500f , -0.2500f ,  0.2000f  , // FLR index 3
        -0.2500f ,  0.2500f , -0.2000f  , // BTL index 4
         0.2500f ,  0.2500f , -0.2000f  , // BTR index 5
        -0.2500f , -0.2500f , -0.2000f  , // BLL index 6
         0.2500f , -0.2500f , -0.2000f  } ; // BLL index 7


     std::vector<glm::vec3> Vvertices = {
         {-0.2500f ,  0.2500f ,  0.2000f } , // FTL index 0
         { 0.2500f ,  0.2500f ,  0.2000f } , // FTR index 1
         {-0.2500f , -0.2500f ,  0.2000f } , // FLL index 2
         { 0.2500f , -0.2500f ,  0.2000f } , // FLR index 3
         {-0.2500f ,  0.2500f , -0.2000f } , // BTL index 4
         { 0.2500f ,  0.2500f , -0.2000f } , // BTR index 5
         {-0.2500f , -0.2500f , -0.2000f } , // BLL index 6
          {0.2500f , -0.2500f , -0.2000f }  // BLL index 7
     };


    //    Indices-----------
         unsigned int indices[] = {
     0 , 1 , 3  // FTL - FTR - FLR -
    , 3 , 2 , 0  // FLR - FLL - FTL -
    , 4 , 5 , 7  // BTL - BTR - BLR -
    , 7 , 6 , 4  // BLR - BLL - BTL -
    , 6 , 7 , 3  // BLL - BLR - FLR -
    , 3 , 2 , 6  // FLR - FLL - BLL -glm::vec3(0.0f, 1.0f, 1.0f)
    , 4 , 5 , 1  // BTL - BTR - FTR -
    , 1 , 0 , 4  // FTR - FTL - BTL -
    , 0 , 4 , 6  // FTL - BTL - BLL -
    , 6 , 2 , 0  // BLL - FLL - FTL -
    , 1 , 5 , 7  // FTR - BTR - BLR -
    , 7 , 3 , 1  // BLR - FLR - FTR -
     } ;
    std::vector<unsigned int> Vindices = {
     0 , 1 , 3  // FTL - FTR - FLR -
    , 3 , 2 , 0  // FLR - FLL - FTL -
    , 4 , 5 , 7  // BTL - BTR - BLR -
    , 7 , 6 , 4  // BLR - BLL - BTL -
    , 6 , 7 , 3  // BLL - BLR - FLR -
    , 3 , 2 , 6  // FLR - FLL - BLL -
    , 4 , 5 , 1  // BTL - BTR - FTR -
    , 1 , 0 , 4  // FTR - FTL - BTL -
    , 0 , 4 , 6  // FTL - BTL - BLL -
    , 6 , 2 , 0  // BLL - FLL - FTL -
    , 1 , 5 , 7  // FTR - BTR - BLR -
    , 7 , 3 , 1  // BLR - FLR - FTR -
     } ;
    // lamp object
    
    // Only position, no color
         float lampvertices[] = {
        -0.02500f ,  0.02500f ,  0.02000f  , // FTL index 0
         0.02500f ,  0.02500f ,  0.02000f  , // FTR index 1
        -0.02500f , -0.02500f ,  0.02000f  , // FLL index 2
         0.02500f , -0.02500f ,  0.02000f  , // FLR index 3
        -0.02500f ,  0.02500f , -0.02000f  , // BTL index 4
         0.02500f ,  0.02500f , -0.02000f  , // BTR index 5
        -0.02500f , -0.02500f , -0.02000f  , // BLL index 6
         0.02500f , -0.02500f , -0.02000f  } ; // BLL index 7

         std::vector<glm::vec3> lampVvertices = {
         {-0.02500f ,  0.02500f ,  0.02000f  }, // FTL index 0
         { 0.02500f ,  0.02500f ,  0.02000f  }, // FTR index 1
         {-0.02500f , -0.02500f ,  0.02000f  }, // FLL index 2
         { 0.02500f , -0.02500f ,  0.02000f  }, // FLR index 3
         {-0.02500f ,  0.02500f , -0.02000f  }, // BTL index 4
         { 0.02500f ,  0.02500f , -0.02000f  }, // BTR index 5
         {-0.02500f , -0.02500f , -0.02000f  }, // BLL index 6
         { 0.02500f , -0.02500f , -0.02000f  } // BLL index 7
         };


    //    Indices-----------
         unsigned int lampindices[] = {
     0 , 1 , 3  // FTL - FTR - FLR -
    , 3 , 2 , 0  // FLR - FLL - FTL -
    , 4 , 5 , 7  // BTL - BTR - BLR -
    , 7 , 6 , 4  // BLR - BLL - BTL -
    , 6 , 7 , 3  // BLL - BLR - FLR -
    , 3 , 2 , 6  // FLR - FLL - BLL -
    , 4 , 5 , 1  // BTL - BTR - FTR -
    , 1 , 0 , 4  // FTR - FTL - BTL -
    , 0 , 4 , 6  // FTL - BTL - BLL -
    , 6 , 2 , 0  // BLL - FLL - FTL -
    , 1 , 5 , 7  // FTR - BTR - BLR -
    , 7 , 3 , 1  // BLR - FLR - FTR -
     } ;

     std::vector<unsigned int> lampVindices = {
         0 , 1 , 3  // FTL - FTR - FLR -
        , 3 , 2 , 0  // FLR - FLL - FTL -
        , 4 , 5 , 7  // BTL - BTR - BLR -
        , 7 , 6 , 4  // BLR - BLL - BTL -
        , 6 , 7 , 3  // BLL - BLR - FLR -
        , 3 , 2 , 6  // FLR - FLL - BLL -
        , 4 , 5 , 1  // BTL - BTR - FTR -
        , 1 , 0 , 4  // FTR - FTL - BTL -
        , 0 , 4 , 6  // FTL - BTL - BLL -
        , 6 , 2 , 0  // BLL - FLL - FTL -
        , 1 , 5 , 7  // FTR - BTR - BLR -
        , 7 , 3 , 1  // BLR - FLR - FTR -
         } ;


 // glm::vec3 wobjectLocation=glm::vec3(0.1f, 0.0f, -0.5f);
    glm::vec3 wobjectLocation=glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3 wobjectrotationaxis=glm::vec3(0.0f, 1.0f, 1.0f);


    camera.setLightPosition(glm::vec3(0.0f, 1.0f, -1.0f));

    const unsigned int apos=0;      /* shader position 0 apos */
    const unsigned int anormal=1;     /* shader position 1 anormal */


#ifndef __USE_ZOBJECT__
    unsigned int BoxVBO, BoxVAO, BoxEBO , BoxNormVBO;

    glGenVertexArrays(1, &BoxVAO);
    glGenBuffers(1, &BoxVBO);
//    glGenBuffers(1, &ObjectEBO);
    // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
    glBindVertexArray(BoxVAO);

    glBindBuffer(GL_ARRAY_BUFFER, BoxVAO);
//    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    std::vector<glm::vec3> ObjectVertices = generateVVertex(Vvertices,Vindices);
//    int wVerticecount= sizeof(vertices)/sizeof(vertices[0]);
//    int wIndicecount = sizeof(indices)/sizeof(indices[0]);
//    std::vector<glm::vec3> ObjectVertices = generateVertex(vertices,wVerticecount,indices,wIndicecount);
    size_t wObjectSize=ObjectVertices.size()*sizeof(glm::vec3);
    glBufferData(GL_ARRAY_BUFFER, wObjectSize, ObjectVertices.data(), GL_STATIC_DRAW);

 //   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ObjectEBO);
 //   glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(apos, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(apos);

    std::vector<glm::vec3> ObjectNormal = generateVNormal(ObjectVertices);
    int wNormalSize=ObjectNormal.size()*sizeof(glm::vec3);

    for (long wi=0;wi<ObjectNormal.size();wi++)
        {
        printf ("%ld>> %f %f %f \n",wi,ObjectNormal[wi].x ,ObjectNormal[wi].y,ObjectNormal[wi].z);
        }

    glGenBuffers(1, &BoxNormVBO);
//    glGenBuffers(1, &BoxNormEBO);

    glBindBuffer(GL_ARRAY_BUFFER, BoxNormVBO);
    glBufferData(GL_ARRAY_BUFFER, wNormalSize, ObjectNormal.data(), GL_STATIC_DRAW);

 //   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ObjectEBO);
 //   glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW); // same indices as vertices

    glVertexAttribPointer(anormal, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(anormal);

    // remember: do NOT unbind the EBO while a VAO is active as the bound element buffer object IS stored in the VAO; keep the EBO bound.
    //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    // You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
    // VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
        // unbind
   // note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // unbind
    glBindVertexArray(0);
#else

/* process box */
    unsigned int BoxVBO, BoxVAO, BoxNormVBO;              /* Box GL identifiers */
    createGL_Object(wBoxFlat,&BoxVBO,&BoxVAO,&BoxNormVBO) ;

/* process arc front left */
    unsigned int ArcFLVBO, ArcFLVAO, ArcFLNormVBO;              /* Arc FL GL identifiers */
    createGL_Object(wArcFLFlat,&ArcFLVBO,&ArcFLVAO,&ArcFLNormVBO) ;
#endif //
/* process lamp object */

    const unsigned int lampapos=0; /* shader position 0 apos */

    unsigned int lampVBO, lampVAO, lampEBO;
    glGenVertexArrays(1, &lampVAO);
    glGenBuffers(1, &lampVBO);
    glGenBuffers(1, &lampEBO);

    // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
    glBindVertexArray(lampVAO);

    glBindBuffer(GL_ARRAY_BUFFER, lampVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(lampvertices), lampvertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, lampEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(lampindices), lampindices, GL_STATIC_DRAW);

    glVertexAttribPointer(lampapos, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(lampapos);

    // note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // remember: do NOT unbind the EBO while a VAO is active as the bound element buffer object IS stored in the VAO; keep the EBO bound.
    //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    // You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
    // VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
    glBindVertexArray(0);


    // uncomment this call to draw in wireframe polygons.
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);





    // render loop
    // -----------
    while (!glfwWindowShouldClose(window))
    {
        // input
        // -----
        processInput(window);

        // render
        // ------
        glClearColor(0.8f, 0.8f, 0.8f, 1.0f);
//        glClearColor(204.0f, 204.0f, 204.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT| GL_DEPTH_BUFFER_BIT);

        lightingShader.use();
        glBindVertexArray(BoxVAO); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized
        glBindBuffer(GL_ARRAY_BUFFER, BoxVBO);
        glBindBuffer(GL_ARRAY_BUFFER, BoxNormVBO);

        // pass projection matrix to shader (note that in this case it could change every frame)
        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        lightingShader.setMat4("projection", projection);

        // camera/view transformation
        glm::mat4 view = camera.GetViewMatrix();
        lightingShader.setMat4("view", view);

        // calculate the model matrix for each object and pass it to shader before drawing
        glm::mat4 model = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
        model = glm::translate(model, wobjectLocation);
        model = glm::rotate(model, (float)glfwGetTime(), wobjectrotationaxis);
        lightingShader.setMat4("model", model);

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
#ifdef __USE_ZOBJECT__
        glDrawArrays(GL_TRIANGLES, 0 , wBoxFlat.size());
#else
        int ws=sizeof(indices)/(sizeof(uint)); // number of indices to take into account
        glDrawElements(GL_TRIANGLES, ws, GL_UNSIGNED_INT, 0);
#endif

        // glBindVertexArray(0); // no need to unbind it every time

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
        model = glm::mat4(1.0f);
        model = glm::translate(model, camera.LightPosition);
//        model = glm::scale(model, glm::vec3(0.2f)); // a smaller cube - no scaling
        lampShader.setMat4("model", model);

        glBindVertexArray(lampVAO);
        glBindBuffer(GL_ARRAY_BUFFER, lampVBO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, lampEBO);
//        glDrawArrays(GL_TRIANGLES, 0, 36);

        int wl=sizeof(lampindices)/(sizeof(uint)); // number of indices to take into account

        glDrawElements(GL_TRIANGLES, wl, GL_UNSIGNED_INT, 0); // number of indices are the

        // unbind all
        glBindVertexArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------
    glDeleteVertexArrays(1, &BoxVAO);
    glDeleteBuffers(1, &BoxVBO);
//    glDeleteBuffers(1, &ObjectEBO);
    glDeleteBuffers(1, &BoxNormVBO);
    glDeleteVertexArrays(1, &lampVAO);
    glDeleteBuffers(1, &lampVBO);
    glDeleteBuffers(1, &lampEBO);
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
    if (glfwGetKey(window, GLFW_KEY_INSERT)==GLFW_PRESS)
            camera.setMouseOnOff();


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
}

std::vector<glm::vec3> packVertex(float* pInput, int pVCount)
{
    std::vector<glm::vec3> wReturn;

    for (int wi=0;wi<pVCount;wi+=wi+3)
        {
        glm::vec3 wVec3(pInput[wi],pInput[wi+1],pInput[wi+2]);
        }
    return wReturn;
}


std::vector<glm::vec3> generateVertex(float* pInput, int pVCount, unsigned int* pIndice, int pICount)
{
   std::vector<glm::vec3> wVertex = packVertex(pInput,pVCount) ;

   std::vector<glm::vec3> wReturn;
   for (int wi=0;wi<pICount;wi++)
       {
       wReturn.push_back(wVertex[pIndice[wi]]);
       }
    return wReturn;
}
std::vector<glm::vec3> generateVVertex(std::vector<glm::vec3> pInput,  std::vector<unsigned int> pIndice)
{
   printf ("generateVVertex-I vertices count <%ld> indices count <%ld>\n",pInput.size(),pIndice.size());
   std::vector<glm::vec3> wReturn;
   for (size_t wi=0;wi<pIndice.size();wi++)
       {
       wReturn.push_back(pInput[pIndice[wi]]);
       }

   printf ("generateVVertex-I resulting vertices count <%ld> \n",wReturn.size());

    return wReturn;
}
/* compute a triangle's surface normal vec3 vector */

glm::vec3 CalculateSurfaceNormal (glm::vec3* pTriangle)
{
    glm::vec3 Normal;
    glm::vec3 U = (pTriangle[1] - pTriangle[0]);
    glm::vec3 V = (pTriangle[2] - pTriangle[1]) ;

    Normal.x = (U.y * V.z) - (U.z * V.y);
    Normal.y = ( U.z * V.x) - ( U.x * V.z);
    Normal.z = ( U.x * V.y) - ( U.y * V.x);

    return Normal;
}
std::vector<glm::vec3> generateVNormal(std::vector<glm::vec3> &wVertex)
{
   glm::vec3 wTriangle[3];
   std::vector<glm::vec3> wReturn;
   for (unsigned int wi=0;wi<wVertex.size();wi=wi+3)
       {

       wTriangle[0]=wVertex[wi];/* compute normal for a triangle A B C */
       wTriangle[1]=wVertex[wi+1];
       wTriangle[2]=wVertex[wi+2];

       glm::vec3 wNormal=CalculateSurfaceNormal(wTriangle);

       wReturn.push_back(wNormal); /* per vertex normal : same for all three vertexes */
       wReturn.push_back(wNormal);
       wReturn.push_back(wNormal);
       }
    return wReturn;
}
std::vector<glm::vec3> generateVNormalMod(std::vector<glm::vec3> &wVertex)
{
   glm::vec3 wTriangle[3];
   std::vector<glm::vec3> wReturn;
   for (unsigned int wi=0;wi<wVertex.size();wi=wi+3)
       {

       wTriangle[0]=wVertex[wi];/* compute normal for a triangle A B C */
       wTriangle[1]=wVertex[wi+1];
       wTriangle[2]=wVertex[wi+2];

       glm::vec3 wNormal=CalculateSurfaceNormal(wTriangle);

       if (wNormal.x>0.0f)
                wNormal.x=1.0f;
       if (wNormal.y>0.0f)
                wNormal.y=1.0f;
       if (wNormal.z>0.0f)
                wNormal.z=1.0f;

       wReturn.push_back(wNormal); /* per vertex normal : same for all three vertexes */
       wReturn.push_back(wNormal);
       wReturn.push_back(wNormal);
       }
    return wReturn;
}
std::vector<glm::vec3> generateVNormal_Indexed(std::vector<glm::vec3> &wVertex, std::vector<unsigned int> &pIndice)
{
   glm::vec3 wTriangle[3];
   std::vector<glm::vec3> wReturn;
   for (unsigned int wi=0;wi<pIndice.size();wi+=wi+3)
       {
       glm::vec3 wNormal;
       wTriangle[0]=wVertex[pIndice[wi]];/* compute normal for a triangle A B C */
       wTriangle[1]=wVertex[pIndice[wi+1]];
       wTriangle[2]=wVertex[pIndice[wi+2]];

       wReturn.push_back(wNormal); /* per vertex normal : same for all three vertexes */
       wReturn.push_back(wNormal);
       wReturn.push_back(wNormal);
       }
    return wReturn;
}
#ifdef __USE_ZOBJECT__
std::vector<Vertice_type> boxIndexSetup (ZObject* pObject,const Color_type &pColor,const float pHigh,const float pWidth, const float pDepth)
{
std::vector<Vertice_type> wMids;
/* remark : coords must remain positive -> to be addressed */

    float wTopY = (pHigh /2.0f) ;
    float wLowY  =  - (pHigh /2.0f) ;

    float wLeftX = - (pWidth / 2.0f);

    float wRightX = (pWidth / 2.0f);

    float wFrontz =  (pDepth / 2.0f);
    float wBackz = - (pDepth / 2.0f);

    /* front face */

    ZVertice wFTL(Vertice_type  (wLeftX,wTopY,wFrontz),pColor);/* front Top Left corner */
    ZVertice wFTR(Vertice_type  (wRightX,wTopY,wFrontz),pColor);/* front Top Right corner */
    ZVertice wFLL (Vertice_type(wLeftX,wLowY,wFrontz),pColor);/* front Low Left corner */
    ZVertice wFLR (Vertice_type(wRightX,wLowY,wFrontz),pColor);/* front Low Right corner */

    Vertice_type wFLMid (wLeftX,(wTopY-wLowY)/2.0,wFrontz); /* Front left mid point */
    Vertice_type wFRMid (wRightX,(wTopY-wLowY)/2.0,wFrontz); /* Front right mid point */

    /* backward face */

    ZVertice wBTL (Vertice_type(wLeftX,wTopY,wBackz),pColor);/* Backward Top Left corner */
    ZVertice wBTR (Vertice_type(wRightX,wTopY,wBackz),pColor);/* Backward Top Right corner */
    ZVertice wBLL (Vertice_type(wLeftX,wLowY,wBackz),pColor);/* Backward Low Left corner */
    ZVertice wBLR (Vertice_type(wRightX,wLowY,wBackz),pColor);/* Backward Low Right corner */

    Vertice_type wBLMid (wLeftX,(wTopY-wLowY)/2.0,wBackz); /* Back left mid point */
    Vertice_type wBRMid (wRightX,(wTopY-wLowY)/2.0,wBackz); /* Back right mid point */

    wMids.push_back(wFLMid);
    wMids.push_back(wBLMid);
    wMids.push_back(wFRMid);
    wMids.push_back(wBRMid);
    /* clock wise */

    /* front face clock wise */
    *pObject << wFTL ;
    pObject->VertexName.push_back("FTL");
    int wFTLIdx= (int)pObject->lastVertexIdx();
    *pObject << wFTR ;
    pObject->VertexName.push_back("FTR");
    int wFTRIdx= pObject->lastVertexIdx();
    *pObject << wFLL ;
    pObject->VertexName.push_back("FLL");
    int wFLLIdx= pObject->lastVertexIdx();
    *pObject <<wFLR ;
    pObject->VertexName.push_back("FLR");
    int wFLRIdx= pObject->lastVertexIdx();

    /* back face clock wise */
    *pObject << wBTL ;
    pObject->VertexName.push_back("BTL");
    int wBTLIdx= pObject->lastVertexIdx();
    *pObject << wBTR ;
    pObject->VertexName.push_back("BTR");
    int wBTRIdx= pObject->lastVertexIdx();
    *pObject << wBLL ;
    pObject->VertexName.push_back("BLL");
    int wBLLIdx= pObject->lastVertexIdx();
    *pObject << wBLR ;
    pObject->VertexName.push_back("BLR");
    int wBLRIdx= pObject->lastVertexIdx();

    /* indices */
    /* front face */
    *pObject <<wFTLIdx;
    *pObject <<wFTRIdx;
    *pObject <<wFLRIdx;

    *pObject <<wFLRIdx;
    *pObject <<wFLLIdx;
    *pObject <<wFTLIdx;

    /* Backward face  */
    *pObject <<wBTLIdx;
    *pObject <<wBTRIdx;
    *pObject <<wBLRIdx;

    *pObject <<wBLRIdx;
    *pObject <<wBLLIdx;
    *pObject <<wBTLIdx;

    /* Down face  */
    *pObject <<wBLLIdx;
    *pObject <<wBLRIdx;
    *pObject <<wFLRIdx;

    *pObject <<wFLRIdx;
    *pObject <<wFLLIdx;
    *pObject <<wBLLIdx;

    /* Top face  */
    *pObject <<wBTLIdx;
    *pObject <<wBTRIdx;
    *pObject <<wFTRIdx;

    *pObject <<wFTRIdx;
    *pObject <<wFTLIdx;
    *pObject <<wBTLIdx;

    /* Left face  */

    *pObject <<wFTLIdx;
    *pObject <<wBTLIdx;
    *pObject <<wBLLIdx;

    *pObject <<wBLLIdx;
    *pObject <<wFLLIdx;
    *pObject <<wFTLIdx;

    /* Right face  */

    *pObject <<wFTRIdx;
    *pObject <<wBTRIdx;
    *pObject <<wBLRIdx;

    *pObject <<wBLRIdx;
    *pObject <<wFLRIdx;
    *pObject <<wFTRIdx;


    /* OK */

/* end indices */

    return wMids;
}
#endif // __USE_ZOBJECT__

/**
 * @brief perfect_arc_setup
 *
 *  Creates an arc of circle equals to half a circle beginning at pStart.
 *  Arc is ending at pEnd.
 *  Diameter of the circle is reputed to be the distance between pStart and pEnd
 *  Radius is then Diameter / 2.0
 *
 *
 * \href https://blog.lapingames.com/draw-circle-glsl-shader/
 *
 * @param pStart
 * @param pEnd
 * @param pDirection either 0 (forward) or (-1) backward
 * @param pNumber
 */
std::vector<Vertice_type> perfect_arc_setup(Vertice_type pCenter,float pRadiusLen, int pDirection, int pNumber)
{

//#define NUMBER_OF_VERTICES 16

double wRadius = pRadiusLen;
std::vector<Vertice_type> vertexBuffer;


Vertice_type wPoint;
double wIncrement = (double)pDirection * 2.0 *(double)M_PI /(double)pNumber ;
double wLimit = 2.0 * (double)M_PI;

/* left from PI / 2  (top- 90°) -> PI ->  to 3*PI / 2   (bottom - 270 °) */

/* rigth from  PI/2 (top-90°) -> 0 -> 3*PI/2 */

    vertexBuffer.push_back(pCenter); /* center is always first element */

    for(double i = 0.0; i < wLimit; i += wIncrement){
        wPoint.x=pCenter.x+(cos(i) * wRadius);
        wPoint.y=pCenter.y+(sin(i) * wRadius);
        wPoint.z=pCenter.z;
        vertexBuffer.push_back( wPoint);
        }// for
    return vertexBuffer;
}//perfect_arc_setup


ZObject generate_Arc(Vertice_type pCenter,float pRadiusLen, int pDirection, int pNumber)
{
ZObject wArc;
    std::vector<Vertice_type> vertexBuffer = perfect_arc_setup(pCenter,pRadiusLen,pDirection,pNumber);

    for (int wi=2;wi<vertexBuffer.size();wi++)
        {
        wArc << vertexBuffer[0];
        wArc << vertexBuffer[wi-1];
        wArc << vertexBuffer[wi];
        }

    return wArc;
}





void createGL_Object(std::vector<glm::vec3> &ObjectVertices,/* in vertices */
//                     std::vector<glm::vec3> &ObjectNormal,  /* out normal vectors */
                     unsigned int *ObjectVBO,               /* out vbo identifier */
                     unsigned int *ObjectVAO,               /* out vao identifier */
                     unsigned int *ObjectNormVBO)           /* out normal vbo identifier */
{
std::vector<glm::vec3> ObjectNormal;
const unsigned int apos=0;      /* shader position 0 apos */
const unsigned int anormal=1;     /* shader position 1 anormal */

//unsigned int ObjectVBO, ObjectVAO,  ObjectNormVBO;

    glGenVertexArrays(1, ObjectVAO);
    glGenBuffers(1, ObjectVBO);
//    glGenBuffers(1, &ObjectEBO);
// bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
    glBindVertexArray(*ObjectVAO);

    glBindBuffer(GL_ARRAY_BUFFER, *ObjectVBO);
//    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
//std::vector<glm::vec3> ObjectVertices = generateVVertex(Vvertices,Vindices);
//    int wVerticecount= sizeof(vertices)/sizeof(vertices[0]);
//    int wIndicecount = sizeof(indices)/sizeof(indices[0]);
//    std::vector<glm::vec3> ObjectVertices = generateVertex(vertices,wVerticecount,indices,wIndicecount);
    size_t wObjectSize=ObjectVertices.size()*sizeof(glm::vec3);
    glBufferData(GL_ARRAY_BUFFER, wObjectSize, ObjectVertices.data(), GL_STATIC_DRAW);

//   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ObjectEBO);
//   glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(apos, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(apos);

    ObjectNormal = generateVNormal(ObjectVertices);
    int wNormalSize=ObjectNormal.size()*sizeof(glm::vec3);

    for (long wi=0;wi<ObjectNormal.size();wi++)
    {
    printf ("%ld>> %f %f %f \n",wi,ObjectNormal[wi].x ,ObjectNormal[wi].y,ObjectNormal[wi].z);
    }

glGenBuffers(1, ObjectNormVBO);
//    glGenBuffers(1, &ObjectNormEBO);

glBindBuffer(GL_ARRAY_BUFFER, *ObjectNormVBO);
glBufferData(GL_ARRAY_BUFFER, wNormalSize, ObjectNormal.data(), GL_STATIC_DRAW);

//   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ObjectEBO);
//   glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW); // same indices as vertices

glVertexAttribPointer(anormal, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
glEnableVertexAttribArray(anormal);

// remember: do NOT unbind the EBO while a VAO is active as the bound element buffer object IS stored in the VAO; keep the EBO bound.
//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

// You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
// VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
    // unbind
// note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
glBindBuffer(GL_ARRAY_BUFFER, 0);

// unbind
glBindVertexArray(0);
}
