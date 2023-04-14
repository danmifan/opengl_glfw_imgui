#version 330 core

layout(location = 0) in vec3 pos;
layout(location = 1) in vec3 color;

out vec3 ourColor;

uniform float size;
uniform mat4 model;
uniform mat4 view_projection;

void main() {
  // gl_Position = vec4(size * pos.x, size * pos.y, size * pos.z, 1.0);
  gl_Position = view_projection * model * vec4(pos, 1.0);
  // gl_Position = vec4(pos, 1.0);
  ourColor = color;
}