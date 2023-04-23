#include "point_cloud.h"

#include <fstream>

void PointCloud::load(std::string filepath) {
  std::ifstream input(filepath);

  if (!input.is_open()) {
    return;
  }

  double x, y, z;
  int r, g, b;
  while (input >> x >> y >> z >> r >> g >> b) {
    Point point;
    point.x = x;
    point.y = y;
    point.z = z;
    point.r = r;
    point.g = g;
    point.b = b;

    VertexPoint vp;
    vp.position = {-x, z, y};
    vp.color = {r / 255.0f, g / 255.0f, b / 255.0f};

    // points_.push_back(point);
    vertices_.push_back(vp);
  }

  std::cout << "Point cloud loaded" << std::endl;
  std::cout << "Point cloud size : " << points_.size() << std::endl;
}

void PointCloud::init(Shader *shader) {
  glGenVertexArrays(1, &vao_);
  glGenBuffers(1, &vbo_);

  glBindVertexArray(vao_);

  glBindBuffer(GL_ARRAY_BUFFER, vbo_);
  glBufferData(GL_ARRAY_BUFFER, vertices_.size() * sizeof(VertexPoint),
               vertices_.data(), GL_STATIC_DRAW);

  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VertexPoint),
                        (void *)0);

  glEnableVertexAttribArray(1);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(VertexPoint),
                        (void *)offsetof(VertexPoint, color));

  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);

  shader_ = shader;
}

void PointCloud::draw(glm::mat4 view_proj) {
  shader_->activate();
  glm::mat4 mvp = view_proj * glm::mat4(1.0f);
  shader_->setUniform("mvp", glm::value_ptr(mvp));
  glBindVertexArray(vao_);
  glDrawArrays(GL_POINTS, 0, vertices_.size());
  glBindVertexArray(0);
}