#pragma once

#include <GL/glew.h>

#include <GL/gl.h>
#include <string>

#include <Mesh.hpp>

namespace simulator {
namespace graphics_utils {
enum GraphicsRes { FAIL = -1, SUCCESS = 1 };

/**
 * @brief load_texture_from_image
 * @param file_name
 * @param texture_id
 * @return
 */
GraphicsRes load_texture_from_image(std::string file_name, GLuint &texture_id);

/**
 * @brief load_shaders
 * @param fragment_shader_path
 * @param vertex_shader_path
 * @return
 */
GraphicsRes load_shaders(const char *fragment_shader_path,
                         const char *vertex_shader_path, GLuint &program_id);

/**
 * @brief bind_to_GPU
 * @param model
 * @return
 */
void bind_to_GPU(model::Model &model);

} // namespace graphics_utils
} // namespace simulator
