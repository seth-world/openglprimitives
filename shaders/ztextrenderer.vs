#version 330 core
attribute vec4 TextCoords;
varying vec2 texpos;

void main(void) {
  gl_Position = vec4(TextCoords.xy, 0.0, 1.0);
  texpos = TextCoords.zw;
}
