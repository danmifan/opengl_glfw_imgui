#include "scene.h"

void Scene::addEntity(Entity *entity) { roots_.push_back(entity); }

std::vector<Entity *> Scene::getEntities() { return roots_; }