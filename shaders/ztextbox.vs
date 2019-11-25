#version 330 core
//layout (location = 0) in vec3 aPos;

attribute vec3 aPosition;

uniform mat4 mModel;
uniform mat4 mView;
uniform mat4 mProjection;

void main()
{
    gl_Position = mProjection * mView * mModel * vec4(aPosition, 1.0);
}
