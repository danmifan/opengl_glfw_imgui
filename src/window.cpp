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

  glfwSetWindowUserPointer(window_, this);
  glfwSetKeyCallback(window_, [](GLFWwindow *window, int key, int scancode,
                                 int action, int mods) {
    static_cast<MyWindow *>(glfwGetWindowUserPointer(window))
        ->keyCallback(window, key, scancode, action, mods);
  });

  glfwSetMouseButtonCallback(
      window_, [](GLFWwindow *window, int button, int action, int mods) {
        static_cast<MyWindow *>(glfwGetWindowUserPointer(window))
            ->mouseCallback(window, button, action, mods);
      });

  glfwSetCursorPosCallback(
      window_, [](GLFWwindow *window, double xpos, double ypos) {
        static_cast<MyWindow *>(glfwGetWindowUserPointer(window))
            ->cursorPosCallback(window, xpos, ypos);
      });

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

  mesh_.create(vertices, sizeof(vertices));
  for (int i = 0; i < 3; i++) {
    entity_[i].create(&mesh_, &shader_);
  }

  // vao_.create(vertices, sizeof(vertices));
  // vao_.bind();
  // vao_.link(0, 3, GL_FLOAT, 6 * sizeof(float), (void *)0);
  // vao_.link(1, 3, GL_FLOAT, 6 * sizeof(float), (void *)(3 * sizeof(float)));
  // vao_.unbind();

  camera_.create(800, 600, glm::vec3(0.0, 0.0, 2.0), 45.0f, 0.1f, 100.0f);

  return 1;
}

void MyWindow::update() {
  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LESS);

  mouse_moved_ = false;

  while (!glfwWindowShouldClose(window_)) {
    auto t1 = std::chrono::high_resolution_clock::now();

    dx_ = 0.0;
    dy_ = 0.0;
    mouse_moved_ = false;

    glViewport(0, 0, 800, 600);
    framebuffer_.bind();
    glClearColor(0, 0, 0, 1.00f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    shader_.activate();
    // shader_.setUniform("color", color[0], color[1], color[2], color[3]);

    glm::mat4 view_proj = camera_.getViewProjMatrix();
    shader_.setUniform("view_projection", glm::value_ptr(view_proj));

    // texture_.bind();
    // vao_.bind();

    static float angle = 0;
    angle += 10.0 * ifps_;

    for (int i = 0; i < 3; i++) {
      entity_[i].setPosition(glm::vec3(5 * i, 0, 0));
      entity_[i].setRotation(glm::vec3(0.0f, angle, angle));
      entity_[i].draw();
      // glm::vec3 angles = entity_[i].getAngles();
      // std::cout << angles.x << " " << angles.y << " " << angles.z <<
      // std::endl;
    }

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
        ImGui::MenuItem("Metrics", NULL, &metrics_);

        ImGui::EndMenu();
      }
      ImGui::EndMainMenuBar();
    }

    if (demo_) {
      ImGui::ShowDemoWindow();
    }

    if (metrics_) {
      ImGui::ShowMetricsWindow();
    }

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

    glm::vec3 pos = camera_.getPosition();

    ImGui::Begin("Render");
    ImGui::Text("x %f y %f z %f", pos.x, pos.y, pos.z);
    ImGui::Image((void *)(intptr_t)framebuffer_.getColorTextureId(),
                 ImVec2(800, 600), ImVec2(0, 1), ImVec2(1, 0));
    hovered_ = ImGui::IsItemHovered();

    ImGui::End();

    ImGuiWindowFlags window_flags =
        ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoDocking |
        ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings |
        ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav;
    ImGui::SetNextWindowBgAlpha(0.35f);  // Transparent background
    if (ImGui::Begin("Example: Simple overlay", NULL, window_flags)) {
      ImGui::Text("ifps : %f s", ifps_);
    }
    ImGui::End();

    ImGui::Render();

    // Render loop end

    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    glfwSwapBuffers(window_);

    glfwPollEvents();

    updateCamera();

    auto t2 = std::chrono::high_resolution_clock::now();

    float render_time_ms =
        std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count();
    int target_ifps = (1.0f / framerate_) * 1000;

    int diff_ifps = target_ifps - render_time_ms;
    // std::cout << render_time << std::endl;
    // std::cout << diff_ifps << std::endl;

    if (diff_ifps > 0) {
      std::this_thread::sleep_for(std::chrono::milliseconds(diff_ifps));
    }

    auto t3 = std::chrono::high_resolution_clock::now();

    int total_time_ms =
        std::chrono::duration_cast<std::chrono::milliseconds>(t3 - t1).count();

    ifps_ = total_time_ms / 1000.0f;
    // std::cout << ifps_ << std::endl;
  }
}

void MyWindow::keyCallback(GLFWwindow * /*window*/, int key, int /*scancode*/,
                           int action, int /*mods*/) {
  keys_[key] = action;
}

void MyWindow::updateCamera() {
  if (keys_[GLFW_KEY_W]) {
    camera_.forward();
  }

  if (keys_[GLFW_KEY_S]) {
    camera_.backward();
  }

  if (keys_[GLFW_KEY_A]) {
    camera_.left();
  }

  if (keys_[GLFW_KEY_D]) {
    camera_.right();
  }

  if (keys_[GLFW_KEY_E]) {
    camera_.up();
  }

  if (keys_[GLFW_KEY_Q]) {
    camera_.down();
  }

  if (mouse_moved_ && right_click_ && hovered_) {
    float yaw = dx_;
    float pitch = dy_;
    // camera_.rotateY(-yaw * ifps_s_);
    camera_.rotate(-pitch, -yaw, 0);
  }
}

void MyWindow::mouseCallback(GLFWwindow * /*window*/, int button, int action,
                             int /*mods*/) {
  switch (button) {
    // Left click
    case GLFW_MOUSE_BUTTON_1:
      break;

    // Right click
    case GLFW_MOUSE_BUTTON_2:
      if (action == GLFW_PRESS) {
        right_click_ = true;
      } else if (action == GLFW_RELEASE) {
        right_click_ = false;
      }
      break;

    default:
      break;
  }
}

void MyWindow::cursorPosCallback(GLFWwindow * /*window*/, double xpos,
                                 double ypos) {
  dx_ = xpos - mouse_x_;
  dy_ = ypos - mouse_y_;
  mouse_x_ = xpos;
  mouse_y_ = ypos;
  mouse_moved_ = true;
}

void MyWindow::shutdown() {
  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplGlfw_Shutdown();

  glfwDestroyWindow(window_);
  glfwTerminate();
  ImGui::DestroyContext();

  // vao_.clean();
  // texture_.clean();
  shader_.clean();
  framebuffer_.clean();
}