#version 330 core

layout(location = 0) in vec3 pos;
layout(location = 1) in vec3 color;

out vec3 o_color;
out vec3 o_pos;

uniform mat4 mvp;

void main() {
  o_color = color;
  o_pos = pos;
  gl_Position = mvp * vec4(pos, 1.0);
  gl_PointSize = 2.0;
}