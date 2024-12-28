#pragma once
#include <GL/glew.h>

#include <GLFW/glfw3.h>

#include <CameraHandler.hpp>

namespace simulator {
class WindowHandler {

public:
  /**
   * @brief getInstance
   * @return
   */
  static WindowHandler &getInstance() {
    static WindowHandler instance;
    return instance;
  }

  GLFWwindow *getWindow() { return m_window; }

  /**
   * @brief initializeWindow
   * @return
   */
  bool initializeWindow();

private:
  WindowHandler() = default;
  ~WindowHandler();

  // Delete copy ctor and assignment
  WindowHandler(const WindowHandler &) = delete;
  WindowHandler &operator=(const WindowHandler &) = delete;

  GLFWwindow *m_window = nullptr;
};
} // namespace simulator
