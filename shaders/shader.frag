#version 330 core

out vec4 FragColor;
in vec2 o_tex_coords;

uniform sampler2D texture_diffuse1;

void main() {
  // FragColor = vec4(ourColor, 1.0);
  FragColor = texture(texture_diffuse1, o_tex_coords);
}