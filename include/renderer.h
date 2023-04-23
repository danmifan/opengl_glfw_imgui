#ifndef RENDERER_H
#define RENDERER_H

#include "scene.h"
#include "camera.h"

class Renderer {
 public:
  void create(Scene* scene, Camera* camera);
  void draw();

 private:
  Scene* scene_ = nullptr;
  Camera* camera_ = nullptr;

  void draw_(Entity* entity);
  glm::mat4 mvp_;
};

#endif  // RENDERER_H