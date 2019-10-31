#version 330 core
//layout (location = 0) in vec4 vertex; // <vec2 pos, vec2 tex>
attribute vec4 TextCoords;// <vec2 pos, vec2 tex>

//out vec2 TextCoords;
out vec2 TextureCoords;

uniform mat4 mModel;
uniform mat4 mView;
uniform mat4 mProjection;
uniform float TextPosZ;

void main()
{
//    gl_Position = mProjection * vec4(TextCoords.xy, 0.0, 1.0);
//    vec4 wPosition = mProjection * vec4(TextCoords.xy, 0.0, 1.0);
    vec4 wPosition = vec4(TextCoords.xy, TextPosZ, 1.0);
//    gl_Position = mProjection * mModel * wPosition;
    gl_Position =  mProjection * mView * mModel *  wPosition;

    TextureCoords = TextCoords.zw;

} 
