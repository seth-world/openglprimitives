#version 330 core
varying vec2 texpos;
uniform sampler2D TextureSampler;
uniform vec3 TextColor;

void main(void) {
//    vec4 wColor=vec4(TextColor,1.0);
//  gl_FragColor = vec4(1, 1, 1, texture2D(TexSampler, texpos).a) * wColor;

  gl_FragColor = vec4(TextColor, texture(TextureSampler, texpos).r);
}
