// GLSL version
#version 330 core
// uniforms
uniform mat4 matrixModelView;
uniform mat4 matrixNormal;
uniform mat4 matrixModelViewProjection;
// vertex attribs (input)
attribute vec3 vertexPosition;
attribute vec3 vertexNormal;
attribute vec2 vertexTexCoord;
// varyings (output)
varying vec3 esVertex, esNormal;
varying vec2 texCoord0;
void main()
{
    esVertex = vec3(matrixModelView * vec4(vertexPosition, 1.0));
    esNormal = vec3(matrixNormal * vec4(vertexNormal, 1.0));
    texCoord0 = vertexTexCoord;
    gl_Position = matrixModelViewProjection * vec4(vertexPosition, 1.0);
}
