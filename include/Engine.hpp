#pragma once
#include <GL/glew.h>

#include <GL/gl.h>

#include <glm/glm.hpp>

#include <CameraHandler.hpp>
#include <Mesh.hpp>

static constexpr float kMHz = 1e6;
static constexpr float kGHz = 1e9;

namespace simulator {

struct ShaderAttr {
  GLuint m_program_id;
  GLuint m_matrix_id;
  GLuint m_view_id;
  GLuint m_projec_id;
  GLuint m_rotation_id;
  GLint m_image_loc;
};

enum EngineState { INTERRUPT, RUNNING };

struct EngineConfig {
  float m_source_freq = 300 * kMHz;
  glm::vec3 m_source_position{};
  std::string m_vertex_shader_path = "";
  std::string m_fragment_shader_path = "";
};

class Engine {
public:
  Engine(const EngineConfig &cfg, std::vector<model::Model> &models);
  ~Engine() { glDeleteProgram(m_shader_cfg.m_program_id); };
  /**
   * @brief run
   * @return
   */
  EngineState run();

private:
  std::vector<model::Model> m_models;
  CameraHandler m_camera;
  ShaderAttr m_shader_cfg;
  EngineConfig m_engine_cfg;
  double m_timeline = 0.f;
};

} // namespace simulator
