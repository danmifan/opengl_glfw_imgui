workspace "modern-opengl"
  configurations { "Debug", "Release" }

  buildoptions {"--std=c++14", "-Wall", "-Wextra"}

  filter "configurations:Debug"
    defines { "DEBUG" }
    symbols "On"

  filter "configurations:Release"
    defines { "NDEBUG" }
    optimize "On"


project "imgui"
  kind "SharedLib"
  language "C++"
  targetdir "lib/"

  includedirs {
    "thirdparty/imgui"
  }

  links {
    "glfw"
  }

  files {
    "thirdparty/imgui/*.cpp",
    "thirdparty/imgui/backends/imgui_impl_glfw.cpp",
    "thirdparty/imgui/backends/imgui_impl_opengl3.cpp",
  }


project "window"
  kind "ConsoleApp"
  language "C++"
  targetdir "bin/"

  includedirs {
    "include",
    "thirdparty/imgui",
    "thirdparty/imgui/backends"
  }

  libdirs {
    "lib",
  }

  links {
    "imgui",
    "glfw",
    "GL",
    "GLEW",
    "assimp"
  }

  files {
    "src/*.cpp"
  }