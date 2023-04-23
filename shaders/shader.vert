#version 330 core

layout(location = 0) in vec3 pos;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 tex_coords;
layout(location = 3) in vec3 color;

out vec3 o_color;
out vec2 o_tex_coords;

// uniform mat4 model;
// uniform mat4 view_projection;
uniform mat4 mvp;

void main() {
  o_color = color;
  o_tex_coords = tex_coords;
  // gl_Position = view_projection * model * vec4(pos, 1.0);
  gl_Position = mvp * vec4(pos, 1.0);
}