#include "texture.h"

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

// void Texture::texUnit(Shader& shader, const char* uniform, GLuint unit)
// {
// 	// Gets the location of the uniform
// 	GLuint texUni = glGetUniformLocation(shader.ID, uniform);
// 	// Shader needs to be activated before changing the value of a uniform
// 	shader.Activate();
// 	// Sets the value of the uniform
// 	glUniform1i(texUni, unit);
// }

void Texture::bind() { glBindTexture(target_, id_); }

void Texture::unbind() { glBindTexture(target_, 0); }

void Texture::clean() { glDeleteTextures(1, &id_); }

GLuint Texture::getId() { return id_; }