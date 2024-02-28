#pragma once

#include "glm/glm.hpp"

#include <string>

#include "Material.h"
#include "Light.h"

namespace RenderSystem
{
	static inline constexpr Material GOLD_MATERIAL(
		glm::vec3(0.24725f, 0.1995f, 0.0745f),
		glm::vec3(0.75164f, 0.60648f, 0.22648f),
		glm::vec3(0.628281f, 0.555802f, 0.366065f),
		2.0f
	);

	static inline constexpr Material RUBY_MATERIAL(
		glm::vec3(0.1745f, 0.01175f, 0.01175f),
		glm::vec3(0.61424f, 0.04136f, 0.04136f),
		glm::vec3(0.727811f, 0.626959f, 0.626959f),
		2.0f
	);

	static inline constexpr Light DEFAULT_LIGHT(
		glm::vec3(0.7f, 0.7f, 0.7f),
		glm::vec3(1.0f, 1.0f, 1.0f),
		glm::vec3(0.1f, 0.1f, 0.1f)
	);

	constexpr glm::vec4 BACKGROUND_COLOR = glm::vec4(0.725f, 0.9f, 1.0f, 1.0f);
	constexpr glm::vec3 CAMERA_POSITION = glm::vec3(0.0f, 0.0f, 5.0f);
	constexpr glm::vec3 CAMERA_TARGET = glm::vec3(0.0f, 0.0f, 0.0f);
	constexpr glm::vec3 CAMERA_UP = glm::vec3(0.0f, 1.0f, 0.0f);
	constexpr glm::vec3 CAMERA_RIGHT = glm::vec3(1.0f, 0.0f, 0.0f);
	constexpr glm::vec2 VIEWPORT_POSITION = glm::vec2(0.0f, 0.0f);

	constexpr float FOV = 45.0f;
	constexpr float NEAR_PLANE_DISTANCE = 0.01f;
	constexpr float FAR_PLANE_DISTANCE = 1000.0f;
	constexpr float LIGHT_SOURCE_TO_CAMERA_DISTANCE = 25.0f;
	constexpr float ZOOM_STEP_KOEF = 0.5f;
	constexpr float ORBIT_SPEED_KOEF = 4.0f;
	constexpr float SURFACE_EXTRUCTION_SPEED_KOEF = 1.0f;
	constexpr int CAMERA_DIST_TO_BBOX_KOEF = 4;
	inline const std::string WINDOW_TITLE = "Mesh Editor";
}
