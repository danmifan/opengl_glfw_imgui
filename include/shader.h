#ifndef SHADER_H
#define SHADER_H

#include <iostream>
#include <GL/glew.h>
#include <GL/gl.h>

class Shader {
 public:
  Shader();
  Shader(std::string vertex_file, std::string fragment_file);
  void activate();
  void clean();
  GLuint getID();

 private:
  std::string loadShaderFile(std::string filename);
  void compileErrorCheck(GLuint shader);
  GLuint id_;
};

#endif  // SHADER_H