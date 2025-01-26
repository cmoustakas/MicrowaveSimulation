#include "WindowHandler.hpp"

#include <GL/gl.h>

#include <iostream>

static constexpr int kWidth = 1280;
static constexpr int kHeight = 720;

namespace simulator {

WindowHandler::~WindowHandler() { glfwTerminate(); }

bool WindowHandler::initializeWindow() {
  if (!glfwInit()) {
    std::cerr << "Could not init OpenGL window\n";
    return false;
  }

  glfwWindowHint(GLFW_SAMPLES, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  // window = glfwCreateWindow(WIDTH,HEIGHT, "My OpenGL simulation ",
  // glfwGetPrimaryMonitor() , nullptr);
  m_window =
      glfwCreateWindow(kWidth, kHeight, "Microwave Heat Transmition simulator",
                       nullptr, nullptr);

  if (m_window == nullptr) {
    std::cerr << ("Failed to init Window\n");
    return false;
  }
  glfwMakeContextCurrent(m_window);

  // Init GLEW to specify Targets attributes
  glewExperimental = true; // Needed for core profile
  if (glewInit() != GLEW_OK) {
    std::cerr << "Failed to init Glew\n";
    glfwTerminate();
    return false;
  }

  glEnable(GL_DEBUG_OUTPUT);
  glDebugMessageCallback(
      [](GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length,
         const GLchar *message, const void *userParam) {
        std::cerr << "GL Debug: " << message << std::endl;
      },
      nullptr);

  // Just to make sure the programm can be interrupted by pressing Esc
  glfwSetInputMode(m_window, GLFW_STICKY_KEYS, GL_TRUE);
  // glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED); // Hide mouse
  glfwPollEvents();
  glfwSetCursorPos(m_window, kWidth >> 1, kHeight >> 1);
  glClearColor(0.1f, 0.1f, 0.1f, 0.0f);

  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LESS);
  glDisable(GL_CULL_FACE);

  return true;
}

} // namespace simulator
