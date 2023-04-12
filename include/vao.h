#ifndef VAO_H
#define VAO_H

#include <iostream>
#include <GL/glew.h>
#include <GL/gl.h>

class VAO {
 public:
  void create(GLfloat* vertices, GLsizeiptr size);
  void link(GLuint layout, GLuint num_components, GLenum type,
            GLsizeiptr stride, void* offset);
  void bind();
  void unbind();
  void clean();

 private:
  GLuint vao_id_;
  GLuint vbo_id_;
};

#endif  // VAO_H