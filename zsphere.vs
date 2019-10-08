// GLSL version
#version 330
// uniforms
uniform mat4 mModel;
uniform mat4 mView;
uniform mat4 mNormal;
uniform mat4 mProjection;
// vertex attribs (input)
attribute vec3 aPosition;
attribute vec3 aNormal;
attribute vec2 aTexCoord;
// varyings (output)
varying vec3 esVertex, esNormal;
varying vec2 texCoord0;
void main()
{
    esVertex = vec3(mModel * vec4(aPosition, 1.0));
    esNormal = vec3(mNormal * vec4(aNormal, 1.0));
    texCoord0 = aTexCoord;
//    gl_Position = mProjection * mView * vec4(aPosition, 1.0);
    gl_Position = mProjection * mView * vec4(esVertex, 1.0);
}


