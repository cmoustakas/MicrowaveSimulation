#include "GraphicsUtils.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include <fstream>
#include <iostream>
#include <sstream>

namespace simulator {
namespace graphics_utils {

static void bind_VAO(GLuint &vertex_array_ID) {
  glGenVertexArrays(1, &vertex_array_ID);
  glBindVertexArray(vertex_array_ID);
}

template <typename T>
void bind_VBO(GLuint &vertex_buffer_ID, std::vector<T> &buffer) {
  glGenBuffers(1, &vertex_buffer_ID);
  glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_ID);
  glBufferData(GL_ARRAY_BUFFER, buffer.size() * sizeof(T), &buffer[0],
               GL_STATIC_DRAW);
}

static GraphicsRes compile_shader(GLuint &shaderID, const char *path) {
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

  GLint Result = GL_FALSE;
  int info_log_len;

  // Compile Shader
  std::cout << "Compiling shader : " << path << " \n";
  char const *shader_ptr = shader_code.c_str();
  glShaderSource(shaderID, 1, &shader_ptr, NULL);
  glCompileShader(shaderID);

  glGetShaderiv(shaderID, GL_COMPILE_STATUS, &Result);
  glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &info_log_len);

  if (info_log_len > 0) {
    std::vector<char> error_message(info_log_len + 1);
    glGetShaderInfoLog(shaderID, info_log_len, NULL, &error_message[0]);
    std::cout << &error_message[0];
    return GraphicsRes::FAIL;
  }

  return GraphicsRes::SUCCESS;
}

GraphicsRes load_texture_from_image(std::string file_name, GLuint &texture_id) {

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

GraphicsRes load_shaders(const char *fragment_shader_path,
                         const char *vertex_shader_path, GLuint &program_id) {

  GLuint vertex_shader_Id = glCreateShader(GL_VERTEX_SHADER);
  GLuint fragment_shader_Id = glCreateShader(GL_FRAGMENT_SHADER);

  if (compile_shader(vertex_shader_Id, vertex_shader_path) ==
          GraphicsRes::FAIL ||
      compile_shader(fragment_shader_Id, fragment_shader_path) ==
          GraphicsRes::FAIL) {
    std::cerr << "Couldn't compile Shaders\n";
    return GraphicsRes::FAIL;
  }

  // Linking vertex shader and fragment shader to copmlete the pipeline
  program_id = glCreateProgram();
  glAttachShader(program_id, vertex_shader_Id);
  glAttachShader(program_id, fragment_shader_Id);
  glLinkProgram(program_id);

  GLint Result;
  int info_log_len;

  // Check the program
  glGetProgramiv(program_id, GL_LINK_STATUS, &Result);
  glGetProgramiv(program_id, GL_INFO_LOG_LENGTH, &info_log_len);
  if (info_log_len > 0) {
    std::vector<char> error_message(info_log_len + 1);
    glGetProgramInfoLog(program_id, info_log_len, NULL, &error_message[0]);
    std::cerr << &error_message[0] << "\n";
    return GraphicsRes::FAIL;
  }

  glDetachShader(program_id, vertex_shader_Id);
  glDetachShader(program_id, fragment_shader_Id);

  glDeleteShader(vertex_shader_Id);
  glDeleteShader(fragment_shader_Id);

  return GraphicsRes::SUCCESS;
}

void bind_to_GPU(model::Model &current_model) {
  for (size_t index = 0; index < current_model.m_mesh.size(); ++index) {
    bind_VAO(current_model.m_mesh[index].m_VAO);

    bind_VBO(current_model.m_mesh[index].m_VBO_pos,
             current_model.m_mesh[index].m_vert_positions);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float),
                          (void *)0);

    bind_VBO(current_model.m_mesh[index].m_VBO_norm,
             current_model.m_mesh[index].m_vert_normals);

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float),
                          (void *)0);

    bind_VBO(current_model.m_mesh[index].m_VBO_tex,
             current_model.m_mesh[index].m_tex_coords);

    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float),
                          (void *)0);

    glGenBuffers(1, &current_model.m_mesh[index].m_EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, current_model.m_mesh[index].m_EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                 sizeof(unsigned int) *
                     current_model.m_mesh[index].m_vert_indices.size(),
                 &current_model.m_mesh[index].m_vert_indices[0],
                 GL_STATIC_DRAW);

    glBindVertexArray(0);
  }
}

} // namespace graphics_utils
} // namespace simulator
