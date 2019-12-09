#version 330 core
//layout (location = 0) in vec4 vertex; // <vec2 pos, vec2 tex>
attribute vec2 aPosition;
attribute vec2 aTextureCoords;

out vec2 TextureCoords;

uniform float BoxPosZ;
uniform mat4 mModel;
uniform mat4 mView;
uniform mat4 mProjection;


void main()
{
    gl_Position =  mProjection * mView * mModel *  vec4(aPosition,BoxPosZ, 1.0);

    TextureCoords = aTextureCoords;
}
