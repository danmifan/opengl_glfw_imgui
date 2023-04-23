#ifndef MATERIAL_H
#define MATERIAL_H

#include "texture.h"
#include "shader.h"

class Material {
 public:
  void create(Shader* shader, std::string name);
  void create(Shader* shader, Texture* diffuse, std::string name);
  void draw();

  void setDiffuse(Texture* diffuse);
  Texture* getDiffuse();

  void setNormals(Texture* normals);

  void setUniform(std::string name, GLfloat x);
  void setUniform(std::string name, GLfloat x, GLfloat y, GLfloat z, GLfloat w);
  void setUniform(std::string name, GLfloat* mat);
  void setUniform(std::string name, GLint i);

  std::string getName();

 private:
  Texture* diffuse_;
  Texture* normals_;
  Shader* shader_;
  std::string name_;
};

#endif  // MATERIAL_H