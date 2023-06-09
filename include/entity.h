#ifndef ENTITY_H
#define ENTITY_H

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

#include "mesh.h"
#include "material.h"

class Entity {
 public:
  Entity();
  void create(Mesh* mesh, std::string name = "default");
  void create(Mesh* mesh, Material* material, std::string name = "default");

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
  glm::mat4 getWorldTransform();
  glm::mat4 getParentTransform();

  // void setMesh(Mesh* mesh);
  void draw();

  void setName(std::string name);
  std::string getName();

  void addChild(Entity* entity);
  void removeChild();
  std::vector<Entity*> getChildren();

  unsigned int getID();

  Material* getMaterial();

 private:
  glm::mat4 parent_transform_ = glm::mat4(1.0f);
  glm::mat4 local_transform_ = glm::mat4(1.0f);
  glm::mat4 world_transform_ = glm::mat4(1.0f);
  glm::vec3 position_ = glm::vec3(0.0f, 0.0f, 0.0f);
  glm::vec3 euler_ = glm::vec3(0.0f, 0.0f, 0.0f);
  glm::vec3 scale_ = glm::vec3(1.0f, 1.0f, 1.0f);
  glm::quat rotation_ = glm::quat(glm::vec3(0, 0, 0));

  Mesh* mesh_ = nullptr;
  Material* material_ = nullptr;
  std::vector<Entity*> children_;
  unsigned int id_;
  std::string name_;

  void updateMatrix();
};

#endif  // ENTITY_H