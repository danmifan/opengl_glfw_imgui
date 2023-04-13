#ifndef TEXTURE_H
#define TEXTURE_H

#include <iostream>
#include <GL/glew.h>
#include <GL/gl.h>

class Texture {
 public:
  GLuint create(GLenum target, GLint internal_format, int width, int height,
                GLenum format, GLenum type, const void* image = NULL);
  void bind();
  void unbind();
  void clean();
  GLuint getId();

 private:
  GLuint id_;
  GLenum target_;
};

#endif  // TEXTURE_H

//  glTexImage2D(type_, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE,
//                image);