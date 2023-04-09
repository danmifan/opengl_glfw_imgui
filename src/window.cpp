#include "window.h"

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include <chrono>
#include <fstream>
#include <iostream>
#include <streambuf>
#include <thread>

float color[4] = {0.8f, 0.3f, 0.02f, 1.0f};
GLuint fbo;
float size = 1.0f;

MyWindow::MyWindow(int width, int height, int framerate)
    : width_(width), height_(height), framerate_(framerate) {}

void glfw_error_callback(int error, const char *description) {
  std::cout << "GLFW error : " << error << " : " << description << std::endl;
}

int MyWindow::init() {
  glfwSetErrorCallback(glfw_error_callback);
  if (!glfwInit()) {
    return -1;
  }

  window_ = glfwCreateWindow(width_, height_, "Window", NULL, NULL);

  if (!window_) {
    glfwTerminate();
    return -1;
  }

  glfwMakeContextCurrent(window_);
  glfwSwapInterval(1);

  if (glewInit() != GLEW_OK) {
    std::cout << "GLEW ERROR" << std::endl;
    return -1;
  }

  glViewport(0, 0, width_, height_);

  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGuiIO &io = ImGui::GetIO();
  (void)io;
  // io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;  // Enable Docking

  ImGui::StyleColorsDark();

  ImGui_ImplGlfw_InitForOpenGL(window_, true);
  ImGui_ImplOpenGL3_Init("#version 330");

  // Vertices coordinates
  GLfloat vertices[] = {
      -0.5f, -0.5f * float(sqrt(3)) / 3,    0.0f,  // Lower left corner
      0.5f,  -0.5f * float(sqrt(3)) / 3,    0.0f,  // Lower right corner
      0.0f,  0.5f * float(sqrt(3)) * 2 / 3, 0.0f   // Upper corner
  };

  shader_ = Shader("shaders/shader.vert", "shaders/shader.frag");

  glGenFramebuffers(1, &fbo);
  glBindFramebuffer(GL_FRAMEBUFFER, fbo);

  texture_ = Texture(GL_TEXTURE_2D, 800, 600);

  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D,
                         texture_.getId(), 0);

  vao_ = new VAO();

  vbo_ = VBO(vertices, sizeof(vertices));
  vao_->bind();
  vao_->link(vbo_, 0, 3, GL_FLOAT, 3 * sizeof(float), (void *)0);

  vbo_.unbind();
  vao_->unbind();

  camera_ = new Camera(800, 600, glm::vec3(0.0f, 0.0f, 2.0f));

  return 1;
}

void MyWindow::update() {
  glEnable(GL_DEPTH_TEST);

  glfwSetWindowUserPointer(window_, this);
  glfwSetKeyCallback(window_, [](GLFWwindow *window, int key, int scancode,
                                 int action, int mods) {
    static_cast<MyWindow *>(glfwGetWindowUserPointer(window))
        ->keyCallback(window, key, scancode, action, mods);
  });

  while (!glfwWindowShouldClose(window_)) {
    auto t1 = std::chrono::high_resolution_clock::now();

    glViewport(0, 0, 800, 600);
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);
    glClearColor(0, 0, 0, 1.00f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    shader_.activate();
    shader_.setUniform("size", size);
    shader_.setUniform("color", color[0], color[1], color[2], color[3]);

    glm::mat4 mat = camera_->matrix(45.0f, 0.1f, 100.0f);
    shader_.setUniform("cam_matrix", glm::value_ptr(mat));

    glm::vec3 pos = camera_->getPosition();
    // std::cout << pos.x << " " << pos.y << " " << pos.z << std::endl;

    vao_->bind();

    texture_.bind();
    glDrawArrays(GL_TRIANGLES, 0, 3);

    glViewport(0, 0, width_, height_);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glClearColor(0.45f, 0.55f, 0.60f, 1.00f);
    glClear(GL_COLOR_BUFFER_BIT);

    // Render loop

    // ImGUI new frame
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    // Opengl end
    // ImGuiWindowFlags window_flags =
    //     ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
    // const ImGuiViewport *viewport = ImGui::GetMainViewport();
    // ImGui::SetNextWindowPos(viewport->WorkPos);
    // ImGui::SetNextWindowSize(viewport->WorkSize);
    // ImGui::SetNextWindowViewport(viewport->ID);
    // ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
    // ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
    // window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse
    // |
    //                 ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
    // window_flags |=
    //     ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;

    // ImGui::Begin("Dockspace", NULL, window_flags);

    // ImGui::PopStyleVar(2);

    // ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
    // ImGui::DockSpace(dockspace_id, ImVec2(0, 0));

    ImGui::Begin("Test");
    ImGui::Text("x %f y %f z %f", pos.x, pos.y, pos.z);
    // ImGui::Button("Teeest");
    ImGui::Image((void *)(intptr_t)texture_.getId(), ImVec2(800, 600));

    ImGui::End();

    // ImGui::End();

    ImGui::Render();

    // Render loop end

    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    glfwSwapBuffers(window_);

    glfwPollEvents();

    auto t2 = std::chrono::high_resolution_clock::now();

    int ifps =
        std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count();
    int target_ifps = (1.0f / framerate_) * 1000;

    int diff_ifps = target_ifps - ifps;

    if (diff_ifps > 0) {
      std::this_thread::sleep_for(std::chrono::milliseconds(diff_ifps));
    }
  }
}

void MyWindow::shutdown() {
  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplGlfw_Shutdown();

  glfwDestroyWindow(window_);
  glfwTerminate();
  ImGui::DestroyContext();

  vao_->clean();
  vbo_.clean();
  texture_.clean();
  shader_.clean();

  glDeleteFramebuffers(1, &fbo);
}

void MyWindow::keyCallback(GLFWwindow *window, int key, int scancode,
                           int action, int mods) {
  // if (action == GLFW_PRESS) {
  switch (key) {
    case GLFW_KEY_W:
      camera_->forward();
      break;

    case GLFW_KEY_S:
      camera_->backward();
      break;

    case GLFW_KEY_A:
      camera_->left();
      break;

    case GLFW_KEY_D:
      camera_->right();
      break;

    case GLFW_KEY_E:
      camera_->up();
      break;

    case GLFW_KEY_Q:
      camera_->down();
      break;

    default:
      break;
  }
  // }
}