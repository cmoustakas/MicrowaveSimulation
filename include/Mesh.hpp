#pragma once

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

struct Model {
  std::vector<Mesh> m_mesh;
  std::vector<Texture> m_texture;
};

} // namespace model
} // namespace simulator
