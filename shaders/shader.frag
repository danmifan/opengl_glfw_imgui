#version 330 core

in vec2 o_tex_coords;
in vec3 o_color;

// out vec4 FragColor;

uniform sampler2D texture_diffuse;

void main() {
  // FragColor = vec4(ourColor, 1.0);
  gl_FragColor = texture2D(texture_diffuse, o_tex_coords) * vec4(o_color, 1.0);
}