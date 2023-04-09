#version 330 core

layout(location = 0) in vec3 pos;

uniform float size;

uniform mat4 cam_matrix;

void main() {
  // gl_Position = vec4(size * pos.x, size * pos.y, size * pos.z, 1.0);
  gl_Position = cam_matrix * vec4(pos, 1.0);
}