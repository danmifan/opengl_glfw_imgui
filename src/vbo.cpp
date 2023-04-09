#include "vbo.h"

VBO::VBO() {}

VBO::VBO(GLfloat* vertices, GLsizeiptr size) {
  glGenBuffers(1, &id_);
  glBindBuffer(GL_ARRAY_BUFFER, id_);
  glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);
}

void VBO::bind() { glBindBuffer(GL_ARRAY_BUFFER, id_); }

void VBO::unbind() { glBindBuffer(GL_ARRAY_BUFFER, 0); }

void VBO::clean() { glDeleteBuffers(1, &id_); }