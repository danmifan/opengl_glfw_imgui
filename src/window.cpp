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
float size = 1.0f;

// clang-format off
  float vertices[] = {
        -0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f,
         0.5f, -0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
         0.5f,  0.5f, -0.5f, 0.0f, 0.0f, 1.0f,
         0.5f,  0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 
        -0.5f,  0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
        -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f, 1.0f, 0.0f, 0.0f,
         0.5f, -0.5f,  0.5f, 0.0f, 1.0f, 0.0f,
         0.5f,  0.5f,  0.5f, 0.0f, 0.0f, 1.0f,
         0.5f,  0.5f,  0.5f, 1.0f, 0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f, 0.0f, 1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f, 0.0f, 0.0f, 1.0f,
        -0.5f,  0.5f,  0.5f, 1.0f, 0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
        -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f,
        -0.5f, -0.5f,  0.5f, 0.0f, 1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f, 0.0f, 0.0f, 1.0f,
         0.5f,  0.5f,  0.5f, 1.0f, 0.0f, 0.0f,
         0.5f,  0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
         0.5f, -0.5f, -0.5f, 0.0f, 0.0f, 1.0f,
         0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f,
         0.5f, -0.5f,  0.5f, 0.0f, 1.0f, 0.0f,
         0.5f,  0.5f,  0.5f, 0.0f, 0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f,
         0.5f, -0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
         0.5f, -0.5f,  0.5f, 0.0f, 0.0f, 1.0f,
         0.5f, -0.5f,  0.5f, 1.0f, 0.0f, 0.0f,
        -0.5f, -0.5f,  0.5f, 0.0f, 1.0f, 0.0f,
        -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, 1.0f,
        -0.5f,  0.5f, -0.5f, 1.0f, 0.0f, 0.0f,
         0.5f,  0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
         0.5f,  0.5f,  0.5f, 0.0f, 0.0f, 1.0f,
         0.5f,  0.5f,  0.5f, 1.0f, 0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f, 0.0f, 1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f, 0.0f, 0.0f, 1.0f
    };
// clang-format on
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

  shader_ = Shader("shaders/shader.vert", "shaders/shader.frag");

  framebuffer_.create(800, 600);

  vao_.create(vertices, sizeof(vertices));
  vao_.bind();
  vao_.link(0, 3, GL_FLOAT, 6 * sizeof(float), (void *)0);
  vao_.link(1, 3, GL_FLOAT, 6 * sizeof(float), (void *)(3 * sizeof(float)));
  vao_.unbind();

  camera_.create(800, 600, glm::vec3(0.0, 0.0, 2.0));

  return 1;
}

void MyWindow::update() {
  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LESS);

  glfwSetWindowUserPointer(window_, this);
  glfwSetKeyCallback(window_, [](GLFWwindow *window, int key, int scancode,
                                 int action, int mods) {
    static_cast<MyWindow *>(glfwGetWindowUserPointer(window))
        ->keyCallback(window, key, scancode, action, mods);
  });

  while (!glfwWindowShouldClose(window_)) {
    auto t1 = std::chrono::high_resolution_clock::now();

    glViewport(0, 0, 800, 600);
    framebuffer_.bind();
    glClearColor(0, 0, 0, 1.00f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    shader_.activate();
    shader_.setUniform("size", size);
    // shader_.setUniform("color", color[0], color[1], color[2], color[3]);

    glm::mat4 mat = camera_.matrix(45.0f, 0.1f, 100.0f);
    shader_.setUniform("cam_matrix", glm::value_ptr(mat));

    glm::vec3 pos = camera_.getPosition();

    // texture_.bind();
    vao_.bind();
    glDrawArrays(GL_TRIANGLES, 0, sizeof(vertices));
    // vao_.unbind();
    // glDisableVertexAttribArray(0);
    // glDisableVertexAttribArray(1);
    // glUseProgram(0);

    glViewport(0, 0, width_, height_);
    framebuffer_.unbind();
    glClearColor(0.45f, 0.55f, 0.60f, 1.00f);
    glClear(GL_COLOR_BUFFER_BIT);

    // Render loop

    // ImGUI new frame
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    if (ImGui::BeginMainMenuBar()) {
      if (ImGui::BeginMenu("Menu")) {
        ImGui::MenuItem("Demo", NULL, &demo_);

        ImGui::EndMenu();
      }
      ImGui::EndMainMenuBar();
    }

    if (demo_) {
      ImGui::ShowDemoWindow();
    }

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
    ImGui::Image((void *)(intptr_t)framebuffer_.getTextureId(),
                 ImVec2(800, 600), ImVec2(0, 1), ImVec2(1, 0));

    ImGui::End();

    ImGuiWindowFlags window_flags =
        ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoDocking |
        ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings |
        ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav;
    ImGui::SetNextWindowBgAlpha(0.35f);  // Transparent background
    if (ImGui::Begin("Example: Simple overlay", NULL, window_flags)) {
      ImGui::Text("ifps : %i ms", ifps_);
    }
    ImGui::End();

    ImGui::Render();

    // Render loop end

    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    glfwSwapBuffers(window_);

    glfwPollEvents();

    auto t2 = std::chrono::high_resolution_clock::now();

    ifps_ =
        std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count();
    int target_ifps = (1.0f / framerate_) * 1000;

    int diff_ifps = target_ifps - ifps_;

    if (diff_ifps > 0) {
      std::this_thread::sleep_for(std::chrono::milliseconds(diff_ifps));
    }
  }
}

void MyWindow::keyCallback(GLFWwindow *window, int key, int scancode,
                           int action, int mods) {
  // if (action == GLFW_PRESS) {
  switch (key) {
    case GLFW_KEY_W:
      camera_.forward();
      break;

    case GLFW_KEY_S:
      camera_.backward();
      break;

    case GLFW_KEY_A:
      camera_.left();
      break;

    case GLFW_KEY_D:
      camera_.right();
      break;

    case GLFW_KEY_E:
      camera_.up();
      break;

    case GLFW_KEY_Q:
      camera_.down();
      break;

    default:
      break;
  }
  // }
}

void MyWindow::shutdown() {
  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplGlfw_Shutdown();

  glfwDestroyWindow(window_);
  glfwTerminate();
  ImGui::DestroyContext();

  vao_.clean();
  // texture_.clean();
  shader_.clean();
  framebuffer_.clean();
}