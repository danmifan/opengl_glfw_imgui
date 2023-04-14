#ifndef CAMERA_H
#define CAMERA_H

#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <GL/gl.h>

class Camera {
 public:
  void create(int width, int height, glm::vec3 position);
  glm::mat4 matrix(float fov, float near, float far);

  void forward();
  void backward();
  void left();
  void right();
  void up();
  void down();
  // void rotateY(float angle);
  // void rotateX(float angle);
  void rotate(float x, float y, float z);
  void move(glm::vec3 mvt);

  glm::vec3 getPosition();

 private:
  int width_;
  int height_;
  glm::vec3 position_;
  glm::vec3 orientation_ = glm::vec3(0.0f, 0.0f, -1.0f);
  glm::vec3 start_orientation_ = glm::vec3(0.0f, 0.0f, -1.0f);
  glm::vec3 up_ = glm::vec3(0.0f, 1.0f, 0.0f);
  float speed_ = 0.1f;
  float sensitivity = 100.0f;

  glm::mat4 transform_ = glm::mat4(1.0f);
};

#endif  // CAMERA_H