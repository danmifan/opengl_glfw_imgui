#ifndef VBO_H
#define VBO_H

#include <GL/glew.h>
#include <GL/gl.h>

class VBO {
 public:
  VBO();
  VBO(GLfloat* vertices, GLsizeiptr size);
  void bind();
  void unbind();
  void clean();

 private:
  GLuint id_;
};

#endif  // VBO_H