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

class ZShader
{    
public:
    GLuint ID;
    const char* Name=nullptr;
    int setErrorCount=0;
    const int MaxErrorDisplay=15;
    ZShader()=delete; /* no default constructor for this object */
    // constructor generates the shader on the fly
    // ------------------------------------------------------------------------
    ZShader(const char* pVertexPath, const char* pFragmentPath, const char* pName );
    ZShader(const char* pVertexPath, const char* pFragmentPath, const char* geometryPath,const char*pName);
    ZShader(std::string vertexPath, std::string fragmentPath, std::string geometryPath , const char *pName)
    {
        if (geometryPath.empty())
            ZShader(vertexPath.c_str(),fragmentPath.c_str(),pName);
        else
            ZShader(vertexPath.c_str(),fragmentPath.c_str(),geometryPath.c_str(),pName);
    }
    ZShader (const ZShader& pIn) {_cloneFrom(pIn);GLResources->registerShader(this);}
    ZShader& operator = (const ZShader& pIn) {_cloneFrom(pIn); GLResources->registerShader(this); return *this;}

    ~ZShader() {GLResources->deregisterShader(this);}


    void setupTexSampler(ZTexture* pTexture);

    void deleteGLContext()
    {
        glDeleteProgram(ID);
    }
private:
    void _cloneFrom(const ZShader& pIn)
    {
        ID=pIn.ID;
        Name=pIn.Name;
        setErrorCount=0;
    }
public:

    void setMaterial(const ZMaterial &pMaterial);
    void setLight(const ZLight& pLight);

    // activate the shader
    // ------------------------------------------------------------------------
    void use() 
    { 
        glUseProgram(ID); 
    }
    // release the shader
    // ------------------------------------------------------------------------
    void release()
    {
        glUseProgram(0);
    }

    GLuint getShaderId() {return ID;}


    void _displayErrorUniform(const char*pName)
    {
        if (setErrorCount > MaxErrorDisplay)
                                        return;

        if (setErrorCount == MaxErrorDisplay)
            fprintf (stderr,
                     "Shader-E-Uniform  Uniform name <%s> not found within shader id <%u> <%s>.\n Maximum error display count reached subsequent messages ignored for this shader....\n",
                     pName,
                     ID,
                     Name);
            else
            fprintf (stderr,
                     "Shader-E-Uniform  Uniform name <%s> not found within shader id <%u> <%s>\n",
                     pName,
                     ID,
                     Name);
        setErrorCount++;
    }//_displayErrorUniform


    void _displayErrorAttribute(const char*pName)
    {
        if (setErrorCount > MaxErrorDisplay)
                                        return;

        if (setErrorCount == MaxErrorDisplay)
            fprintf (stderr,
                     "Shader-E-Attribute  Attribute name <%s> not found within shader id <%u> <%s>.\n Maximum error display count reached subsequent messages ignored for this shader....\n",
                     pName,
                     ID,
                     Name);
            else
            fprintf (stderr,
                     "Shader-E-Attribute  Attribute name <%s> not found within shader id <%u> <%s>\n",
                     pName,
                     ID,
                     Name);
        setErrorCount++;
    }//_displayErrorAttribute

