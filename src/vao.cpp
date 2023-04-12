#include "vao.h"

void VAO::create(GLfloat* vertices, GLsizeiptr size) {
  // VBO
  glGenBuffers(1, &vbo_id_);
  glBindBuffer(GL_ARRAY_BUFFER, vbo_id_);
  glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);

  // VAO
  glGenVertexArrays(1, &vao_id_);
}

void VAO::link(GLuint layout, GLuint num_components, GLenum type,
               GLsizeiptr stride, void* offset) {
  // vbo.bind();
  glBindBuffer(GL_ARRAY_BUFFER, vbo_id_);
  // Position
  glVertexAttribPointer(0, 3, type, GL_FALSE, 6 * stride, (void*)0);
  glEnableVertexAttribArray(0);
  // Color
  glVertexAttribPointer(1, 3, type, GL_FALSE, 6 * stride,
                        (void*)(3 * sizeof(float)));
  glEnableVertexAttribArray(1);

  glBindBuffer(GL_ARRAY_BUFFER, 0);
  // vbo.unbind();
}

void VAO::bind() { glBindVertexArray(vao_id_); }

void VAO::unbind() { glBindVertexArray(0); }

void VAO::clean() {
  glDeleteBuffers(1, &vbo_id_);
  glDeleteVertexArrays(1, &vao_id_);
}