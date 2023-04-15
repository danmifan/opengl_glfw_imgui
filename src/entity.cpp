#include "entity.h"

#include <glm/ext.hpp>
#include <glm/gtc/matrix_access.hpp>
#include <glm/gtx/matrix_decompose.hpp>
#include <glm/gtx/euler_angles.hpp>

void Entity::create(Mesh* mesh, Shader* shader) {
  mesh_ = mesh;
  shader_ = shader;
}

void Entity::setPosition(glm::vec3 position) {
  position_ = position;
  updateMatrix();
}

glm::vec3 Entity::getPosition() { return position_; }

void Entity::setRotation(glm::vec3 angles) {
  glm::mat4 rot = glm::mat4(1.0f);
  glm::mat4 y =
      glm::rotate(rot, glm::radians(angles.y), glm::vec3(0.0f, 1.0f, 0.0f));
  glm::mat4 x =
      glm::rotate(rot, glm::radians(angles.x), glm::vec3(1.0f, 0.0f, 0.0f));
  glm::mat4 z =
      glm::rotate(rot, glm::radians(angles.z), glm::vec3(0.0f, 0.0f, 1.0f));
  rot = y * x * z;
  rotation_ = glm::quat_cast(rot);
  updateMatrix();
}

void Entity::getRotation() {}

glm::vec3 Entity::getAngles() {
  glm::vec3 angles;
  glm::extractEulerAngleYXZ(glm::mat4_cast(rotation_), angles.x, angles.y,
                            angles.z);
  angles.x = glm::degrees(angles.x);
  angles.y = glm::degrees(angles.y);
  angles.z = glm::degrees(angles.z);

  return angles;
}

void Entity::setScale(glm::vec3 scale) {
  scale_ = scale;
  updateMatrix();
}

glm::vec3 Entity::getScale() { return scale_; }

void Entity::setTransform(glm::mat4 transform) {
  // A tester
  transform_ = transform;
  glm::quat rotation;
  glm::vec3 skew;
  glm::vec4 perspective;
  glm::decompose(transform_, scale_, rotation, position_, skew, perspective);
}

glm::mat4 Entity::getTransform() { return transform_; }

void Entity::updateMatrix() {
  glm::mat4 mat = glm::mat4(1.0f);
  glm::mat4 t = glm::translate(mat, position_);
  glm::mat4 r = glm::mat4_cast(rotation_);
  glm::mat4 s = glm::scale(mat, scale_);
  transform_ = t * r * s;
}

void Entity::draw() {
  shader_->setUniform("model", glm::value_ptr(transform_));
  mesh_->draw();
}