#include "Demo.hpp"

#include <filesystem>
#include <iostream>
#include <vector>

#include <Engine.hpp>
#include <GraphicsUtils.hpp>
#include <ModelImport.hpp>
#include <WindowHandler.hpp>

namespace fs = std::filesystem;
static void resolvePaths(fs::path &shaders, fs::path &models) {
  fs::path proj_path = fs::canonical(fs::current_path());
  fs::path candidate_path;
  do {
    proj_path = proj_path.parent_path();
    candidate_path = proj_path / "MicrowaveSimulation";

    if (proj_path == proj_path.root_path()) {
      throw std::runtime_error("Can't resolve the project path");
    }
  } while (!fs::exists(candidate_path));

  proj_path = candidate_path;
  assert(fs::exists(proj_path));
  assert(fs::is_directory(proj_path));

  models = proj_path;
  shaders = proj_path;

  models /= "models";
  assert(fs::exists(models));
  assert(fs::is_directory(models));

  shaders /= "src";
  shaders /= "shaders";
  assert(fs::exists(shaders));
  assert(fs::is_directory(shaders));

  return;
}

static simulator::EngineConfig configureEngine(fs::path &shaders_path) {
  simulator::EngineConfig engine_cfg;

  fs::path fragment_path = shaders_path / "fragment.glsl";
  fs::path vertex_path = shaders_path / "vertex.glsl";

  const auto checkShaders = [](fs::path &shader_path) {
    if (!fs::exists(shader_path) || fs::is_directory(shader_path)) {
      throw std::runtime_error("Shader path: unresolved");
    }
  };

  checkShaders(fragment_path);
  checkShaders(vertex_path);

  engine_cfg.m_source_position = glm::vec3(0.f, 0.f, 0.f);
  engine_cfg.m_vertex_shader_path = vertex_path.c_str();
  engine_cfg.m_fragment_shader_path = fragment_path.c_str();

  return engine_cfg;
}

void runSimulation() {
  simulator::WindowHandler::getInstance().initializeWindow();

  fs::path models_path;
  fs::path shaders_path;
  resolvePaths(shaders_path, models_path);

  simulator::EngineConfig engine_cfg = configureEngine(shaders_path);

  // Models are gonna be moved(&&) to the Engine
  fs::path model_path = models_path / "model.obj";
  fs::path texture_path = models_path / "";
  assert(fs::exists(model_path));

  constexpr int kTotalModels = 1;
  std::vector<simulator::model::Model> models;
  models.resize(kTotalModels);

  for (auto &m : models) {
    simulator::loadModel(model_path.c_str(), texture_path.c_str(), m);
    simulator::graphics_utils::bindToGPU(m);
  }

  simulator::Engine engine(engine_cfg, models);
  while (engine.run() == simulator::EngineState::RUNNING) {
  }
}
