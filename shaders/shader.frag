#version 330 core

out vec4 FragColor;
in vec2 o_tex_coords;

uniform sampler2D texture_diffuse;

void main() {
  // FragColor = vec4(ourColor, 1.0);
  FragColor = texture2D(texture_diffuse, o_tex_coords);
}