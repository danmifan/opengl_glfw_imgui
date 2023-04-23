#include "model.h"

Entity* Model::load(std::string path, Shader* shader) {
  Assimp::Importer importer;
#warning flip uv here
  const aiScene* scene = importer.ReadFile(
      path.c_str(), aiProcess_Triangulate | aiProcess_FlipUVs);

  if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE ||
      !scene->mRootNode) {
    std::cout << "ERROR::ASSIMP::" << importer.GetErrorString() << std::endl;
    return nullptr;
  }

  path_ = path;

  shader_ = shader;

  std::cout << "Importing model " << path << std::endl;

  Entity* root_entity = new Entity();
  root_entity->setName(std::string(scene->mRootNode->mName.C_Str()));

  // Load materials
  processMaterials(scene);

  processNode(scene->mRootNode, scene, root_entity);

  std::cout << "Model imported" << std::endl;

  return root_entity;
}

void Model::processNode(aiNode* node, const aiScene* scene, Entity* entity) {
  std::cout << "Nb : " << node->mNumMeshes << " " << node->mNumChildren << " "
            << std::string(node->mName.C_Str()) << std::endl;

  for (unsigned int i = 0; i < node->mNumMeshes; i++) {
    aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
    Mesh* my_mesh = processMesh(mesh);
    Assets::get().meshes.push_back(my_mesh);
    Entity* child = new Entity();
    child->create(my_mesh, materials_[mesh->mMaterialIndex],
                  std::string(mesh->mName.C_Str()));
    entity->addChild(child);
  }

  for (unsigned int i = 0; i < node->mNumChildren; i++) {
    processNode(node->mChildren[i], scene, entity);
  }
}

Mesh* Model::processMesh(aiMesh* mesh) {
  std::vector<Vertex> vertices;
  std::vector<unsigned int> indices;

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

  std::string name = std::string(mesh->mName.C_Str());

  Mesh* my_mesh = new Mesh();
  my_mesh->create(vertices, indices, name);
  return my_mesh;
}

Texture* Model::loadMaterialTexture(aiMaterial* material, aiTextureType type,
                                    TextureType my_type) {
  if (material->GetTextureCount(type) > 0) {
    Texture* texture = new Texture;

    aiString texture_name_ai;
    material->GetTexture(type, 0, &texture_name_ai);
    texture->setName(std::string(texture_name_ai.C_Str()));
    std::string tmp_path = path_.substr(0, path_.find_last_of('/'));
    std::string texture_path =
        tmp_path + "/" + std::string(texture_name_ai.C_Str());

    if (texture_path.find(".dds") != std::string::npos) {
      texture->loadDDSFile(texture_path, my_type);
    } else {
      texture->loadFromImage(texture_path, my_type);
    }

    return texture;
  } else {
    return nullptr;
  }
}

void Model::processMaterials(const aiScene* scene) {
  unsigned int nb_materials = scene->mNumMaterials;

  for (unsigned int i = 0; i < nb_materials; i++) {
    aiMaterial* material = scene->mMaterials[i];
    Material* mat = new Material;

    Texture* diffuse = loadMaterialTexture(material, aiTextureType_DIFFUSE,
                                           TextureType::DIFFUSE);
    Texture* normals = loadMaterialTexture(material, aiTextureType_NORMALS,
                                           TextureType::NORMALS);

    std::string name = std::string(material->GetName().C_Str());

    mat->create(shader_, diffuse, name);

    Assets::get().materials.push_back(mat);

    materials_[i] = mat;
  }
}
