#version 330 core

in vec3 o_color;
in vec3 o_pos;

void main() {
  float value = length(o_pos) / 2.5;
  gl_FragColor = vec4(value, value, value, 1.0);
}