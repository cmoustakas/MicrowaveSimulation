#pragma once

#include <Mesh.hpp>
namespace simulator {
/**
 * @brief load_model
 * @param model_path
 * @param textures_path
 * @param model
 */
void load_model(const char *model_path, const char *textures_path,
                model::Model &model);
} // namespace simulator
