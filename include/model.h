#ifndef MODEL_H
#define MODEL_H

#include <iostream>
#include <map>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "entity.h"
#include "material.h"
#include "assets.h"

class Model {
 public:
  Entity* load(std::string path, Shader* shader);
  std::vector<Mesh> getMeshes();

 private:
  void processNode(aiNode* node, const aiScene* scene, Entity* entity);
  Mesh* processMesh(aiMesh* mesh);
  void processMaterials(const aiScene* scene);
  Texture* loadMaterialTexture(aiMaterial* material, aiTextureType type,
                               TextureType my_type);

  std::map<int, Material*> materials_;
  std::string path_;

#warning tmp
  Shader* shader_;
};

#endif  // MODEL_H