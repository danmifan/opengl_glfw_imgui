#ifndef SCENE_H
#define SCENE_H

#include "entity.h"

class Scene {
 public:
  void addEntity(Entity* entity);
  std::vector<Entity*> getEntities();

 private:
  std::vector<Entity*> roots_;
};

#endif  // SCENE_H