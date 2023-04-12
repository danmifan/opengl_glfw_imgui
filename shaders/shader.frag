#version 330 core

in vec3 ourColor;
out vec4 FragColor;

// uniform vec4 color;

void main() {
  //  frag_color = color;
  FragColor = vec4(ourColor, 1.0);
}