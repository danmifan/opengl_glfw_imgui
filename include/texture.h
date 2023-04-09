#ifndef TEXTURE_H
#define TEXTURE_H

#include <iostream>
#include <GL/glew.h>
#include <GL/gl.h>

class Texture {
 public:
  Texture();
  Texture(GLenum texture_type, int width, int height, const void* image = NULL);
  void bind();
  void unbind();
  void clean();
  GLuint getId();

 private:
  GLuint id_;
  GLenum type_;
};

#endif  // TEXTURE_H