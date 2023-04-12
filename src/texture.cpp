#include "texture.h"

Texture::Texture() {}

Texture::Texture(GLenum texture_type, int width, int height,
                 const void* image) {
  type_ = texture_type;
  glGenTextures(1, &id_);
  glBindTexture(type_, id_);

  glTexImage2D(type_, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE,
               image);

  glTexParameteri(type_, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(type_, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  // glTexParameteri(type_, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  // glTexParameteri(type_, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

  // glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
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

void Texture::bind() { glBindTexture(type_, id_); }

void Texture::unbind() { glBindTexture(type_, 0); }

void Texture::clean() { glDeleteTextures(1, &id_); }

GLuint Texture::getId() { return id_; }