#include "material.h"

void Material::create(Shader* shader, std::string name) {
  shader_ = shader;
  name_ = name;
}

void Material::create(Shader* shader, Texture* diffuse, std::string name) {
  shader_ = shader;
  diffuse_ = diffuse;
  name_ = name;
}

void Material::draw() {
  shader_->activate();
  glActiveTexture(GL_TEXTURE0);

  // std::string name = diffuse_->getTypeName();

  setUniform("texture_diffuse", 0);

  diffuse_->bind();

  // std::cout << glGetError() << std::endl;
  glActiveTexture(GL_TEXTURE0);
}

void Material::setUniform(std::string name, GLfloat x) {
  glUniform1f(glGetUniformLocation(shader_->getID(), name.c_str()), x);
}

void Material::setUniform(std::string name, GLfloat x, GLfloat y, GLfloat z,
                          GLfloat w) {
  glUniform4f(glGetUniformLocation(shader_->getID(), name.c_str()), x, y, z, w);
}

void Material::setUniform(std::string name, GLfloat* mat) {
  glUniformMatrix4fv(glGetUniformLocation(shader_->getID(), name.c_str()), 1,
                     GL_FALSE, mat);
}

void Material::setUniform(std::string name, GLint i) {
  glUniform1i(glGetUniformLocation(shader_->getID(), name.c_str()), i);
}

void Material::setDiffuse(Texture* diffuse) { diffuse_ = diffuse; }

Texture* Material::getDiffuse() { return diffuse_; }

void Material::setNormals(Texture* normals) { normals_ = normals; }

// unsigned int diffuseNr = 1;
// unsigned int specularNr = 1;
// unsigned int normalNr = 1;
// unsigned int heightNr = 1;

// for (unsigned int i = 0; i < textures_.size(); i++) {
//   glActiveTexture(GL_TEXTURE0 + i);

//   std::string number;
//   std::string name = textures_[i].getTypeName();

//   // std::cout << name << std::endl;

//   if (name == "texture_diffuse") {
//     number = std::to_string(diffuseNr++);

//     // std::cout << name + number << std::endl;
//     // std::cout << i << std::endl;

//     shader->setUniform(name + number, (GLint)i);

//     textures_[i].bind();

//     // std::cout << glGetError() << std::endl;
//   }
// }

std::string Material::getName() { return name_; }