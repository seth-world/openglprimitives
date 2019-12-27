#include <zshader.h>

#include <ztexture.h>
#include <zglconstants.h>

void _checkCompileErrors(GLuint ID, std::string type)
{
    GLint success=0;
    GLchar infoLog[1024];
    if(type != "PROGRAM")
    {
        glGetShaderiv(ID, GL_COMPILE_STATUS, &success);
        if(!success)
        {
            glGetShaderInfoLog(ID, 1024, NULL, infoLog);
            std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
        }
    }
    else
    {
        glGetProgramiv(ID, GL_LINK_STATUS, &success);
        if(!success)
        {
            glGetProgramInfoLog(ID, 1024, NULL, infoLog);
            std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
        }
    }
}


_ShaderBase* _InitShader(const char* pVertexPath, const char* pFragmentPath, const char* pInlName )
{

    std::string wVertexPath=GLResources->getShaderPath(pVertexPath).c_str();
    std::string wFragmentPath=GLResources->getShaderPath(pFragmentPath).c_str();
    const char* wCurrentPath=nullptr;
    // 1. retrieve the vertex/fragment source code from filePath
    std::string vertexCode;
    std::string fragmentCode;
    std::string geometryCode;
    std::ifstream vShaderFile;
    std::ifstream fShaderFile;
    std::ifstream gShaderFile;
    // ensure ifstream objects can throw exceptions:
    vShaderFile.exceptions (std::ifstream::failbit | std::ifstream::badbit);
    fShaderFile.exceptions (std::ifstream::failbit | std::ifstream::badbit);
    gShaderFile.exceptions (std::ifstream::failbit | std::ifstream::badbit);

    fprintf(stdout,"\nVertex path <%s> \nFragment path <%s>\n",wVertexPath.c_str(),wFragmentPath.c_str());
    try
    {
        std::stringstream vShaderStream, fShaderStream;

        wCurrentPath=wVertexPath.c_str();
        vShaderFile.open(wVertexPath);
        vShaderStream << vShaderFile.rdbuf();
        vShaderFile.close();

        wCurrentPath=wFragmentPath.c_str();
        fShaderFile.open(wFragmentPath);
        fShaderStream << fShaderFile.rdbuf();
        fShaderFile.close();

        // convert stream into string
        vertexCode = vShaderStream.str();
        fragmentCode = fShaderStream.str();
        // if geometry shader path is present, also load a geometry shader
    }
    catch (std::ifstream::failure& e)
    {
        fprintf (stderr,
                 "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ error on file <%s>\n"
                 "       error code <%d> description <%s>\n",
                 wCurrentPath,
                 errno,
                 strerror(errno));
//                     e.code(),
//                     e.what());
        return nullptr;
    }//catch
    const char* vShaderCode = vertexCode.c_str();
    const char * fShaderCode = fragmentCode.c_str();
    // 2. compile shaders
    unsigned int vertex, fragment;
    // vertex shader
    vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vShaderCode, nullptr);
    glCompileShader(vertex);
    _checkCompileErrors(vertex, "VERTEX");
    // fragment Shader
    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fShaderCode, nullptr);
    glCompileShader(fragment);
    _checkCompileErrors(fragment, "FRAGMENT");
    // if geometry shader is given, compile geometry shader

    _ShaderBase* wShader=new _ShaderBase;
    wShader->Name=pInlName;

    // shader Program

/*    GLuint wS = glCreateProgram();
    glAttachShader(wS, vertex);
    glAttachShader(wS, fragment);
    glLinkProgram(wS);
    _checkCompileErrors(wS, "PROGRAM");*/
    wShader->GLId = glCreateProgram();
    glAttachShader(wShader->GLId, vertex);
    glAttachShader(wShader->GLId, fragment);
    glLinkProgram(wShader->GLId);
    _checkCompileErrors(wShader->GLId, "PROGRAM");
    // delete the shaders as they're linked into our program now and no longer necessary
    glDeleteShader(vertex);
    glDeleteShader(fragment);

//    glUseProgram(wShader->ID);

    printf("Shaders Name <%s> sucessfully loaded with id <%u>.\n",
           wShader->Name,
           wShader->GLId);

    return wShader;
}// _InitShader

