#include "mesh.h"

void Mesh::create(std::vector<Vertex> vertices, std::vector<GLuint> indices,
                  std::vector<Texture> textures, std::string name) {
  glGenVertexArrays(1, &vao_);
  glGenBuffers(1, &vbo_);
  glGenBuffers(1, &ebo_);

  glBindVertexArray(vao_);

  glBindBuffer(GL_ARRAY_BUFFER, vbo_);
  glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex),
               vertices.data(), GL_STATIC_DRAW);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo_);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint),
               indices.data(), GL_STATIC_DRAW);

  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)0);
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                        (void *)offsetof(Vertex, normal));
  glEnableVertexAttribArray(2);
  glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                        (void *)offsetof(Vertex, tex_coords));

  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);

  vertices_ = vertices;
  indices_ = indices;
  textures_ = textures;
  name_ = name;
}

void Mesh::draw(Shader *shader) {
  unsigned int diffuseNr = 1;
  unsigned int specularNr = 1;
  unsigned int normalNr = 1;
  unsigned int heightNr = 1;

  for (unsigned int i = 0; i < textures_.size(); i++) {
    glActiveTexture(GL_TEXTURE0 + i);

    std::string number;
    std::string name = textures_[i].getTypeName();

    if (name == "texture_diffuse") {
      number = std::to_string(diffuseNr++);

      // std::cout << name + number << std::endl;
      // std::cout << i << std::endl;

      shader->setUniform(name + number, (GLint)i);

      textures_[i].bind();
    }
  }

  // std::cout << name_ << std::endl;
  // std::cout << vertices_.size() << std::endl;
  // std::cout << indices_.size() << std::endl;

  glBindVertexArray(vao_);
  glDrawElements(GL_TRIANGLES, indices_.size(), GL_UNSIGNED_INT, 0);
  glBindVertexArray(0);

  glActiveTexture(GL_TEXTURE0);
}

std::string Mesh::getName() { return name_; }

#warning TODO : clean