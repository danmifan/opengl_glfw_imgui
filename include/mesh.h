#ifndef MESH_H
#define MESH_H

#include <iostream>
#include <GL/glew.h>
#include <GL/gl.h>
#include "vao.h"

class Mesh {
 public:
  void create(GLfloat* vertices, GLsizeiptr size);
  void draw();
  void bind();

 private:
  VAO vao_;
  GLfloat* vertices_;
  GLsizeiptr size_;
};

#endif  // MESH_H