_ShaderBase* _InitShaderGeometry(const char* pVertexPath,
                             const char* pFragmentPath,
                             const char* pGeometryPath ,
                             const char* pInlName)
{
    std::string wVertexPath=GLResources->getShaderPath(pVertexPath).c_str();
    std::string wFragmentPath=GLResources->getShaderPath(pFragmentPath).c_str();
    const char* wCurrentPath=nullptr;
    // 1. retrieve the vertex/fragment source code from filePath
    std::string vertexCode;
    std::string fragmentCode;
    std::string geometryCode;
    std::ifstream vShaderFile;
    std::ifstream fShaderFile;
    std::ifstream gShaderFile;
    // ensure ifstream objects can throw exceptions:
    vShaderFile.exceptions (std::ifstream::failbit | std::ifstream::badbit);
    fShaderFile.exceptions (std::ifstream::failbit | std::ifstream::badbit);
    gShaderFile.exceptions (std::ifstream::failbit | std::ifstream::badbit);

    fprintf(stdout,"Vertex path <%s> \nFragment path <%s>\n",wVertexPath.c_str(),wFragmentPath.c_str());
    try
    {
        std::stringstream vShaderStream, fShaderStream;

        wCurrentPath=wVertexPath.c_str();
        vShaderFile.open(wVertexPath);
        vShaderStream << vShaderFile.rdbuf();
        vShaderFile.close();

        wCurrentPath=wFragmentPath.c_str();
        fShaderFile.open(wFragmentPath);
        fShaderStream << fShaderFile.rdbuf();
        fShaderFile.close();

        // convert stream into string
        vertexCode = vShaderStream.str();
        fragmentCode = fShaderStream.str();
        // if geometry shader path is present, also load a geometry shader

        std::string wgeometryPath=GLResources->getShaderPath(pGeometryPath);
        wCurrentPath=wgeometryPath.c_str();
        gShaderFile.open(wgeometryPath);
        std::stringstream gShaderStream;
        gShaderStream << gShaderFile.rdbuf();
        gShaderFile.close();
        geometryCode = gShaderStream.str();

    }
    catch (std::ifstream::failure e)
    {
        fprintf (stderr,
                 "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ error on file <%s>\n"
                 "       error code <%d> description <%s>\n",
                 wCurrentPath,
                 errno,
                 strerror(errno));
//                     e.code(),
//                     e.what());
        return nullptr;
    }//catch
    const char* vShaderCode = vertexCode.c_str();
    const char * fShaderCode = fragmentCode.c_str();
    // 2. compile shaders
    unsigned int vertex, fragment;
    // vertex shader
    vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vShaderCode, nullptr);
    glCompileShader(vertex);
    _checkCompileErrors(vertex, "VERTEX");
    // fragment Shader
    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fShaderCode, nullptr);
    glCompileShader(fragment);
    _checkCompileErrors(fragment, "FRAGMENT");
    // if geometry shader is given, compile geometry shader
    unsigned int geometry;
    const char * gShaderCode = geometryCode.c_str();
    geometry = glCreateShader(GL_GEOMETRY_SHADER);
    glShaderSource(geometry, 1, &gShaderCode, nullptr);
    glCompileShader(geometry);
    _checkCompileErrors(geometry, "GEOMETRY");

    _ShaderBase* wShader=new _ShaderBase;
    wShader->Name=pInlName;
    // shader Program
    wShader->GLId = glCreateProgram();
    glAttachShader(wShader->GLId, vertex);
    glAttachShader(wShader->GLId, fragment);
    if(pGeometryPath != nullptr)
        glAttachShader(wShader->GLId, geometry);
    glLinkProgram(wShader->GLId);
    _checkCompileErrors(wShader->GLId, "PROGRAM");
    // delete the shaders as they're linked into our program now and no longer necessary
    glDeleteShader(vertex);
    glDeleteShader(fragment);
    glDeleteShader(geometry);

    printf("Shaders Name <%s> (with geometry) sucessfully loaded with id <%u>\n Paths <%s> <%s> <%s>  .\n",
           wShader->Name,
           wShader->GLId,
           pVertexPath,
           pFragmentPath,
           pGeometryPath);
    return wShader;
}//_InitShaderGeometry

ZShader _ShaderBase::share()
{
    Shared++;
    return ZShader(this);
}

