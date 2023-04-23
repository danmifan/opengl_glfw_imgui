#ifndef POINT_CLOUD_H
#define POINT_CLOUD_H

#include <iostream>
#include <vector>

#include <GL/glew.h>
#include <GL/gl.h>
#include <glm/ext.hpp>

#include "shader.h"

struct Point {
  double x;
  double y;
  double z;
  int r;
  int g;
  int b;
};

struct VertexPoint {
  glm::vec3 position;
  glm::vec3 color;
};

class PointCloud {
 public:
  void load(std::string filepath);
  void init(Shader* shader);
  void draw(glm::mat4 view_proj);

 private:
  std::vector<Point> points_;
  std::vector<VertexPoint> vertices_;
  unsigned int vao_;
  unsigned int vbo_;
  Shader* shader_;
};

#endif  // POINT_CLOUD_H