#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H

#include <iostream>
#include <GL/glew.h>
#include <GL/gl.h>
#include "texture.h"

class Framebuffer {
 public:
  void create(int width, int height);
  void bind();
  void unbind();
  void clean();
  GLuint getTextureId();

 private:
  GLuint fbo_id_;
  GLuint texture_id_;
  Texture texture_;
};

#endif  // FRAMEBUFFER_H