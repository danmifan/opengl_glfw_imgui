#include "entity.h"

#include <glm/ext.hpp>
#include <glm/gtc/matrix_access.hpp>
#include <glm/gtx/matrix_decompose.hpp>
#include <glm/gtx/euler_angles.hpp>

#include <algorithm>

#include "id.h"

Entity::Entity() {
  id_ = global_id;
  global_id++;
}

void Entity::create(Mesh* mesh, std::string name) {
  mesh_ = mesh;
  name_ = name;
}

void Entity::create(Mesh* mesh, Material* material, std::string name) {
  mesh_ = mesh;
  material_ = material;
  name_ = name;
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
  local_transform_ = transform;
  world_transform_ = parent_transform_ * local_transform_;
  // glm::quat rotation;
  glm::vec3 skew;
  glm::vec4 perspective;
  glm::decompose(local_transform_, scale_, rotation_, position_, skew,
                 perspective);

  for (const auto& child : children_) {
    child->setParentTransform(world_transform_);
    child->updateMatrix();
  }
}

void Entity::setParentTransform(glm::mat4 parent_transform) {
  parent_transform_ = parent_transform;
}

glm::mat4 Entity::getTransform() { return local_transform_; }

glm::mat4 Entity::getWorldTransform() { return world_transform_; }

glm::mat4 Entity::getParentTransform() { return parent_transform_; }

// void Entity::setMesh(Mesh* mesh) { mesh_ = mesh; }

void Entity::updateMatrix() {
  glm::mat4 mat = glm::mat4(1.0f);
  glm::mat4 t = glm::translate(mat, position_);
  glm::mat4 r = glm::mat4_cast(rotation_);
  glm::mat4 s = glm::scale(mat, scale_);
  local_transform_ = t * r * s;
  world_transform_ = parent_transform_ * local_transform_;

  for (const auto& child : children_) {
    child->setParentTransform(world_transform_);
    child->updateMatrix();
  }
}

void Entity::draw() {
  if (mesh_ && material_) {
    material_->setUniform("view_projection", glm::value_ptr(view_proj_));
    material_->setUniform("model", glm::value_ptr(world_transform_));
    material_->draw();
    mesh_->draw();
  }
}

void Entity::addChild(Entity* entity) {
  auto it = std::find_if(children_.begin(), children_.end(),
                         [&](Entity* e) { return entity->getID() == e->id_; });

  if (it == children_.end()) {
    children_.push_back(entity);
  } else {
    std::cout << "Error : entity " << entity->getID()
              << " is already a children of entity : " << id_;
  }
}

void Entity::removeChild() {
  // auto it = std::find_if(children_.begin(), children_.end(),
  //                        [&](Entity* e) { return entity->getID() == e->id_;
  //                        });
}

std::vector<Entity*> Entity::getChildren() { return children_; }

unsigned int Entity::getID() { return id_; }

void Entity::setName(std::string name) { name_ = name; }

std::string Entity::getName() { return name_; }

void Entity::setViewProj(glm::mat4 view_proj) { view_proj_ = view_proj; }