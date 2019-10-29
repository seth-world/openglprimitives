#version 330 core
//layout (location = 0) in vec4 vertex; // <vec2 pos, vec2 tex>
attribute vec4 TextCoords;// <vec2 pos, vec2 tex>

//out vec2 TextCoords;
out vec2 TextureCoords;

uniform mat4 mProjection;

void main()
{
    gl_Position = mProjection * vec4(TextCoords.xy, 0.0, 1.0);
    TextureCoords = TextCoords.zw;

} 
