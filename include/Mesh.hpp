#pragma once

#include <GL/glew.h>

#include <GL/gl.h>

#include <glm/glm.hpp>
#include <string>
#include <vector>

namespace simulator {
namespace model {

struct Mesh {
  GLuint m_VAO;
  GLuint m_VBO_pos;
  GLuint m_VBO_norm;
  GLuint m_VBO_tex;
  GLuint m_EBO;
  std::vector<glm::vec3> m_vert_positions;
  std::vector<glm::vec3> m_vert_normals;
  std::vector<glm::vec2> m_tex_coords;
  std::vector<size_t> m_vert_indices;
  size_t m_tex_handle;
  std::string m_name;
};

struct Texture {
  GLuint m_texture_id;
  std::string m_image_name;
};

struct Material {
  float m_density = 0.f;                 ///< kg/m^3
  float m_thickness = 0.f;               ///< m
  float m_heat_capacity = 0.f;           ///< J/kg*K
  float m_electrical_conductivity = 0.f; ///< S/m
};

class Model {
public:
  Model() = default;
  Model(Material &material);
  ~Model() {
    for (auto &m : m_mesh) {
      glDeleteVertexArrays(1, &m.m_VAO);
      glDeleteBuffers(1, &m.m_VBO_pos);
      glDeleteBuffers(1, &m.m_VBO_norm);
      glDeleteBuffers(1, &m.m_VBO_tex);
      glDeleteBuffers(1, &m.m_EBO);
    }
  }

  void updatePosition(const glm::vec3 &new_position) {
    m_position = new_position;
  };

  void calculateTemperature(float timestamp);

  std::vector<Mesh> &getMeshVec() { return m_mesh; }
  std::vector<Texture> &getTextureVec() { return m_texture; }
  glm::vec3 &getPosition() { return m_position; }
  void setPosition(const glm::vec3 &position) { m_position = position; }

private:
  glm::vec3 m_position{};
  float m_temperature = 0.f; ///< K
  std::vector<Mesh> m_mesh;
  std::vector<Texture> m_texture;
  Material m_material;
};

} // namespace model
} // namespace simulator
