#ifndef SHADER_H
#define SHADER_H

#ifdef __USE_GLAD__
#include <glad/glad.h>
#else
#include <GL/glew.h>
#endif
#include <glm/glm.hpp>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <string.h> // for strerror

#include <zglresource.h>
#include <zmaterial.h>
#include <zglconstants.h>

static constexpr int MaxShaderErrorDisplay=15;


class ZShader;

class _ShaderBase
{
public:

    GLuint GLId=0;
    const char* Name=nullptr;
    int    Shared=0;
    int setUniformErrorCount=0;
    int setAttributeErrorCount=0;

    _ShaderBase()=default;
    ~_ShaderBase()
        {
            glDeleteProgram(GLId);
        }

    ZShader  share();
    ZShader* sharePtr();
    bool unShare() {Shared--; if (Shared<0)return true; return false;}

};

_ShaderBase* _InitShader(const char* pVertexPath,
                     const char* pFragmentPath,
                     const char* pInlName );
_ShaderBase* _InitShaderGeometry(const char* pVertexPath,
                             const char* pFragmentPath,
                             const char* geometryPath,
                             const char*pInlName);

//typedef _Shader* _ShaderPtr;

class ZShader
{    
friend class ZGLResource;

private:
    _ShaderBase* ShaderBase=nullptr;

    void _cloneFrom(const ZShader& pIn)
    {
        ShaderBase=pIn.ShaderBase;
        if (ShaderBase)
                ShaderBase->Shared++;
    }

public:
    ZShader()=delete; /* no default constructor for this object */

    ZShader(_ShaderBase* pBSh);
    ZShader (const ZShader& pIn) {_cloneFrom(pIn);}
    ZShader (const ZShader&& pIn) {_cloneFrom(pIn);}
    ZShader& operator = (const ZShader& pIn) {_cloneFrom(pIn); return *this;}

    ~ZShader()
    {
        GLResources->deregisterShaderBase(ShaderBase);
    }

    ZShader share() {return ShaderBase->share();}
    ZShader* sharePtr() {return ShaderBase->sharePtr();}


    bool isViable()
    {
        if (!ShaderBase)
            return false;
        return ShaderBase->GLId!=0 ;
    }
    // constructor generates the shader on the fly
    // ------------------------------------------------------------------------
/*    ZShader(const char* pVertexPath, const char* pFragmentPath, const char* pName );
    ZShader(const char* pVertexPath, const char* pFragmentPath, const char* geometryPath,const char*pName);
    ZShader(std::string vertexPath, std::string fragmentPath, std::string geometryPath , const char *pName)
    {
        if (geometryPath.empty())
            ZShader(vertexPath.c_str(),fragmentPath.c_str(),pName);
        else
            ZShader(vertexPath.c_str(),fragmentPath.c_str(),geometryPath.c_str(),pName);
    }
    */

    int setupTexSampler(const ZTexture *pTexture);



public:
/* don't forget invoking use() before setting any value for shaders */

    int setupMaterial(const ZMaterial *pMaterial);
    int setupLight(const ZLight *pLight);

    // activate the shader
    // ------------------------------------------------------------------------
    void use() 
    { 
        glUseProgram(ShaderBase->GLId);
    }
    // release the shader
    // ------------------------------------------------------------------------
    void release()
    {
        glUseProgram(0);
    }

    GLuint getShaderGLId() {return ShaderBase->GLId;}


    void _displayErrorUniform(const char*pName)
    {
        if (ShaderBase->setUniformErrorCount > MaxShaderErrorDisplay)
                                        return;

        if (ShaderBase->setUniformErrorCount == MaxShaderErrorDisplay)
            fprintf (stderr,
                     "Shader-E-Uniform  Uniform name <%s> not found within shader id <%u> <%s>.\n Maximum error display count reached subsequent messages ignored for this shader....\n",
                     pName,
                     ShaderBase->GLId,
                     ShaderBase->Name);
            else
            fprintf (stderr,
                     "Shader-E-Uniform  Uniform name <%s> not found within shader id <%u> <%s>\n",
                     pName,
                     ShaderBase->GLId,
                     ShaderBase->Name);
        ShaderBase->setUniformErrorCount++;
    }//_displayErrorUniform