    // utility uniform functions
    // ------------------------------------------------------------------------
    void setBool(const char* pName, bool value)
    {
        int wLoc=glGetUniformLocation(ID, pName);
        if (wLoc<0)
                {
                _displayErrorUniform(pName);
                return;
                }

        glUniform1i(wLoc, (int)value);
    }
    // ------------------------------------------------------------------------
    void setInt(const char* pName, int value)
    { 
        int wLoc=glGetUniformLocation(ID, pName);
        if (wLoc<0)
                {
                _displayErrorUniform(pName);
                return;
                }
        glUniform1i(wLoc, value);
    }
    // ------------------------------------------------------------------------
    void setFloat(const char* pName, float value)
    { 
        int wLoc=glGetUniformLocation(ID, pName);
        if (wLoc<0)
                {
                _displayErrorUniform(pName);
                return;
                }
        glUniform1f(wLoc, value);
    }
    // ------------------------------------------------------------------------
    void setVec2(const char* pName, const glm::vec2 &value)
    { 
        int wLoc=glGetUniformLocation(ID, pName);
        if (wLoc<0)
                {
                _displayErrorUniform(pName);
                return;
                }
        glUniform2fv(wLoc, 1,  &value[0]);
    }
    void setVec2(const char* pName, float x, float y)
    { 
        int wLoc=glGetUniformLocation(ID, pName);
        if (wLoc<0)
                {
                _displayErrorUniform(pName);
                return;
                }
        glUniform2f(glGetUniformLocation(ID, pName), x, y);
    }
    // ------------------------------------------------------------------------
    void setVec3(const char* pName, const glm::vec3 &value)
    { 
        int wLoc=glGetUniformLocation(ID, pName);
        if (wLoc<0)
                {
                _displayErrorUniform(pName);
                return;;
                }
        glUniform3fv(wLoc, 1, &value[0]);
    }
    void setVec3(const char* pName, float x, float y, float z)
    { 
        int wLoc=glGetUniformLocation(ID, pName);
        if (wLoc<0)
                {
                _displayErrorUniform(pName);
                return;
                }
        glUniform3f(wLoc, x, y, z);
    }
    // ------------------------------------------------------------------------
    void setVec4(const char* pName, const glm::vec4 &value)
    { 
        int wLoc=glGetUniformLocation(ID, pName);
        if (wLoc<0)
                {
                _displayErrorUniform(pName);
                return;
                }
        glUniform4fv(wLoc, 1, &value[0]);
    }
    void setVec4(const char* pName, float x, float y, float z, float w)
    { 
        int wLoc=glGetUniformLocation(ID, pName);
        if (wLoc<0)
                {
                _displayErrorUniform(pName);
                return;;
                }
        glUniform4f(wLoc, x, y, z, w);
    }
    // ----------------------geometryPath--------------------------------------------------
    void setMat2(const char* pName, const glm::mat2 &mat)
    {
        int wLoc=glGetUniformLocation(ID, pName);
        if (wLoc<0)
                {
                _displayErrorUniform(pName);
                return;
                }
        glUniformMatrix2fv(wLoc, 1, GL_FALSE, &mat[0][0]);
    }
    // ------------------------------------------------------------------------
    void setMat3(const char* pName, const glm::mat3 &mat)
    {
        int wLoc=glGetUniformLocation(ID, pName);
        if (wLoc<0)
                {
                _displayErrorUniform(pName);
                return;
                }
        glUniformMatrix3fv(wLoc, 1, GL_FALSE, &mat[0][0]);
    }
    // ------------------------------------------------------------------------
    void setMat4(const char* pName, const glm::mat4 &mat)
    {
        int wLoc=glGetUniformLocation(ID, pName);
        if (wLoc<0)
                {
                _displayErrorUniform(pName);
                return;
                }
        glUniformMatrix4fv(wLoc, 1, GL_FALSE, &mat[0][0]);
    }
    void setMat4Transpose(const char* pName, const glm::mat4 &mat)
    {
        int wLoc=glGetUniformLocation(ID, pName);
        if (wLoc<0)
                {
                _displayErrorUniform(pName);
                return;
                }
        glUniformMatrix4fv(wLoc, 1, GL_TRUE, &mat[0][0]);
    }

    void setAttributeFloat(const char* pAttributeName,const float pValue) const
    {
        int wLoc=glGetAttribLocation(ID,pAttributeName);
        if (wLoc<0)
                {
                fprintf (stderr,
                         "Shader-E-Attribute  Attribute name <%s> not found within shader id <%u> <%s>\n",
                         pAttributeName,
                         ID,
                         Name);

                return;
                }
        glVertexAttrib1f((GLuint) wLoc,pValue);
    }
    /* Searches attribute of name pName or abort if not found and pAbortOnError option set
     * returns value of attribute location : 0 or positive if attribute is found, or negative value is not found
     * and pAbortOnError option not set
     */
    int getNamedAttributeLocation(const char* pName, bool pAbortOnError=false)
    {
        int wLoc=glGetAttribLocation(ID, pName);
        if (wLoc<0)
                {
                _displayErrorAttribute(pName);
                if (pAbortOnError)
                        abort();
                }
        return wLoc;
    }


private:
    // utility function for checking shader compilation/linking errors.
    // ------------------------------------------------------------------------
    void checkCompileErrors(GLuint shader, std::string type)
    {
        GLint success;
        GLchar infoLog[1024];
        if(type != "PROGRAM")
        {
            glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
            if(!success)
            {
                glGetShaderInfoLog(shader, 1024, NULL, infoLog);
                std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
            }
        }
        else
        {
            glGetProgramiv(shader, GL_LINK_STATUS, &success);
            if(!success)
            {
                glGetProgramInfoLog(shader, 1024, NULL, infoLog);
                std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
            }
        }
    }
};
#endif
