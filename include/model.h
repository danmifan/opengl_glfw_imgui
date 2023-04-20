#ifndef MODEL_H
#define MODEL_H

#include <iostream>
#include "entity.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

class Model {
 public:
  void load(std::string path);
  std::vector<Mesh> getMeshes();
  Entity* getEntity();

 private:
  void processNode(aiNode* node, const aiScene* scene, Entity* entity);
  Mesh* processMesh(aiMesh* mesh, const aiScene* scene);
  std::vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type,
                                            std::string type_name);

  std::vector<Mesh> meshes_;
  std::vector<Texture> textures_;
  std::string path_;
  Entity* root_entity_;
};

#endif  // MODEL_H