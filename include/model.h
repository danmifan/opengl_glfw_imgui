#ifndef MODEL_H
#define MODEL_H

#include <iostream>
#include "mesh.h"
#include "texture.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

class Model {
 public:
  void load(std::string path);
  std::vector<Mesh> getMeshes();

 private:
  void processNode(aiNode* node, const aiScene* scene);
  Mesh processMesh(aiMesh* mesh, const aiScene* scene);
  std::vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type,
                                            std::string type_name);

  std::vector<Mesh> meshes_;
  std::vector<Texture> textures_;
  std::string path_;
};

#endif  // MODEL_H