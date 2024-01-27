#pragma once

#include "glm/glm.hpp"

#include <string>

namespace RenderSystem
{
	struct GoldMaterial
	{
		static inline constexpr glm::vec3 AMBIENT = glm::vec3(0.24725f, 0.1995f, 0.0745f);
		static inline constexpr glm::vec3 DIFFUSE = glm::vec3(0.75164f, 0.60648f, 0.22648f);
		static inline constexpr glm::vec3 SPECULAR = glm::vec3(0.628281f, 0.555802f, 0.366065f);
		static inline constexpr float SHININESS = 2.0f;
	};

	struct DefaultLight
	{
		static inline constexpr glm::vec3 AMBIENT = glm::vec3(0.7f, 0.7f, 0.7f);
		static inline constexpr glm::vec3 DIFFUSE = glm::vec3(1.0f, 1.0f, 1.0f);
		static inline constexpr glm::vec3 SPECULAR = glm::vec3(0.1f, 0.1f, 0.1f);
	};

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
	constexpr int CAMERA_DIST_TO_BBOX_KOEF = 4;

	inline const std::string WINDOW_TITLE = "Mesh Editor";
}
