#version 330 core
//layout (location = 0) in vec3 aPosition;
//layout (location = 1) in vec3 aNormal;
//layout (location = 2) in vec2 aTexCoords;

attribute vec3 aPosition;
attribute vec3 aNormal;
attribute vec2 aTexCoords;

uniform mat4 mProjection;
uniform mat4 mModel;
uniform mat4 mView;
uniform mat4 mNormal;

// declare an interface block; see 'Advanced GLSL' for what these are.
out Vertex_iblock {
    vec3 Position;
    vec3 Normal;
    vec2 TexCoords;
} vs_out;

void main()
{
    vec3 wPosition = vec3(mModel * vec4(aPosition, 1.0));
//    vs_out.Position = wPosition;
    vs_out.Normal   = vec3(mNormal * vec4(aNormal, 1.0));
    vs_out.TexCoords = aTexCoords;
    vec4 wMPosition=mProjection * mView * vec4(wPosition , 1.0);
    vs_out.Position = aPosition;
    gl_Position = mProjection * mView * vec4(wPosition , 1.0);
}
