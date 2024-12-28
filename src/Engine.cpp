#include "Engine.hpp"

#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

#include <GraphicsUtils.hpp>
#include <WindowHandler.hpp>

constexpr double kTimeInterval = 1e-3; ///< ms

namespace simulator {

Engine::Engine(const EngineConfig &cfg, std::vector<model::Model> &models)
    : m_engine_cfg(cfg) {

  m_models = models;
  const char *v_shader = m_engine_cfg.m_vertex_shader_path.c_str();
  const char *f_shader = m_engine_cfg.m_fragment_shader_path.c_str();

  if (graphics_utils::loadShaders(f_shader, v_shader,
                                  m_shader_cfg.m_program_id) ==
      graphics_utils::GraphicsRes::FAIL) {
    throw std::runtime_error("Throw on shader loading");
  }

  // Reusing the same texture unit for each model mesh.
  glActiveTexture(GL_TEXTURE0);
  m_shader_cfg.m_image_loc =
      glGetUniformLocation(m_shader_cfg.m_program_id, "image");
  glUniform1i(m_shader_cfg.m_image_loc, 0);

  // Bind my uniforms
  m_shader_cfg.m_matrix_id =
      glGetUniformLocation(m_shader_cfg.m_program_id, "model_mat");

  m_shader_cfg.m_view_id =
      glGetUniformLocation(m_shader_cfg.m_program_id, "view_mat");

  m_shader_cfg.m_projec_id =
      glGetUniformLocation(m_shader_cfg.m_program_id, "projection_mat");

  m_shader_cfg.m_rotation_id =
      glGetUniformLocation(m_shader_cfg.m_program_id, "rotation_mat");
}

EngineState Engine::run() {

  assert(!m_models.empty());
  glUseProgram(m_shader_cfg.m_program_id);

  //  glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  graphics_utils::updateOnEvents(m_camera, &m_models[0].getPosition());

  constexpr float fov = glm::radians(45.f);
  constexpr float aspect = 4.f / 3.f;
  constexpr float near = 0.1f;
  constexpr float far = 1e3;

  const glm::mat4 model_mat = glm::mat4(1.0f);
  const glm::mat4 projection_mat = glm::perspective(fov, aspect, near, far);
  glm::mat4 view_mat = m_camera.getCameraModel();
  const glm::mat4 rotation_matrix = glm::mat4(1.0f);

  // Upload the matrices to the GPU
  glUniformMatrix4fv(m_shader_cfg.m_matrix_id, 1, GL_FALSE, &model_mat[0][0]);
  glUniformMatrix4fv(m_shader_cfg.m_projec_id, 1, GL_FALSE,
                     &projection_mat[0][0]);
  glUniformMatrix4fv(m_shader_cfg.m_view_id, 1, GL_FALSE, &view_mat[0][0]);
  glUniformMatrix4fv(m_shader_cfg.m_rotation_id, 1, GL_FALSE,
                     &rotation_matrix[0][0]);

  // Render each model
  for (auto &m : m_models) {
    graphics_utils::render(m, m_shader_cfg.m_view_id, view_mat);
  }

  glfwSwapBuffers(WindowHandler::getInstance().getWindow());
  glfwPollEvents();

  EngineState state =
      (glfwGetKey(WindowHandler::getInstance().getWindow(), GLFW_KEY_ESCAPE) !=
           GLFW_PRESS &&
       glfwWindowShouldClose(WindowHandler::getInstance().getWindow()) == 0)
          ? (EngineState::RUNNING)
          : (EngineState::INTERRUPT);

  return state;
}

} // namespace simulator
