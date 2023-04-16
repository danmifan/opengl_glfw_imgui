#include "texture.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

GLuint Texture::create(GLenum target, GLint internal_format, int width,
                       int height, GLenum format, GLenum type,
                       const void* image) {
  target_ = target;
  glGenTextures(1, &id_);
  glBindTexture(target_, id_);

  glTexImage2D(target_, 0, internal_format, width, height, 0, format, type,
               image);

  glTexParameteri(target_, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(target_, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  // glTexParameteri(target_, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  // glTexParameteri(target_, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

  // glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);

  return id_;
}

void Texture::loadFromImage(std::string path, std::string type_name) {
  glGenTextures(1, &id_);

  int width, height, nb_components;
  unsigned char* data =
      stbi_load(path.c_str(), &width, &height, &nb_components, 0);

  if (data) {
    GLenum format;
    switch (nb_components) {
      case 1:
        format = GL_RED;
        break;

      case 3:
        format = GL_RGB;
        break;

      case 4:
        format = GL_RGBA;
        break;
    }

    glBindTexture(GL_TEXTURE_2D, id_);
    glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format,
                 GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                    GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    std::cout << "Texture : " << path << " loaded" << std::endl;
    std::cout << width << " " << height << " "
              << " " << type_name << " " << nb_components << std::endl;

    stbi_image_free(data);

  } else {
    std::cout << "Texture loading failed : " << path << std::endl;
    stbi_image_free(data);
  }

  type_name_ = type_name;
  path_ = path;
}

// void Texture::texUnit(Shader& shader, const char* uniform, GLuint unit)
// {
// 	// Gets the location of the uniform
// 	GLuint texUni = glGetUniformLocation(shader.ID, uniform);
// 	// Shader needs to be activated before changing the value of a uniform
// 	shader.Activate();
// 	// Sets the value of the uniform
// 	glUniform1i(texUni, unit);
// }

void Texture::bind() { glBindTexture(GL_TEXTURE_2D, id_); }

void Texture::unbind() { glBindTexture(target_, 0); }

void Texture::clean() { glDeleteTextures(1, &id_); }

GLuint Texture::getId() { return id_; }

std::string Texture::getPath() { return path_; }

std::string Texture::getTypeName() { return type_name_; }
