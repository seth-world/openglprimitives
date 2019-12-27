#version 330 core
//layout (location = 0) in vec4 vertex; // <vec2 pos, vec2 tex>
attribute vec2 aPosition;
attribute vec2 aTexCoords;
attribute vec2 aNormal;

out vec2 TextureCoords;
out vec3 NormalCoords;

uniform float BoxPosZ;
uniform mat4 mModel;
uniform mat4 mView;
uniform mat4 mProjection;
uniform mat4 mNormal;


void main()
{
    NormalCoords   = vec3(mNormal * vec4(aNormal, 1.0));
    gl_Position =  mProjection * mView * mModel *  vec4(aPosition,BoxPosZ, 1.0);

    TextureCoords = aTexCoords;
}
