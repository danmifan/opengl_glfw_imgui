#ifndef VAO_H
#define VAO_H

#include <iostream>
#include "vbo.h"

class VAO {
 public:
  VAO();
  void link(VBO& vbo, GLuint layout, GLuint num_components, GLenum type,
            GLsizeiptr stride, void* offset);
  void bind();
  void unbind();
  void clean();

 private:
  GLuint id_ = 0;
};

#endif  // VAO_H