    void _displayErrorAttribute(const char*pName)
    {
        if (ShaderBase->setAttributeErrorCount > MaxShaderErrorDisplay)
                                        return;

        if (ShaderBase->setAttributeErrorCount == MaxShaderErrorDisplay)
            fprintf (stderr,
                     "Shader-E-Attribute  Attribute name <%s> not found within shader id <%u> <%s>.\n Maximum error display count reached subsequent messages ignored for this shader....\n",
                     pName,
                     ShaderBase->GLId,
                     ShaderBase->Name);
            else
            fprintf (stderr,
                     "Shader-E-Attribute  Attribute name <%s> not found within shader id <%u> <%s>\n",
                     pName,
                     ShaderBase->GLId,
                     ShaderBase->Name);
        ShaderBase->setAttributeErrorCount++;
    }//_displayErrorAttribute

    // utility uniform functions
    // ------------------------------------------------------------------------
    bool hasUniform(const char* pName)
    {
    return glGetUniformLocation(ShaderBase->GLId, pName)>=0;
    }

    int setBool(const char* pName, bool value)
    {
        int wLoc=glGetUniformLocation(ShaderBase->GLId, pName);
        if (wLoc<0)
                {
                _displayErrorUniform(pName);
                return wLoc;
                }

        glUniform1i(wLoc, (int)value);
        return wLoc;
    }
    // ------------------------------------------------------------------------
    int setInt(const char* pName, int value)
    { 
        int wLoc=glGetUniformLocation(ShaderBase->GLId, pName);
        if (wLoc<0)
                {
                _displayErrorUniform(pName);
                return wLoc;
                }
        glUniform1i(wLoc, value);
        return wLoc;
    }
    // ------------------------------------------------------------------------
    int setFloat(const char* pName, float value)
    { 
        int wLoc=glGetUniformLocation(ShaderBase->GLId, pName);
        if (wLoc<0)
                {
                _displayErrorUniform(pName);
                return wLoc;
                }
        glUniform1f(wLoc, value);
        return wLoc;
    }
    // ------------------------------------------------------------------------
    int setVec2(const char* pName, const glm::vec2 &value)
    { 
        int wLoc=glGetUniformLocation(ShaderBase->GLId, pName);
        if (wLoc<0)
                {
                _displayErrorUniform(pName);
                return wLoc;
                }
        glUniform2fv(wLoc, 1,  &value[0]);
        return wLoc;
    }
    int setVec2(const char* pName, float x, float y)
    { 
        int wLoc=glGetUniformLocation(ShaderBase->GLId, pName);
        if (wLoc<0)
                {
                _displayErrorUniform(pName);
                return wLoc;
                }
        glUniform2f(glGetUniformLocation(ShaderBase->GLId, pName), x, y);
        return wLoc;
    }
    // ------------------------------------------------------------------------
    int setVec3(const char* pName, const glm::vec3 &value)
    { 
        int wLoc=glGetUniformLocation(ShaderBase->GLId, pName);
        if (wLoc<0)
                {
                _displayErrorUniform(pName);
                return wLoc;
                }
        glUniform3fv(wLoc, 1, &value[0]);
        return wLoc;
    }
    int setVec3(const char* pName, float x, float y, float z)
    { 
        int wLoc=glGetUniformLocation(ShaderBase->GLId, pName);
        if (wLoc<0)
                {
                _displayErrorUniform(pName);
                return wLoc;
                }
        glUniform3f(wLoc, x, y, z);
        return wLoc;
    }
    // ------------------------------------------------------------------------
    int setVec4(const char* pName, const glm::vec4 &value)
    { 
        int wLoc=glGetUniformLocation(ShaderBase->GLId, pName);
        if (wLoc<0)
                {
                _displayErrorUniform(pName);
                return wLoc;
                }
        glUniform4fv(wLoc, 1, &value[0]);
        return wLoc;
    }
    int setVec4(const char* pName, float x, float y, float z, float w)
    { 
        int wLoc=glGetUniformLocation(ShaderBase->GLId, pName);
        if (wLoc<0)
                {
                _displayErrorUniform(pName);
                return wLoc;
                }
        glUniform4f(wLoc, x, y, z, w);
        return wLoc;
    }
    // ----------------------geometryPath--------------------------------------------------
    int setMat2(const char* pName, const glm::mat2 &mat)
    {
        int wLoc=glGetUniformLocation(ShaderBase->GLId, pName);
        if (wLoc<0)
                {
                _displayErrorUniform(pName);
                return wLoc;
                }
        glUniformMatrix2fv(wLoc, 1, GL_FALSE, &mat[0][0]);
        return wLoc;
    }
    // ------------------------------------------------------------------------
    int setMat3(const char* pName, const glm::mat3 &mat)
    {
        int wLoc=glGetUniformLocation(ShaderBase->GLId, pName);
        if (wLoc<0)
                {
                _displayErrorUniform(pName);
                return wLoc;
                }
        glUniformMatrix3fv(wLoc, 1, GL_FALSE, &mat[0][0]);
        return wLoc;
    }
    // ------------------------------------------------------------------------
    int setMat4(const char* pName, const glm::mat4 &mat)
    {
        int wLoc=glGetUniformLocation(ShaderBase->GLId, pName);
        if (wLoc<0)
                {
                _displayErrorUniform(pName);
                return wLoc;
                }
        glUniformMatrix4fv(wLoc, 1, GL_FALSE, &mat[0][0]);
        return wLoc;
    }
    int setMat4Transpose(const char* pName, const glm::mat4 &mat)
    {
        int wLoc=glGetUniformLocation(ShaderBase->GLId, pName);
        if (wLoc<0)
                {
                _displayErrorUniform(pName);
                return wLoc;
                }
        glUniformMatrix4fv(wLoc, 1, GL_TRUE, &mat[0][0]);
        return wLoc;
    }

