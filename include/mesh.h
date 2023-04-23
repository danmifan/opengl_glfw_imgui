#ifndef MESH_H
#define MESH_H

#include <iostream>
#include <vector>
#include <GL/glew.h>
#include <GL/gl.h>

#include "vertex.h"
#include "texture.h"
#include "shader.h"

class Mesh {
 public:
  void create(std::vector<Vertex> vertices, std::vector<GLuint> indices,
              std::string name = "");
  void create(std::vector<Vertex> vertices, std::string name = "");
  void draw();
  std::string getName();

 private:
  std::vector<Vertex> vertices_;
  std::vector<GLuint> indices_;
  std::string name_ = "";
  GLuint vao_;
  GLuint vbo_;
  GLuint ebo_;
};

#endif  // MESH_H