#include "camera.h"

Camera::Camera(int width, int height, glm::vec3 position) {
  width_ = width;
  height_ = height;
  position_ = position;
}

glm::mat4 Camera::matrix(float fov, float znear, float zfar) {
  glm::mat4 model = glm::mat4(1.0f);
  glm::mat4 view = glm::mat4(1.0f);
  glm::mat4 proj = glm::mat4(1.0f);

  // static float rotation = 0.0f;
  // rotation++;

  // model =
  //     glm::rotate(model, glm::radians(rotation), glm::vec3(0.0f, 1.0f,
  //     0.0f));
  // view = glm::translate(view, glm::vec3(0.0f, -0.5f, -2.0f));
  // proj =
  //     glm::perspective(glm::radians(fov), (float)width_ / height_, znear,
  //     zfar);

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

glm::vec3 Camera::getPosition() { return position_; }
