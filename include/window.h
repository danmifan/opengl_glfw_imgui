#ifndef IMGUI_WINDOW_H
#define IMGUI_WINDOW_H

#include <GL/glew.h>
#include <GL/gl.h>
#include <GLFW/glfw3.h>
#include <glm/ext.hpp>

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <ImGuizmo.h>

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
  int framerate_;

  Shader shader_;
  Model model_;
  Model model2_;
  Scene scene_;
  Camera camera_;
  Framebuffer framebuffer_;

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
  bool wireframe_ = false;

  int scene_width_ = 800;
  int scene_height_ = 600;

  void keyCallback(GLFWwindow* window, int key, int scancode, int action,
                   int mods);
  void mouseCallback(GLFWwindow* window, int button, int action, int mods);
  void cursorPosCallback(GLFWwindow* window, double xpos, double ypos);
  void scrollCallback(GLFWwindow* window, double xoffset, double yoffset);
  void updateCamera();
  Entity* createGrid(int width, int height);
  Entity* createCube(int x, int y, int z);
  void showEntities(const std::vector<Entity*>& entities);

  unsigned int selected_id_ = -1;
  Entity* selected_entity_ = nullptr;
  glm::vec4 bg_color_ = {0.48904, 0.690881, 0.776892, 1.0f};
  float color_[4];

  ImGuizmo::OPERATION current_operation_ = ImGuizmo::OPERATION::TRANSLATE;
  ImGuizmo::MODE current_mode_ = ImGuizmo::MODE::WORLD;
};

#endif  // IMGUI_WINDOW_H