ZShader* _ShaderBase::sharePtr()
{
    Shared++;
    return new ZShader(this);
}
ZShader::ZShader(_ShaderBase* pBSh)
{
    ShaderBase=pBSh;
}
/*
ZShader::ZShader(const char* pVertexPath, const char* pFragmentPath, const char* pName )
{
    ShaderBase->Name=pName;
    std::string wVertexPath=GLResources->getShaderPath(pVertexPath).c_str();
    std::string wFragmentPath=GLResources->getShaderPath(pFragmentPath).c_str();
    const char* wCurrentPath=nullptr;
    // 1. retrieve the vertex/fragment source code from filePath
    std::string vertexCode;
    std::string fragmentCode;
    std::string geometryCode;
    std::ifstream vShaderFile;
    std::ifstream fShaderFile;
    std::ifstream gShaderFile;
    // ensure ifstream objects can throw exceptions:
    vShaderFile.exceptions (std::ifstream::failbit | std::ifstream::badbit);
    fShaderFile.exceptions (std::ifstream::failbit | std::ifstream::badbit);
    gShaderFile.exceptions (std::ifstream::failbit | std::ifstream::badbit);

    fprintf(stdout,"\nVertex path <%s> \nFragment path <%s>\n",wVertexPath.c_str(),wFragmentPath.c_str());
    try
    {
        std::stringstream vShaderStream, fShaderStream;

        wCurrentPath=wVertexPath.c_str();
        vShaderFile.open(wVertexPath);
        vShaderStream << vShaderFile.rdbuf();
        vShaderFile.close();

        wCurrentPath=wFragmentPath.c_str();
        fShaderFile.open(wFragmentPath);
        fShaderStream << fShaderFile.rdbuf();
        fShaderFile.close();

        // convert stream into string
        vertexCode = vShaderStream.str();
        fragmentCode = fShaderStream.str();
        // if geometry shader path is present, also load a geometry shader
    }
    catch (std::ifstream::failure e)
    {
        fprintf (stderr,
                 "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ error on file <%s>\n"
                 "       error code <%d> description <%s>\n",
                 wCurrentPath,
                 errno,
                 strerror(errno));
//                     e.code(),
//                     e.what());
        return;
    }//catch
    const char* vShaderCode = vertexCode.c_str();
    const char * fShaderCode = fragmentCode.c_str();
    // 2. compile shaders
    unsigned int vertex, fragment;
    // vertex shader
    vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vShaderCode, nullptr);
    glCompileShader(vertex);
    checkCompileErrors(vertex, "VERTEX");
    // fragment Shader
    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fShaderCode, nullptr);
    glCompileShader(fragment);
    checkCompileErrors(fragment, "FRAGMENT");
    // if geometry shader is given, compile geometry shader

    // shader Program
    ShaderBase->ID = glCreateProgram();
    glAttachShader(ShaderBase->ID, vertex);
    glAttachShader(ShaderBase->ID, fragment);
    glLinkProgram(ShaderBase->ID);
    checkCompileErrors(ShaderBase->ID, "PROGRAM");
    // delete the shaders as they're linked into our program now and no longer necessery
    glDeleteShader(vertex);
    glDeleteShader(fragment);

    printf("Shaders Name <%s> sucessfully loaded with id <%u>.\n",
           ShaderBase->Name,
           ShaderBase->ID);
}// ZShader CTOR

ZShader::ZShader(const char* pVertexPath, const char* pFragmentPath, const char* pGeometryPath ,const char* pName)
{
    ShaderBase->Name=pName;
    std::string wVertexPath=GLResources->getShaderPath(pVertexPath).c_str();
    std::string wFragmentPath=GLResources->getShaderPath(pFragmentPath).c_str();
    const char* wCurrentPath=nullptr;
    // 1. retrieve the vertex/fragment source code from filePath
    std::string vertexCode;
    std::string fragmentCode;
    std::string geometryCode;
    std::ifstream vShaderFile;
    std::ifstream fShaderFile;
    std::ifstream gShaderFile;
    // ensure ifstream objects can throw exceptions:
    vShaderFile.exceptions (std::ifstream::failbit | std::ifstream::badbit);
    fShaderFile.exceptions (std::ifstream::failbit | std::ifstream::badbit);
    gShaderFile.exceptions (std::ifstream::failbit | std::ifstream::badbit);

    fprintf(stdout,"Vertex path <%s> \nFragment path <%s>\n",wVertexPath.c_str(),wFragmentPath.c_str());
    try
    {
        std::stringstream vShaderStream, fShaderStream;

        wCurrentPath=wVertexPath.c_str();
        vShaderFile.open(wVertexPath);
        vShaderStream << vShaderFile.rdbuf();
        vShaderFile.close();

        wCurrentPath=wFragmentPath.c_str();
        fShaderFile.open(wFragmentPath);
        fShaderStream << fShaderFile.rdbuf();
        fShaderFile.close();

        // convert stream into string
        vertexCode = vShaderStream.str();
        fragmentCode = fShaderStream.str();
        // if geometry shader path is present, also load a geometry shader

        std::string wgeometryPath=GLResources->getShaderPath(pGeometryPath);
        wCurrentPath=wgeometryPath.c_str();
        gShaderFile.open(wgeometryPath);
        std::stringstream gShaderStream;
        gShaderStream << gShaderFile.rdbuf();
        gShaderFile.close();
        geometryCode = gShaderStream.str();

    }
    catch (std::ifstream::failure e)
    {
        fprintf (stderr,
                 "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ error on file <%s>\n"
                 "       error code <%d> description <%s>\n",
                 wCurrentPath,
                 errno,
                 strerror(errno));
//                     e.code(),
//                     e.what());
        return;
    }//catch
    const char* vShaderCode = vertexCode.c_str();
    const char * fShaderCode = fragmentCode.c_str();
    // 2. compile shaders
    unsigned int vertex, fragment;
    // vertex shader
    vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vShaderCode, nullptr);
    glCompileShader(vertex);
    checkCompileErrors(vertex, "VERTEX");
    // fragment Shader
    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fShaderCode, nullptr);
    glCompileShader(fragment);
    checkCompileErrors(fragment, "FRAGMENT");
    // if geometry shader is given, compile geometry shader
    unsigned int geometry;
    const char * gShaderCode = geometryCode.c_str();
    geometry = glCreateShader(GL_GEOMETRY_SHADER);
    glShaderSource(geometry, 1, &gShaderCode, nullptr);
    glCompileShader(geometry);
    checkCompileErrors(geometry, "GEOMETRY");

    // shader Program
    ShaderBase->ID = glCreateProgram();
    glAttachShader(ShaderBase->ID, vertex);
    glAttachShader(ShaderBase->ID, fragment);
    if(pGeometryPath != nullptr)
        glAttachShader(ID, geometry);
    glLinkProgram(ShaderBase->ID);
    checkCompileErrors(ShaderBase->ID, "PROGRAM");
    // delete the shaders as they're linked into our program now and no longer necessery
    glDeleteShader(vertex);
    glDeleteShader(fragment);
    glDeleteShader(geometry);

    printf("Shaders Name <%s> sucessfully loaded with id <%u>\n Paths <%s> <%s> <%s>  .\n",
           ShaderBase->Name,
           ShaderBase->ID,
           pVertexPath,
           pFragmentPath,
           pGeometryPath);

}// ZShader CTOR 2
*/
int ZShader::setupTexSampler(const ZTexture* pTexture)
{
    return setInt(__SHD_TEXTURESAMPLER__,pTexture->getTextureEngineNumber());
}

int ZShader::setupMaterial(const ZMaterial* pMaterial)
{
    int wRet=setVec3("material.Ambient", pMaterial->Ambient);
    if (wRet<0)
            return wRet;
    if ((wRet=setVec3("material.Diffuse", pMaterial->Diffuse))<0)
                return wRet;
    if ((wRet=setVec3("material.Specular", pMaterial->Diffuse))<0)
                return wRet;
    return setVec3("material.Shininess", pMaterial->Diffuse);
}
int ZShader::setupLight(const ZLight* pLight)
{
   int wRet=setVec3("light.Ambient", pLight->Ambient);
   if (wRet<0)
           return wRet;
   if ((wRet=setVec3("light.Diffuse", pLight->Diffuse))<0)
           return wRet;
   return setVec3("light.Specular",pLight->Specular);
}

