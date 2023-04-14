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
  GLuint getColorTextureId();
  GLuint getDepthTextureId();

 private:
  GLuint fbo_id_;
  Texture color_texture_;
  Texture depth_texture_;
};

#endif  // FRAMEBUFFER_H