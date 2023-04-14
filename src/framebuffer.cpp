#include "framebuffer.h"

void Framebuffer::create(int width, int height) {
  glGenFramebuffers(1, &fbo_id_);
  glBindFramebuffer(GL_FRAMEBUFFER, fbo_id_);

  color_texture_.create(GL_TEXTURE_2D, GL_RGB, width, height, GL_RGB,
                        GL_UNSIGNED_BYTE);

  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D,
                         color_texture_.getId(), 0);

  depth_texture_.create(GL_TEXTURE_2D, GL_DEPTH_COMPONENT32F, width, height,
                        GL_DEPTH_COMPONENT, GL_FLOAT);

  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D,
                         depth_texture_.getId(), 0);
}

void Framebuffer::bind() {
  glBindFramebuffer(GL_FRAMEBUFFER, fbo_id_);
  // Really useful here ?
  color_texture_.bind();
}

void Framebuffer::unbind() {
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
  color_texture_.unbind();
}

void Framebuffer::clean() {
  glDeleteFramebuffers(1, &fbo_id_);
  color_texture_.clean();
}

GLuint Framebuffer::getColorTextureId() { return color_texture_.getId(); }

GLuint Framebuffer::getDepthTextureId() { return depth_texture_.getId(); }
