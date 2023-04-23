#include "window.h"

#include <glm/gtx/string_cast.hpp>

#include <chrono>
#include <fstream>
#include <iostream>
#include <streambuf>
#include <thread>

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

  glfwSetScrollCallback(
      window_, [](GLFWwindow *window, double xoffset, double yoffset) {
        static_cast<MyWindow *>(glfwGetWindowUserPointer(window))
            ->scrollCallback(window, xoffset, yoffset);
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
  io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;  // Enable Docking
  io.ConfigWindowsMoveFromTitleBarOnly = true;

  ImGui::StyleColorsDark();

  ImGui_ImplGlfw_InitForOpenGL(window_, true);
  ImGui_ImplOpenGL3_Init("#version 330");

  shader_ = Shader("shaders/shader.vert", "shaders/shader.frag");

  framebuffer_.create(scene_width_, scene_height_);

  Entity *tello =
      model_.load("models/tello/obj/DJITelloWhiteVray2015SC.obj", &shader_);
  tello->setScale(glm::vec3(0.01, 0.01, 0.01));

  Entity *grid = createGrid(200, 200);
  grid->setRotation(glm::vec3(90.0f, 0.0f, 0.0f));

  Entity *cube = createCube(0, 0, 0);

  scene_.addEntity(tello);
  scene_.addEntity(cube);
  scene_.addEntity(grid);

  camera_.create(scene_width_, scene_height_, glm::vec3(0.0, 1.0, 2.0), 45.0f,
                 0.1f, 100.0f);

  renderer_.create(&scene_, &camera_);

  return 1;
}

void MyWindow::update() {
  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LESS);
  // glEnable(GL_CULL_FACE);
  // glFrontFace(GL_CCW);
  // glCullFace(GL_CULL_FACE);

  ImGuizmo::SetOrthographic(false);

  mouse_moved_ = false;

  while (!glfwWindowShouldClose(window_)) {
    auto t1 = std::chrono::high_resolution_clock::now();

    dx_ = 0.0;
    dy_ = 0.0;
    mouse_moved_ = false;

    glViewport(0, 0, scene_width_, scene_height_);
    framebuffer_.bind();
    glClearColor(bg_color_.x, bg_color_.y, bg_color_.z, bg_color_.w);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    if (wireframe_) {
      glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    } else {
      glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }

    // Draw scene graph
    renderer_.draw();

    framebuffer_.unbind();

    glViewport(0, 0, width_, height_);
    glClearColor(0.45f, 0.55f, 0.60f, 1.00f);
    glClear(GL_COLOR_BUFFER_BIT);

    // Render loop

    // ImGUI new frame
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
    ImGuizmo::BeginFrame();

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

    ImGuiWindowFlags dockspace_flags =
        ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
    const ImGuiViewport *viewport = ImGui::GetMainViewport();
    ImGui::SetNextWindowPos(viewport->WorkPos);
    ImGui::SetNextWindowSize(viewport->WorkSize);
    ImGui::SetNextWindowViewport(viewport->ID);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
    dockspace_flags |= ImGuiWindowFlags_NoTitleBar |
                       ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize |
                       ImGuiWindowFlags_NoMove;
    dockspace_flags |=
        ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;

    ImGui::Begin("Dockspace", NULL, dockspace_flags);

    ImGui::PopStyleVar(2);

    ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
    ImGui::DockSpace(dockspace_id, ImVec2(0, 0));

    glm::vec3 pos = camera_.getPosition();

    ImGui::Begin("Options");
    if (ImGui::Button("wireframe")) {
      wireframe_ = !wireframe_;
    }

    if (ImGui::CollapsingHeader("Background color")) {
      if (ImGui::ColorPicker4("Background", color_)) {
        bg_color_.x = color_[0];
        bg_color_.y = color_[1];
        bg_color_.z = color_[2];
        bg_color_.w = color_[3];
      }
    }

    ImGui::End();

    ImGui::Begin("Render");
    ImGui::Text("x %f y %f z %f", pos.x, pos.y, pos.z);
    if (ImGui::Button("Pos")) {
      current_operation_ = ImGuizmo::TRANSLATE;
    }

    ImGui::SameLine();
    if (ImGui::Button("Rot")) {
      current_operation_ = ImGuizmo::ROTATE;
    }
    ImGui::SameLine();

    if (ImGui::Button("Scale")) {
      current_operation_ = ImGuizmo::SCALE;
    }
    ImGui::SameLine();

    if (ImGui::Button("World")) {
      current_mode_ = ImGuizmo::MODE::WORLD;
    }
    ImGui::SameLine();

    if (ImGui::Button("Local")) {
      current_mode_ = ImGuizmo::MODE::LOCAL;
    }
    ImGui::Image((void *)(intptr_t)framebuffer_.getColorTextureId(),
                 ImVec2(scene_width_, scene_height_), ImVec2(0, 1),
                 ImVec2(1, 0));

    hovered_ = ImGui::IsItemHovered();

    float max_x = ImGui::GetWindowPos().x + scene_width_;
    float max_y = ImGui::GetWindowPos().y + scene_height_;

    ImGui::PushClipRect(ImGui::GetWindowPos(), ImVec2(max_x, max_y), true);

    ImGuizmo::SetDrawlist();

    ImGuizmo::SetRect(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y,
                      scene_width_, scene_height_);

    if (selected_entity_) {
      glm::mat4 world_transform = selected_entity_->getWorldTransform();
      if (ImGuizmo::Manipulate(glm::value_ptr(camera_.getViewMatrix()),
                               glm::value_ptr(camera_.getProjMatrix()),
                               current_operation_, current_mode_,
                               glm::value_ptr(world_transform))) {
        glm::mat4 local_transform_ =
            glm::inverse(selected_entity_->getParentTransform()) *
            world_transform;
        selected_entity_->setTransform(local_transform_);
      }
    }

    ImGui::PopClipRect();

    ImGui::End();

    ImGui::Begin("Properties");
    if (selected_entity_) {
      ImGui::Text("ID : %i", selected_entity_->getID());
      glm::vec3 position = selected_entity_->getPosition();
      glm::vec3 rotation = selected_entity_->getAngles();
      glm::vec3 scale = selected_entity_->getScale();
      ImGui::InputFloat3("Position", glm::value_ptr(position));
      ImGui::InputFloat3("Rotation", glm::value_ptr(rotation));
      ImGui::InputFloat3("Scale", glm::value_ptr(scale));

      Material *mat = selected_entity_->getMaterial();

      if (mat) {
        ImGui::Text("Material : %s", mat->getName().c_str());
        Texture *diffuse = mat->getDiffuse();
        if (diffuse) {
          ImGui::Text("Diffuse : %s", diffuse->getName().c_str());
        }
      }
    }
    ImGui::End();

    ImGui::Begin("Assets");

    if (ImGui::CollapsingHeader("meshes")) {
      for (const auto &mesh : Assets::get().meshes) {
        ImGui::Text("%s", mesh->getName().c_str());
      }
    }

    if (ImGui::CollapsingHeader("materials")) {
      for (const auto &material : Assets::get().materials) {
        ImGui::Text("%s", material->getName().c_str());
      }
    }

    ImGui::End();

    // ImGuiWindowFlags flags =
    //     ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoDocking |
    //     ImGuiWindowFlags_AlwaysAutoResize |
    //     ImGuiWindowFlags_NoSavedSettings |
    //     ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav;
    // ImGui::SetNextWindowBgAlpha(0.35f);  // Transparent background
    // if (ImGui::Begin("Example: Simple overlay", NULL, flags)) {
    //   ImGui::Text("ifps : %f s", ifps_);
    // }
    // ImGui::End();

    // Scene graph
    ImGui::Begin("Scene");

    showEntities(scene_.getEntities());

    ImGui::End();

    ImGui::End();  // end of dockspace

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

void MyWindow::scrollCallback(GLFWwindow * /*window*/, double /*xoffset*/,
                              double yoffset) {
  if (yoffset > 0) {
    camera_.forward();
  }

  if (yoffset < 0) {
    camera_.backward();
  }
}

void MyWindow::showEntities(const std::vector<Entity *> &entities) {
  for (const auto &entity : entities) {
    int tree_flags = ImGuiTreeNodeFlags_OpenOnArrow;

    if (entity->getID() == selected_id_) {
      tree_flags |= ImGuiTreeNodeFlags_Selected;
    }

    if (entity->getChildren().size() == 0) {
      tree_flags |= ImGuiTreeNodeFlags_Leaf;
    }

    bool node_open = ImGui::TreeNodeEx(entity->getName().c_str(), tree_flags);
    if (ImGui::IsItemClicked()) {
      selected_id_ = entity->getID();
      selected_entity_ = entity;
    }
    if (node_open) {
      showEntities(entity->getChildren());

      ImGui::TreePop();
    }
  }
}

Entity *MyWindow::createGrid(int width, int height) {
  // GRID
  Texture *texture = new Texture;
  texture->loadFromImage("models/grid.png", TextureType::DIFFUSE);
  // texture.loadFromImage("models/debug.jpg", "texture_diffuse");

  Mesh *mesh = new Mesh;
  Material *material = new Material;
  material->create(&shader_, texture, "grid_material");

  std::vector<Vertex> vertices;

  for (int y = 0; y < height; y++) {
    for (int x = 0; x < width; x++) {
      Vertex v1, v2, v3, v4, v5, v6;

      int test_x = x - width / 2;
      int test_y = y - width / 2;
      v1.position = {test_x + 1, test_y, 0};
      v2.position = {test_x, test_y + 1, 0};
      v3.position = {test_x, test_y, 0};
      v1.normal = {0, 0, 1};
      v2.normal = {0, 0, 1};
      v3.normal = {0, 0, 1};
      v1.tex_coords = {1, 1};
      v2.tex_coords = {0, 0};
      v3.tex_coords = {0, 1};

      v4.position = {test_x + 1, test_y, 0};
      v5.position = {test_x + 1, test_y + 1, 0};
      v6.position = {test_x, test_y + 1, 0};
      v4.normal = {0, 0, 1};
      v5.normal = {0, 0, 1};
      v6.normal = {0, 0, 1};
      v4.tex_coords = {1, 1};
      v5.tex_coords = {1, 0};
      v6.tex_coords = {0, 0};

      vertices.push_back(v1);
      vertices.push_back(v2);
      vertices.push_back(v3);
      vertices.push_back(v4);
      vertices.push_back(v5);
      vertices.push_back(v6);
    }
  }

  mesh->create(vertices, "grid_mesh");
  Entity *grid = new Entity;
  grid->create(mesh, material, "grid");
  return grid;
}

Entity *MyWindow::createCube(int x, int y, int z) {
  std::vector<Vertex> vertices;

  Texture *texture = new Texture;
  // texture->loadFromImage("models/grid.png", TextureType::DIFFUSE);
  texture->loadFromImage("models/debug.jpg", TextureType::DIFFUSE);

  Mesh *mesh = new Mesh;

  // Front
  vertices.push_back({{1, -1, 0}, {0, 0, 1}, {1, 1}});
  vertices.push_back({{-1, 1, 0}, {0, 0, 1}, {0, 0}});
  vertices.push_back({{-1, -1, 0}, {0, 0, 1}, {0, 1}});
  vertices.push_back({{1, -1, 0}, {0, 0, 1}, {1, 1}});
  vertices.push_back({{1, 1, 0}, {0, 0, 1}, {1, 0}});
  vertices.push_back({{-1, 1, 0}, {0, 0, 1}, {0, 0}});

  // Top
  vertices.push_back({{1, 1, 0}, {0, 1, 0}, {1, 1}});
  vertices.push_back({{-1, 1, -2}, {0, 1, 0}, {0, 0}});
  vertices.push_back({{-1, 1, 0}, {0, 1, 0}, {0, 1}});
  vertices.push_back({{1, 1, 0}, {0, 1, 0}, {1, 1}});
  vertices.push_back({{1, 1, -2}, {0, 1, 0}, {1, 0}});
  vertices.push_back({{-1, 1, -2}, {0, 1, 0}, {0, 0}});

  // Bottom
  vertices.push_back({{1, -1, -2}, {0, -1, 0}, {1, 1}});
  vertices.push_back({{-1, -1, 0}, {0, -1, 0}, {0, 0}});
  vertices.push_back({{-1, -1, -2}, {0, -1, 0}, {0, 1}});
  vertices.push_back({{1, -1, -2}, {0, -1, 0}, {1, 1}});
  vertices.push_back({{1, -1, 0}, {0, -1, 0}, {1, 0}});
  vertices.push_back({{-1, -1, 0}, {0, -1, 0}, {0, 0}});

  // Back
  vertices.push_back({{-1, -1, -2}, {0, 0, -1}, {1, 1}});
  vertices.push_back({{1, 1, -2}, {0, 0, -1}, {0, 0}});
  vertices.push_back({{1, -1, -2}, {0, 0, -1}, {0, 1}});
  vertices.push_back({{-1, -1, -2}, {0, 0, -1}, {1, 1}});
  vertices.push_back({{-1, 1, -2}, {0, 0, -1}, {1, 0}});
  vertices.push_back({{1, 1, -2}, {0, 0, -1}, {0, 0}});

  // Left
  vertices.push_back({{-1, -1, 0}, {-1, 0, 0}, {1, 1}});
  vertices.push_back({{-1, 1, -2}, {-1, 0, 0}, {0, 0}});
  vertices.push_back({{-1, -1, -2}, {-1, 0, 0}, {0, 1}});
  vertices.push_back({{-1, -1, 0}, {-1, 0, 0}, {1, 1}});
  vertices.push_back({{-1, 1, 0}, {-1, 0, 0}, {1, 0}});
  vertices.push_back({{-1, 1, -2}, {-1, 0, 0}, {0, 0}});

  // Right
  vertices.push_back({{1, -1, -2}, {1, 0, 0}, {1, 1}});
  vertices.push_back({{1, 1, 0}, {1, 0, 0}, {0, 0}});
  vertices.push_back({{1, -1, 0}, {1, 0, 0}, {0, 1}});
  vertices.push_back({{1, -1, -2}, {1, 0, 0}, {1, 1}});
  vertices.push_back({{1, 1, -2}, {1, 0, 0}, {1, 0}});
  vertices.push_back({{1, 1, 0}, {1, 0, 0}, {0, 0}});

  mesh->create(vertices, "cube_mesh");
  Material *material = new Material;
  material->create(&shader_, texture, "cube_material");

  Entity *cube = new Entity;
  cube->create(mesh, material, "cube");
  return cube;
}