#ifndef ASSETS_H
#define ASSETS_H

#include "mesh.h"
#include "material.h"

class Assets {
 public:
  static Assets& get();

  std::vector<Mesh*> meshes;
  std::vector<Material*> materials;

 private:
};

#endif  // ASSETS_H