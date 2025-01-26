#include "GraphicsUtils.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include <glm/gtc/matrix_transform.hpp>

#include <fstream>
#include <iostream>
#include <sstream>

#include <WindowHandler.hpp>

namespace simulator {
namespace graphics_utils {

static void bindVAO(GLuint &vertex_array_id) {
  glGenVertexArrays(1, &vertex_array_id);
  glBindVertexArray(vertex_array_id);
  assert(vertex_array_id != 0);
}

template <typename T>
void bindVBO(GLuint &vertex_buffer_id, std::vector<T> &buffer) {
  glGenBuffers(1, &vertex_buffer_id);
  glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_id);
  glBufferData(GL_ARRAY_BUFFER, buffer.size() * sizeof(T), &buffer[0],
               GL_STATIC_DRAW);
}

static GraphicsRes compileShader(GLuint &shader_id, const char *path) {
  // Read the Vertex Shader code from the file
  std::string shader_code;
  std::ifstream shader_stream(path, std::ios::in);
  if (!shader_stream.is_open()) {
    return GraphicsRes::FAIL;
  }
  std::stringstream sstr;
  sstr << shader_stream.rdbuf();
  shader_code = sstr.str();
  shader_stream.close();

  GLint result = GL_FALSE;
  int info_log_len;

  // Compile Shader
  std::cout << "Compiling shader : " << path << " \n";
  char const *shader_ptr = shader_code.c_str();
  glShaderSource(shader_id, 1, &shader_ptr, nullptr);
  glCompileShader(shader_id);

  glGetShaderiv(shader_id, GL_COMPILE_STATUS, &result);
  glGetShaderiv(shader_id, GL_INFO_LOG_LENGTH, &info_log_len);

  if (info_log_len > 0) {
    std::vector<char> error_message(info_log_len + 1);
    glGetShaderInfoLog(shader_id, info_log_len, nullptr, &error_message[0]);
    std::cout << &error_message[0];
    return GraphicsRes::FAIL;
  }

  return GraphicsRes::SUCCESS;
}

GraphicsRes loadTextureFromImage(std::string file_name, GLuint &texture_id) {

  size_t position = file_name.find_last_of("\\");
  file_name = file_name.substr(position + 1);

  int width, height, num_channels;
  unsigned char *image_data =
      stbi_load(file_name.c_str(), &width, &height, &num_channels, 0);

  if (!image_data) {
    return GraphicsRes::FAIL;
  }

  glGenTextures(1, &texture_id);

  GLenum format{};

  if (num_channels == 1) {
    format = GL_RED;
  } else if (num_channels == 3) {
    format = GL_RGB;
  } else if (num_channels == 4) {
    format = GL_RGBA;
  }

  glBindTexture(GL_TEXTURE_2D, texture_id);
  glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

  glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format,
               GL_UNSIGNED_BYTE, image_data);
  glGenerateMipmap(GL_TEXTURE_2D);

  glTexParameteri(
      GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,
      GL_MIRRORED_REPEAT); // GL_REPEAT... GL_MIRRORED_REPEAT...
                           // GL_CLAMP_TO_EDGE... GL_CLAMP_TO_BORDER.
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                  GL_LINEAR_MIPMAP_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,
                  GL_LINEAR); // GL_NEAREST or GL_LINEAR.

  stbi_image_free(image_data);

  return GraphicsRes::SUCCESS;
}

GraphicsRes loadShaders(const char *fragment_shader_path,
                        const char *vertex_shader_path, GLuint &program_id) {

  GLuint vertex_shader_id = glCreateShader(GL_VERTEX_SHADER);
  GLuint fragment_shader_id = glCreateShader(GL_FRAGMENT_SHADER);

  if (compileShader(vertex_shader_id, vertex_shader_path) ==
          GraphicsRes::FAIL ||
      compileShader(fragment_shader_id, fragment_shader_path) ==
          GraphicsRes::FAIL) {
    std::cerr << "Couldn't compile Shaders\n";
    return GraphicsRes::FAIL;
  }

  // Linking vertex shader and fragment shader to copmlete the pipeline
  program_id = glCreateProgram();
  glAttachShader(program_id, vertex_shader_id);
  glAttachShader(program_id, fragment_shader_id);
  glLinkProgram(program_id);

  GLint Result;
  int info_log_len;

  // Check the program
  glGetProgramiv(program_id, GL_LINK_STATUS, &Result);
  glGetProgramiv(program_id, GL_INFO_LOG_LENGTH, &info_log_len);
  if (info_log_len > 0) {
    std::vector<char> error_message(info_log_len + 1);
    glGetProgramInfoLog(program_id, info_log_len, nullptr, &error_message[0]);
    std::cerr << &error_message[0] << "\n";
    return GraphicsRes::FAIL;
  }

  glDetachShader(program_id, vertex_shader_id);
  glDetachShader(program_id, fragment_shader_id);

  glDeleteShader(vertex_shader_id);
  glDeleteShader(fragment_shader_id);

  return GraphicsRes::SUCCESS;
}

void bindToGPU(model::Model &current_model) {
  auto &mesh_vec = current_model.getMeshVec();

  for (size_t index = 0; index < mesh_vec.size(); ++index) {
    bindVAO(mesh_vec[index].m_VAO);

    bindVBO(mesh_vec[index].m_VBO_pos, mesh_vec[index].m_vert_positions);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float),
                          (void *)0);

    bindVBO(mesh_vec[index].m_VBO_norm, mesh_vec[index].m_vert_normals);

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float),
                          (void *)0);

    bindVBO(mesh_vec[index].m_VBO_tex, mesh_vec[index].m_tex_coords);

    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float),
                          (void *)0);

    glGenBuffers(1, &mesh_vec[index].m_EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh_vec[index].m_EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                 sizeof(GLuint) * mesh_vec[index].m_vert_indices.size(),
                 mesh_vec[index].m_vert_indices.data(), GL_STATIC_DRAW);

    glBindVertexArray(0);
  }
}

GraphicsRes render(model::Model &model, GLuint view_id, glm::mat4 view_matrix) {

  view_matrix = glm::translate(view_matrix, model.getPosition());
  glUniformMatrix4fv(view_id, 1, GL_FALSE, &view_matrix[0][0]);
  auto &mesh_vec = model.getMeshVec();

  for (size_t i = 0; i < mesh_vec.size(); ++i) {

    glBindTexture(
        GL_TEXTURE_2D,
        mesh_vec[i].m_tex_handle); // Bind texture for the current mesh.

    glBindVertexArray(mesh_vec[i].m_VAO);

    glDrawElements(GL_TRIANGLES, (GLsizei)mesh_vec[i].m_vert_indices.size(),
                   GL_UNSIGNED_INT, 0);

    //    glBindVertexArray(0);
  }

  return GraphicsRes::SUCCESS;
}

void debugOpenGL() {
  GLenum error;
  while ((error = glGetError()) != GL_NO_ERROR) {
    std::cerr << "OpenGL Error: " << error << std::endl;
  }
}

void updateOnEvents(CameraHandler &camera, glm::vec3 *model_pos) {

  constexpr float dtheta_quant = 0.1f;
  constexpr float dfi_quant = 0.1f;

  float theta = 0.f;
  float fi = 0.f;
  const auto window = WindowHandler::getInstance().getWindow();
  if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
    theta += dtheta_quant;
  } else if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
    theta -= dtheta_quant;
  }

  if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
    fi += dfi_quant;
  } else if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
    fi -= dfi_quant;
  }

  camera.updateCamera(theta, fi, model_pos);
}

} // namespace graphics_utils
} // namespace simulator
