#include "material.h"

void Material::create(Shader* shader, std::string name) {
  shader_ = shader;
  name_ = name;
}

void Material::create(Shader* shader, Texture* albedo, std::string name) {
  shader_ = shader;
  albedo_ = albedo;
  name_ = name;
}

void Material::draw() {
  glActiveTexture(GL_TEXTURE0);

  std::string name = albedo_->getTypeName();

  if (name == "texture_diffuse") {
    setUniform(name, 0);

    albedo_->bind();

    // std::cout << glGetError() << std::endl;
  }
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