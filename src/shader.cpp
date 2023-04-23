#include "shader.h"

#include <fstream>
#include <streambuf>

Shader::Shader() {}

Shader::Shader(std::string vertex_file, std::string fragment_file) {
  std::string vertex_code_str = loadShaderFile(vertex_file);
  std::string fragment_code_str = loadShaderFile(fragment_file);

  const char* vertex_code = vertex_code_str.c_str();
  const char* fragment_code = fragment_code_str.c_str();

  GLuint vertex_shader = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vertex_shader, 1, &vertex_code, NULL);
  glCompileShader(vertex_shader);
  compileErrorCheck(vertex_shader);

  GLuint fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragment_shader, 1, &fragment_code, NULL);
  glCompileShader(fragment_shader);
  compileErrorCheck(fragment_shader);

  id_ = glCreateProgram();

  glAttachShader(id_, vertex_shader);
  glAttachShader(id_, fragment_shader);
  glLinkProgram(id_);

  // check for linking errors
  int success;
  char info_log[256];
  glGetProgramiv(id_, GL_LINK_STATUS, &success);
  if (!success) {
    glGetProgramInfoLog(id_, sizeof(info_log), NULL, info_log);
    std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n"
              << info_log << std::endl;
  }

  glDeleteShader(vertex_shader);
  glDeleteShader(fragment_shader);
}

std::string Shader::loadShaderFile(std::string filename) {
  std::ifstream input_file(filename, std::ifstream::in);

  if (!input_file.is_open()) {
    std::cout << "File : " << filename << " could not be opened" << std::endl;
  }

  return std::string(std::istreambuf_iterator<char>(input_file),
                     std::istreambuf_iterator<char>());
}

void Shader::compileErrorCheck(GLuint shader) {
  GLint is_compiled = 0;
  glGetShaderiv(shader, GL_COMPILE_STATUS, &is_compiled);

  if (is_compiled == GL_FALSE) {
    GLint max_length = 0;
    glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &max_length);

    GLchar error_log[max_length];
    glGetShaderInfoLog(shader, max_length, &max_length, error_log);

    std::cout << "Shader compilation failed" << std::endl;
    std::cout << error_log << std::endl;

    glDeleteShader(shader);

    exit(-1);
  } else {
    std::cout << "Shader compilation successful" << std::endl;
  }
}

GLuint Shader::getID() { return id_; }

void Shader::activate() { glUseProgram(id_); }

void Shader::clean() { glDeleteProgram(id_); }

void Shader::setUniform(std::string name, GLfloat* mat) {
  glUniformMatrix4fv(glGetUniformLocation(id_, name.c_str()), 1, GL_FALSE, mat);
}
