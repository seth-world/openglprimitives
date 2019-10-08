#version 330 core
out vec4 FragColor;
//uniform vec3 InColor;
//uniform float InAlpha;
//in vec2 TexCoord;

// texture samplers
//uniform sampler2D texture1;
//uniform sampler2D texture2;

void main()
{
	// linearly interpolate between both textures (80% container, 20% awesomeface)
//	FragColor = mix(texture(texture1, TexCoord), texture(texture2, TexCoord), 0.2);
	FragColor = vec4(0.5f, 0.8f, 1.0f, 0.5f);
//    FragColor = vec4(InColor, InAlpha);
}
