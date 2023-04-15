#include "mesh.h"

void Mesh::create(GLfloat *vertices, GLsizeiptr size) {
  vertices_ = vertices;
  size_ = size;
  vao_.create(vertices, size);
  vao_.bind();
  vao_.link(0, 3, GL_FLOAT, 6 * sizeof(float), (void *)0);
  vao_.link(1, 3, GL_FLOAT, 6 * sizeof(float), (void *)(3 * sizeof(float)));
  vao_.unbind();
}

void Mesh::draw() {
  vao_.bind();
  glDrawArrays(GL_TRIANGLES, 0, size_);
}
