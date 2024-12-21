#pragma once

#include <GLFW/glfw3.h>

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

private:
  WindowHandler();
  ~WindowHandler();

  // Delete copy ctor and assignment
  WindowHandler(const WindowHandler &) = delete;
  WindowHandler &operator=(const WindowHandler &) = delete;

  /**
   * @brief initializeWindow
   * @return
   */
  bool initializeWindow();

  GLFWwindow *m_window = nullptr;
};
} // namespace simulator
