#ifndef ZSHADERCONTEXT_H
#define ZSHADERCONTEXT_H

#ifdef __USE_GLAD__
#include <glad/glad.h>
#else
#include <GL/glew.h>
#endif
#include <glm/glm.hpp>

#include <ztoolset/zarray.h>

#include <zglconstants.h>
#include <zshader.h>

enum ShaderUniform_type:uint16_t {
    SHU_Nothing         = 0,
    SHU_Bool            = 1,
    SHU_Float           = 2,
    SHU_Vec2            = 0x1004,
    SHU_Vec3            = 0x1008,
    SHU_Vec4            = 0x1010,
    SHU_Mat2            = 0x1020,
    SHU_Mat3            = 0x1040,
    SHU_Mat4            = 0x1080,
    SHU_Mat4Transpose   = 0x1100,

    SHU_Texture         = 0x3000,
    SHU_Material        = 0x5000,
    SHU_Light           = 0x9000,

    SHU_LineWidth        = 0x1400, /* special case for glLineSize() */

    SHU_IsPointer       = 0x1000
};

class ZTexture;
class ZLight;
class ZMaterial;

class ShaderUniform {
public:
    const char* Name=nullptr;
    ShaderUniform_type Type;
    union {
        void*       Void;   /* pointer is stored here */
        bool        Bool;   /* value is stored here */
        float       Float;  /* value is stored here */
        const glm::vec2*  Vec2;   /* pointer is stored here SHU_IsPointer  */
        const glm::vec3*  Vec3;   /* pointer is stored here SHU_IsPointer*/
        const glm::vec4*  Vec4;   /* pointer is stored here SHU_IsPointer */
        glm::mat2*  Mat2;   /* pointer is stored here SHU_IsPointer */
        glm::mat3*  Mat3;   /* pointer is stored here SHU_IsPointer */
        glm::mat4*  Mat4;   /* pointer is stored here SHU_IsPointer */
        const ZTexture*     Texture;
        const ZMaterial*    Material;
        const ZLight*       Light;
    } Value;

    void addTexture(ZTexture* pValue);
    void addLight(const ZLight* pValue);
    void addMaterial(const ZMaterial* pValue);

    void addBool(const char*pName, bool pValue);
    void addFloat(const char*pName, float pValue);


    void addVec2(const char*pName, const glm::vec2* pValue);
    void addVec3(const char*pName, const glm::vec3* pValue);
    void addVec4(const char*pName, const glm::vec4* pValue);

    void addMat2(const char*pName, glm::mat2* pValue);
    void addMat3(const char*pName, glm::mat3* pValue);
    void addMat4(const char*pName, glm::mat4* pValue);
    void addMat4Transpose(const char*pName, glm::mat4* pValue);

    void addLineWidth(float pLineSize) ;

    void add(ShaderUniform_type pType, const char*pName, void* pValue);

    void print(FILE*pOutput=stdout);
};



//class ZShader;
int _applyShader(ZShader* Shader,struct ShaderUniform pSHU );
int setupShader(ZShader& pShader,zbs::ZArray<ShaderUniform>& pParams);


/* Contains data that are dependent from Draw operation, one per type of draw (actually Draw, Shape and Normal Visualization.
 * This class holds necessary information to draw prepared GL data for the requested drawing action type :
 * - shader
 * - Texture
 * - type of drawing : GL_TRIANGLES, or GL_LINES or ...
 * - uniforms that might have been set to shader before glDraw in invoked
 *      for instance
 *          - drawing Color
 *          - BlinnPhong effect
 *          - etc.
 *
 *  Remarks :
 *  - uniform values do NOT include matrices that are managed separately.
 *  - object position is a stable external data
 *  - any matrice dependent operation is external to this class
 *
 *  Shader is appropriately setup with these rules just before glDraw is invoked for the object or meta object components
 *
 *  a ZShaderContext is held at meta object level, or might be overriden by a local instance for a particular component
 *
 * A ZShaderContext is dependent from the kind of draw action perform :
 * - draw of the object
 * - shape draw
 * - normal vector visualization draw
 */
class ZShaderContext: public zbs::ZArray<ShaderUniform>
{
    friend class ZMetaObject;
    friend class ZObject;
    friend class ZGLUnicodeText;
    friend class ZGLTextWriter;
    friend class ZTextBox;
private:
    typedef zbs::ZArray<ShaderUniform> _Base;
    ZShaderContext& _cloneFrom(ZShaderContext&pIn);

    ZShaderContext(ZShader*pSh)     {Shader=pSh;}

public:
    ZShaderContext()=delete ;
    ZShaderContext(ZShaderContext&pIn) {_cloneFrom(pIn);}
    ZShaderContext(ZShaderContext&&pIn) {_cloneFrom(pIn);}

    ~ZShaderContext();

