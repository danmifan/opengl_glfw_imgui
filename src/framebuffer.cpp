#include "framebuffer.h"

void Framebuffer::create(int width, int height) {
  glGenFramebuffers(1, &fbo_id_);
  glBindFramebuffer(GL_FRAMEBUFFER, fbo_id_);

  texture_id_ = texture_.create(GL_TEXTURE_2D, width, height);

  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D,
                         texture_id_, 0);
}

void Framebuffer::bind() {
  glBindFramebuffer(GL_FRAMEBUFFER, fbo_id_);
  // Really useful here ?
  texture_.bind();
}

void Framebuffer::unbind() {
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
  texture_.unbind();
}

void Framebuffer::clean() {
  glDeleteFramebuffers(1, &fbo_id_);
  texture_.clean();
}

GLuint Framebuffer::getTextureId() { return texture_id_; }