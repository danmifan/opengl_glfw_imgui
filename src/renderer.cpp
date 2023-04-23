#include "renderer.h"

void Renderer::create(Scene* scene, Camera* camera) {
  scene_ = scene;
  camera_ = camera;
}

void Renderer::draw() {
  std::vector<Entity*> entities = scene_->getEntities();

  for (const auto& entity : entities) {
    draw_(entity);
  }
}

void Renderer::draw_(Entity* entity) {
  glm::mat4 view_proj = camera_->getViewProjMatrix();
  glm::mat4 model = entity->getWorldTransform();
  mvp_ = view_proj * model;
  Material* material = entity->getMaterial();
  if (material) {
    material->setUniform("mvp", glm::value_ptr(mvp_));
    material->draw();
  }

  entity->draw();

  for (const auto& child : entity->getChildren()) {
    draw_(child);
  }
}
