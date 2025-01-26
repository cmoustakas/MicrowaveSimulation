#pragma once
#include <GL/glew.h>

#include <GL/gl.h>

#include <string>

#include <CameraHandler.hpp>
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
GraphicsRes loadTextureFromImage(std::string file_name, GLuint &texture_id);

/**
 * @brief load_shaders
 * @param fragment_shader_path
 * @param vertex_shader_path
 * @return
 */
GraphicsRes loadShaders(const char *fragment_shader_path,
                        const char *vertex_shader_path, GLuint &program_id);

/**
 * @brief bind_to_GPU
 * @param model
 * @return
 */
void bindToGPU(model::Model &model);

/**
 * @brief render
 * @param model
 * @return
 */
GraphicsRes render(model::Model &model, GLuint view_id, glm::mat4 view_matrix);

/**
 * @brief debugOpenGL
 * @return
 */
void debugOpenGL();

/**
 * @brief updateOnEvents
 * @param camera
 * @param model_pos
 */
void updateOnEvents(CameraHandler &camera, glm::vec3 *model_pos = nullptr);
} // namespace graphics_utils
} // namespace simulator
