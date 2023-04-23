#include "model.h"

void Model::load(std::string path) {
  Assimp::Importer importer;
#warning flip uv here
  const aiScene* scene = importer.ReadFile(
      path.c_str(), aiProcess_Triangulate | aiProcess_FlipUVs);

  if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE ||
      !scene->mRootNode) {
    std::cout << "ERROR::ASSIMP::" << importer.GetErrorString() << std::endl;
    return;
  }

  path_ = path;

  std::cout << "Importing model " << path << std::endl;

  root_entity_ = new Entity();
  root_entity_->setName(std::string(scene->mRootNode->mName.C_Str()));

  processNode(scene->mRootNode, scene, root_entity_);

  std::cout << "Model imported" << std::endl;
}

void Model::processNode(aiNode* node, const aiScene* scene, Entity* entity) {
  std::cout << "Nb : " << node->mNumMeshes << " " << node->mNumChildren << " "
            << std::string(node->mName.C_Str()) << std::endl;

  for (unsigned int i = 0; i < node->mNumMeshes; i++) {
    aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
    Mesh* my_mesh = processMesh(mesh, scene);
    Entity* child = new Entity();
    child->create(my_mesh);
    child->setName(std::string(mesh->mName.C_Str()));
    entity->addChild(child);
    // meshes_.push_back(processMesh(mesh, scene));
  }

  for (unsigned int i = 0; i < node->mNumChildren; i++) {
    processNode(node->mChildren[i], scene, entity);
  }
}

Mesh* Model::processMesh(aiMesh* mesh, const aiScene* scene) {
  std::vector<Vertex> vertices;
  std::vector<unsigned int> indices;
  std::vector<Texture> textures;

  for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
    Vertex vertex;
    vertex.position = glm::vec3(mesh->mVertices[i].x, mesh->mVertices[i].y,
                                mesh->mVertices[i].z);

    if (mesh->HasNormals()) {
      vertex.normal = glm::vec3(mesh->mNormals[i].x, mesh->mNormals[i].y,
                                mesh->mNormals[i].z);
    }

    if (mesh->mTextureCoords[0]) {
      vertex.tex_coords =
          glm::vec2(mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y);
    } else {
      vertex.tex_coords = glm::vec2(0.0f, 0.0f);
    }

    vertices.push_back(vertex);
  }

  for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
    aiFace face = mesh->mFaces[i];
    for (unsigned int j = 0; j < face.mNumIndices; j++) {
      indices.push_back(face.mIndices[j]);
    }
  }

  aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

  // 1. diffuse maps
  std::vector<Texture> diffuseMaps =
      loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
  textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
  // 2. specular maps
  std::vector<Texture> specularMaps = loadMaterialTextures(
      material, aiTextureType_SPECULAR, "texture_specular");
  textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
  // 3. normal maps
  std::vector<Texture> normalMaps =
      loadMaterialTextures(material, aiTextureType_HEIGHT, "texture_normal");
  textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());
  // 4. height maps
  std::vector<Texture> heightMaps =
      loadMaterialTextures(material, aiTextureType_AMBIENT, "texture_height");
  textures.insert(textures.end(), heightMaps.begin(), heightMaps.end());

  std::string name = std::string(mesh->mName.C_Str());

  Mesh* my_mesh = new Mesh();
  my_mesh->create(vertices, indices, name);
  return my_mesh;
}

std::vector<Texture> Model::loadMaterialTextures(aiMaterial* mat,
                                                 aiTextureType type,
                                                 std::string type_name) {
  std::vector<Texture> textures;
  for (unsigned int i = 0; i < mat->GetTextureCount(type); i++) {
    aiString ai_str;
    mat->GetTexture(type, i, &ai_str);
    std::string tmp_path = path_.substr(0, path_.find_last_of('/'));
    std::string str = tmp_path + "/" + std::string(ai_str.C_Str());

    // check if texture was loaded before and if so, continue to next iteration:
    // skip loading a new texture
    bool skip = false;
    for (unsigned int j = 0; j < textures_.size(); j++) {
      if (textures_[j].getPath() == str) {
        textures.push_back(textures_[j]);
        skip = true;
        break;
      }
    }
    if (!skip) {  // if texture hasn't been loaded already, load it
      Texture texture;
      if (str.find(".dds") != std::string::npos) {
        texture.loadDDSFile(str, type_name);
      } else {
        texture.loadFromImage(str, type_name);
      }
      textures.push_back(texture);
      textures_.push_back(texture);
    }
  }
  return textures;
}

std::vector<Mesh> Model::getMeshes() { return meshes_; }

Entity* Model::getEntity() { return root_entity_; }