#ifndef IMGUI_WINDOW_H
#define IMGUI_WINDOW_H

#include <GL/glew.h>
#include <GL/gl.h>
#include <GLFW/glfw3.h>
#include <glm/ext.hpp>

#include <list>
#include <map>

#include "shader.h"
#include "texture.h"
#include "camera.h"
#include "framebuffer.h"
#include "entity.h"
#include "model.h"
#include "scene.h"

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

  Model model_;
  Model model2_;
  glm::mat4 model_matrix_ = glm::mat4(1.0f);
  Scene scene_;
  Camera camera_;
  Framebuffer framebuffer_;

  Mesh grid_mesh_;
  Texture grid_texture_;
  Entity grid_entity_;

  bool demo_ = false;
  bool metrics_ = false;
  bool hovered_ = false;
  float fps_ = 0.0f;
  float ifps_ = 0;
  bool right_click_ = false;
  double mouse_x_ = 0.0;
  double mouse_y_ = 0.0;
  double dx_ = 0.0;
  double dy_ = 0.0;
  bool mouse_moved_ = false;
  std::map<int, bool> keys_;

  void keyCallback(GLFWwindow* window, int key, int scancode, int action,
                   int mods);
  void mouseCallback(GLFWwindow* window, int button, int action, int mods);
  void cursorPosCallback(GLFWwindow* window, double xpos, double ypos);
  void scrollCallback(GLFWwindow* window, double xoffset, double yoffset);
  void updateCamera();
};

#endif  // IMGUI_WINDOW_H