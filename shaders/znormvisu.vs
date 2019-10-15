#version 330 core
layout (location = 0) in vec3   aPosition;

out vec3 FragPos;

uniform mat4 mModel;
uniform mat4 mView;
uniform mat4 mProjection;
uniform mat4 mNormal;



void main()
{
 //   Normal = mat3(transpose(inverse(model))) * aNormal;  // mNormal already computed : not within shader program
    gl_Position = mProjection * mView * mModel * vec4(aPosition, 1.0);
}
