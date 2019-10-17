#include <zshader.h>

ZShader::ZShader(const char* pVertexPath, const char* pFragmentPath, const char* pName )
{
    Name=pName;
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

    fprintf(stdout,"\n Vertex path <%s> \nFragment path <%s>\n",wVertexPath.c_str(),wFragmentPath.c_str());
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
    ID = glCreateProgram();
    glAttachShader(ID, vertex);
    glAttachShader(ID, fragment);
    glLinkProgram(ID);
    checkCompileErrors(ID, "PROGRAM");
    // delete the shaders as they're linked into our program now and no longer necessery
    glDeleteShader(vertex);
    glDeleteShader(fragment);

    printf("Shaders Name <%s> sucessfully loaded with id <%u>.\n",
           Name,
           ID);
}// ZShader CTOR

ZShader::ZShader(const char* pVertexPath, const char* pFragmentPath, const char* pGeometryPath ,const char* pName)
{
    Name=pName;
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

    fprintf(stdout," Vertex path <%s> \nFragment path <%s>\n",wVertexPath.c_str(),wFragmentPath.c_str());
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
    ID = glCreateProgram();
    glAttachShader(ID, vertex);
    glAttachShader(ID, fragment);
    if(pGeometryPath != nullptr)
        glAttachShader(ID, geometry);
    glLinkProgram(ID);
    checkCompileErrors(ID, "PROGRAM");
    // delete the shaders as they're linked into our program now and no longer necessery
    glDeleteShader(vertex);
    glDeleteShader(fragment);
    glDeleteShader(geometry);

    printf("Shaders Name <%s> sucessfully loaded with id <%u>\n Paths <%s> <%s> <%s>  .\n",
           Name,
           ID,
           pVertexPath,
           pFragmentPath,
           pGeometryPath);

}// ZShader CTOR 2


void ZShader::setMaterial(const ZMaterial& pMaterial)
{
    setVec3("material.Ambient", pMaterial.Ambient);
    setVec3("material.Diffuse", pMaterial.Diffuse);
    setVec3("material.Specular",pMaterial.Specular);
    setFloat("material.Shininess",pMaterial.Shininess);

}
void ZShader::setLight(const ZLight& pLight)
{
   setVec3("light.Ambient", pLight.Ambient);
   setVec3("light.Diffuse", pLight.Diffuse);
   setVec3("light.Specular",pLight.Specular);
}
