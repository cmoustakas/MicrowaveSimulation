#pragma once

#include <glm/glm.hpp>

namespace simulator {

struct SphericalCoords {
  float m_ray = 0.f;
  float m_theta = 0.f; ///< xy plane
  float m_fi = 0.f;    ///< z_axis
};

class CameraHandler {
public:
  CameraHandler();
  ~CameraHandler() = default;

  /**
   * @brief resetCamera
   */
  void resetCamera();

  /**
   * @brief updateCamera
   * @param delta_xy
   * @param delta_z
   * @param model_position
   */
  void updateCamera(float delta_xy, float delta_z,
                    const glm::vec3 *model_position = nullptr);
  /**
   * @brief getCameraModel
   * @return
   */
  glm::mat4 getCameraModel() const { return m_camera_model; }

private:
  /**
   * @brief intializeCamera
   */
  void initializeCamera();

  glm::vec3 m_position;
  glm::vec3 m_up;
  glm::vec3 m_direction;
  glm::mat4 m_camera_model;

  // The camera is moving in polar way and always aiming to the center
  SphericalCoords m_sphere;
};

} // namespace simulator
