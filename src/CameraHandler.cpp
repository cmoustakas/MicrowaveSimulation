#include "CameraHandler.hpp"

#include <glm/gtc/matrix_transform.hpp>

#include <algorithm>
#include <iostream>

static constexpr glm::vec3 kInitPosition = glm::vec3(20.f, 0.f, 0.f);
static constexpr glm::vec3 kInitDirection = glm::vec3(0.f, 0.f, 0.f);
static constexpr glm::vec3 kInitUpVec = glm::vec3(0.f, 1.f, 0.f);

namespace simulator {

static inline void updatePositionOnSphere(glm::vec3 &position,
                                          SphericalCoords &sphere) {
  position.x = sphere.m_ray * cos(sphere.m_theta) * cos(sphere.m_fi);
  position.y = sphere.m_ray * sin(sphere.m_fi);
  position.z = sphere.m_ray * sin(sphere.m_theta) * cos(sphere.m_fi);
}

CameraHandler::CameraHandler() { initializeCamera(); }

void CameraHandler::resetCamera() { initializeCamera(); }

void CameraHandler::initializeCamera() {
  m_position = kInitPosition;
  m_direction = kInitDirection;
  m_up = kInitUpVec;

  // Init my spherical coordinates
  m_sphere.m_ray = glm::length(m_position);
  m_sphere.m_fi = 0.f;
  m_sphere.m_theta = 0.f;

  m_camera_model = glm::lookAt(m_position, -m_direction, m_up);
}

void CameraHandler::updateCamera(float theta, float fi,
                                 const glm::vec3 *model_position) {
  // The deltas are supposed to control the thetas
  m_sphere.m_fi += fi;

  constexpr float low = -glm::pi<float>() / 2 + 0.1;
  constexpr float high = glm::pi<float>() / 2 - 0.1f;

  m_sphere.m_fi = std::clamp(m_sphere.m_fi, low, high);
  std::cout << m_sphere.m_fi << std::endl;

  m_sphere.m_theta += theta;

  // If camera needs to focus on a new model the pointer is supposed to be not
  // null
  if (model_position) {
    // Focus on model
    m_direction = -*model_position;

    // Update my position to "face" the model
    m_position = m_position + *model_position;
    // Update the new ray of the sphere that we inspect the new model
    m_sphere.m_ray = glm::length(m_position - *model_position);
  }

  updatePositionOnSphere(m_position, m_sphere);

  // Update camera model
  m_camera_model = glm::lookAt(m_position, m_direction, m_up);
}

} // namespace simulator
