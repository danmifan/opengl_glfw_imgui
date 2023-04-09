#ifndef IMGUI_WINDOW_H
#define IMGUI_WINDOW_H

#include <GL/glew.h>
#include <GL/gl.h>
#include <GLFW/glfw3.h>
#include <glm/ext.hpp>

#include <list>

#include "shader.h"
#include "texture.h"
#include "vao.h"
#include "camera.h"

class MyWindow {
 public:
  MyWindow(int width, int height, int framerate = 60);
  int init();
  void update();
  void shutdown();

 private:
  GLFWwindow* window_;
  int width_;
  int height_;
  unsigned char* image_data_;
  int framerate_;
  Shader shader_;
  Texture texture_;
  VBO vbo_;
  VAO* vao_;
  Camera* camera_;

  void keyCallback(GLFWwindow* window, int key, int scancode, int action,
                   int mods);
};

#endif  // IMGUI_WINDOW_H