    ZShaderContext& operator =(ZShaderContext&pIn)  {_cloneFrom(pIn); return *this;}
    ZShaderContext& operator =(ZShaderContext&&pIn)  {_cloneFrom(pIn); return *this;}

//    int setShader(ZShader* pShader);
    int setShader(ZShader *pShader);
    /* do not use below : only to copy a texture from one context to another */
    void setTexture(ZTexture* pValue);


    void useShader() {Shader->use();}

    int setTextureByName(const char* pName);
    int setTextureByRank(const long pRank);

    ShaderUniform* getUniformByName(const char*pName);
    ShaderUniform* getUniformByNameCase(const char*pName);

    int getAttributeLocation(const char*pName, bool pAbortOnError=false) {return Shader->getNamedAttributeLocation(pName,pAbortOnError);}


    int getPositionAttributeLocation ( bool pAbortOnError=false) {return Shader->getPositionAttributeLocation(pAbortOnError);}
    int getNormalAttributeLocation ( bool pAbortOnError=false) {return Shader->getNormalAttributeLocation(pAbortOnError);}
    int getTextureAttributeLocation ( bool pAbortOnError=false) {return Shader->getTextureAttributeLocation(pAbortOnError);}

// DrawFigure is defined apart from Shader context
 //   void setDrawFigure(GLenum pDrawFigure) {DrawFigure=pDrawFigure;}

    void addMaterial(const ZMaterial* pValue)
        {
        ShaderUniform wSU;
        wSU.addMaterial(pValue);
        push(wSU);
        }
    void addLight(const ZLight* pValue)
        {
        ShaderUniform wSU;
        wSU.addLight(pValue);
        push(wSU);
        }

    void addBool(const char*pName, bool pValue)
    {
        ShaderUniform wSU;
        wSU.addBool(pName,pValue);
        push(wSU);
    }
    void addFloat(const char*pName, float pValue)
    {
        ShaderUniform wSU;
        wSU.addFloat(pName,pValue);
        push(wSU);
    }

    void addVec2(const char*pName, glm::vec2* pValue)
    {
        ShaderUniform wSU;
        wSU.addVec2(pName,pValue);
        push(wSU);
    }
    void addVec3(const char*pName, const glm::vec3* pValue)
    {
        ShaderUniform wSU;
        wSU.addVec3(pName,pValue);
        push(wSU);
    }

    void addVec4(const char*pName, glm::vec4* pValue)
    {
        ShaderUniform wSU;
        wSU.addVec4(pName,pValue);
        push(wSU);
    }
    void addMat2(const char*pName, glm::mat2* pValue)
    {
        ShaderUniform wSU;
        wSU.addMat2(pName,pValue);
        push(wSU);
    }
    void addMat3(const char*pName, glm::mat3* pValue)
    {
        ShaderUniform wSU;
        wSU.addMat3(pName,pValue);
        push(wSU);
    }
    void addMat4(const char*pName, glm::mat4* pValue)
    {
        ShaderUniform wSU;
        wSU.addMat4(pName,pValue);
        push(wSU);
    }
    void addMat4Transpose(const char*pName, glm::mat4* pValue)
    {
        ShaderUniform wSU;
        wSU.addMat4Transpose(pName,pValue);
        push(wSU);
    }

    void setLineWidth(float pLineWidth)
    {
        ShaderUniform wSU;
        wSU.addLineWidth(pLineWidth);
        push(wSU);
    }

    void add(ShaderUniform_type pType, const char*pName, void* pValue)
    {
        ShaderUniform wSU;
        wSU.add(pType,pName,pValue);
        push(wSU);
    }

    void display(FILE*pOutput=stdout);

    /* applies all existing rules to shader in the order they have been declared */
    int applyRules();

    inline int _applyShader(ShaderUniform pSHU );

    int setupShader(ZShader& pShader,zbs::ZArray<ShaderUniform>& pParams);
    /* applies post processing, and release shader */
    void postProcess();

    ZTexture*   getTexture();
    bool        hasTexture();
    int         bindTexture();
    void        releaseTexture();

    bool        hasShader() {return Shader!=nullptr;}
    int         bindShader();
    void        releaseShader();

    GLuint getPositionAttribute();
    GLuint getNormalAttribute();
    GLuint getTexCoordsAttribute();

    const char* getShaderName()
    {
        if (Shader==nullptr)
                return nullptr;
        return Shader->getName();
    }

private:
    int _addTexture(ZTexture* pValue);

    ZTexture*   Texture=nullptr;
    ZShader*    Shader=nullptr;

    float       LineWidth_restore;
    bool        LineWidth_IsSet=false;

    int         ErrorCount=0;

// DrawFigure is dependant from context draw however independant from ZShaderContext
// see GLenum      DrawFigure[3]={GL_ZERO,GL_ZERO,GL_ZERO}; within ZObject & ZMetaObject
//    GLenum      DrawFigure=GL_TRIANGLES;

};// ZShaderContext

const char* decodeShdCtx(const int pCtx);

#endif // ZSHADERCONTEXT_H
