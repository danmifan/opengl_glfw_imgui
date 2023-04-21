#include "camera.h"

#include <iostream>
#include <glm/gtc/matrix_access.hpp>
#include <glm/gtx/string_cast.hpp>

void Camera::create(int width, int height, glm::vec3 position, float fov,
                    float znear, float zfar) {
  width_ = width;
  height_ = height;
  position_ = position;
  fov_ = fov;
  znear_ = znear;
  zfar_ = zfar;
}

glm::mat4 Camera::getViewProjMatrix() {
  glm::mat4 view = glm::mat4(1.0f);
  glm::mat4 proj = glm::mat4(1.0f);

  view = glm::lookAt(position_, position_ + orientation_, up_);

  proj = glm::perspective(glm::radians(fov_), (float)width_ / height_, znear_,
                          zfar_);

  return proj * view;
}

glm::mat4 Camera::getViewMatrix() {
  return glm::lookAt(position_, position_ + orientation_, up_);
}

glm::mat4 Camera::getProjMatrix() {
  return glm::perspective(glm::radians(fov_), (float)width_ / height_, znear_,
                          zfar_);
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

void Camera::setPosition(glm::vec3 position) { position_ = position; }

glm::vec3 Camera::getPosition() { return position_; }

// void Camera::setPosition(glm::vec3 position) {
//   transform_ = glm::translate(transform_, position);
//   position_ = position;
// }

void Camera::rotate(float x, float y, float z) {
  glm::mat4 rot = glm::mat4(1.0f);
  rot = glm::rotate(rot, glm::radians(y), glm::vec3(0.0f, 1.0f, 0.0f));
  rot = glm::rotate(rot, glm::radians(x),
                    glm::normalize(glm::cross(orientation_, up_)));
  orientation_ = rot * glm::vec4(orientation_, 0.0f);
}
