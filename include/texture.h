#ifndef TEXTURE_H
#define TEXTURE_H

#include <iostream>
#include <GL/glew.h>
#include <GL/gl.h>

class Texture {
 public:
  GLuint create(GLenum target, GLint internal_format, int width, int height,
                GLenum format, GLenum type, const void* image = NULL);
  void loadFromImage(std::string path, std::string type_name);
  void loadDDSFile(std::string path, std::string type_name);
  void bind();
  void unbind();
  void clean();
  GLuint getId();
  std::string getPath();
  std::string getTypeName();

 private:
  GLuint id_;
  GLenum target_;
  std::string path_ = "";
  std::string type_name_ = "";
};

#endif  // TEXTURE_H

//  glTexImage2D(type_, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE,
//                image);