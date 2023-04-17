#ifndef ENTITY_H
#define ENTITY_H

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

#include "shader.h"
#include "mesh.h"

class Entity {
 public:
  void create(unsigned int id, Mesh* mesh, Shader* shader);

  void setPosition(glm::vec3 position);
  glm::vec3 getPosition();
  void setRotation(glm::vec3 angles);
  void getRotation();
  glm::vec3 getAngles();
  void setScale(glm::vec3 scale);
  glm::vec3 getScale();
  void setTransform(glm::mat4 transform);
  void setParentTransform(glm::mat4 parent_transform);
  glm::mat4 getTransform();
  void draw();

  void addChild(Entity* entity);
  void removeChild();

  unsigned int getID();

 private:
  glm::mat4 parent_transform_ = glm::mat4(1.0f);
  glm::mat4 transform_ = glm::mat4(1.0f);
  glm::vec3 position_ = glm::vec3(0.0f, 0.0f, 0.0f);
  glm::vec3 euler_ = glm::vec3(0.0f, 0.0f, 0.0f);
  glm::vec3 scale_ = glm::vec3(1.0f, 1.0f, 1.0f);
  glm::quat rotation_;

  Mesh* mesh_;
  Shader* shader_;
  std::vector<Entity*> children_;
  unsigned int id_;

  void updateMatrix();
};

#endif  // ENTITY_H