    int setAttributeFloat(const char* pAttributeName,const float pValue) const
    {
        int wLoc=glGetAttribLocation(ShaderBase->GLId,pAttributeName);
        if (wLoc<0)
                {
                fprintf (stderr,
                         "Shader-E-Attribute  Attribute name <%s> not found within shader id <%u> <%s>\n",
                         pAttributeName,
                         ShaderBase->GLId,
                         ShaderBase->Name);

                return wLoc;
                }
        glVertexAttrib1f((GLuint) wLoc,pValue);
        return wLoc;
    }

    bool hasAttribute(const char* pName)
    {
    return glGetAttribLocation(ShaderBase->GLId, pName)>=0;
    }

    /* Searches attribute of name pName or abort if not found and pAbortOnError option set
     * returns value of attribute location : 0 or positive if attribute is found, or negative value is not found
     * and pAbortOnError option not set
     */
    int getNamedAttributeLocation(const char* pName, bool pAbortOnError=false)
    {
        int wLoc=glGetAttribLocation(ShaderBase->GLId, pName);
        if (wLoc<0)
                {
                _displayErrorAttribute(pName);
                if (pAbortOnError)
                        abort();
                }
        return wLoc;
    }
    int getPositionAttributeLocation( bool pAbortOnError=false)
    {
        return getNamedAttributeLocation(__SHD_POSITION_ATTR__,pAbortOnError);
    }
    int getNormalAttributeLocation( bool pAbortOnError=false)
    {
        int wN=getNamedAttributeLocation(__SHD_NORMAL_ATTR__,pAbortOnError);
        if (wN<0)
                NormalCoordsExist=false;
        return wN;
    }
    int getTextureAttributeLocation( bool pAbortOnError=false)
    {
        int wN=getNamedAttributeLocation(__SHD_TEXCOORDS_ATTR__,pAbortOnError);
        if (wN<0)
                TextureCoordsExist=false;
        return wN;
    }
    const char* getName() {return ShaderBase->Name;}

    bool hasNormalCoords() {return NormalCoordsExist;}
    bool hasTextureCoords() {return TextureCoordsExist;}

private:

    bool NormalCoordsExist=true;
    bool TextureCoordsExist=true;


};


#endif
