#include "camera.h"

void Camera::create(int width, int height, glm::vec3 position) {
  width_ = width;
  height_ = height;
  position_ = position;
}

glm::mat4 Camera::matrix(float fov, float znear, float zfar) {
  glm::mat4 model = glm::mat4(1.0f);
  glm::mat4 view = glm::mat4(1.0f);
  glm::mat4 proj = glm::mat4(1.0f);

  // model =
  //     glm::rotate(model, glm::radians(rotation), glm::vec3(0.0f, 1.0f,
  //     0.0f));

  view = glm::lookAt(position_, position_ + orientation_, up_);

  proj =
      glm::perspective(glm::radians(fov), (float)width_ / height_, znear, zfar);

  // return proj * view * model;

  return proj * view;
}

void Camera::forward() { position_ += speed_ * orientation_; }

void Camera::backward() { position_ += speed_ * -orientation_; }

void Camera::left() {
  position_ += speed_ * -glm::normalize(glm::cross(orientation_, up_));
}

void Camera::right() {
  position_ += speed_ * glm::normalize(glm::cross(orientation_, up_));
}

void Camera::up() { position_ += speed_ * up_; }

void Camera::down() { position_ += speed_ * -up_; }

void Camera::move(glm::vec3 mvt) {}

glm::vec3 Camera::getPosition() { return position_; }

// void Camera::rotateY(float angle) {
//   glm::mat4 rotation = glm::mat4(1.0f);
//   rotation =
//       glm::rotate(rotation, glm::radians(angle), glm::vec3(0.0f, 1.0f,
//       0.0f));
//   glm::vec4 orientation = glm::vec4(start_orientation_, 0.0f);
//   orientation_ = rotation * orientation;
// }

// void Camera::rotateX(float angle) {
//   glm::mat4 rotation = glm::mat4(1.0f);
//   rotation = glm::rotate(rotation, angle, glm::vec3(1.0f, 0.0f, 0.0f));
//   glm::vec4 orientation = glm::vec4(orientation_, 1.0f);
//   orientation_ = rotation * orientation;
// }

void Camera::rotate(float x, float y, float z) {
  glm::mat4 rot = glm::mat4(1.0f);
  rot = glm::rotate(rot, glm::radians(y), glm::vec3(0.0f, 1.0f, 0.0f));
  rot = glm::rotate(rot, glm::radians(x),
                    glm::normalize(glm::cross(orientation_, up_)));
  orientation_ = rot * glm::vec4(orientation_, 0.0f);
}
