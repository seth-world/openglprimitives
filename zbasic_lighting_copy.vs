#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;

out vec3 FragPos;
out vec3 Normal;

uniform mat4 mModel;
uniform mat4 mView;
uniform mat4 mProjection;
uniform mat4 mNormal



void main()
{
    FragPos = vec3(mModel * vec4(aPos, 1.0));
    Normal = mNormal * aNormal;
 //   Normal = mat3(transpose(inverse(model))) * aNormal;  // mNormal already computed : not within shader program
    gl_Position = projection * view * vec4(FragPos, 1.0);
}
