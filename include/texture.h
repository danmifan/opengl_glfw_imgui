#ifndef TEXTURE_H
#define TEXTURE_H

#include <iostream>
#include <GL/glew.h>
#include <GL/gl.h>

enum TextureType { DIFFUSE = 0, NORMALS = 1 };

class Texture {
 public:
  GLuint create(GLenum target, GLint internal_format, int width, int height,
                GLenum format, GLenum type, const void* image = NULL);
  void loadFromImage(std::string path, TextureType type);
  void loadDDSFile(std::string path, TextureType type);
  void bind();
  void unbind();
  void clean();
  GLuint getId();
  std::string getPath();
  TextureType getType();

  void setName(std::string name);
  std::string getName();

 private:
  GLuint id_;
  GLenum target_;
  std::string path_ = "default";
  TextureType type_;
  std::string name_;
};

#endif  // TEXTURE_H

//  glTexImage2D(type_, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE,
//                image);