#pragma once

#include "glm/glm.hpp"

namespace RenderSystem
{
	const glm::vec4 BACKGROUND_COLOR = glm::vec4(0.09f, 0.42f, 0.69f, 1.0f);
	const glm::vec3 CAMERA_POSITION = glm::vec3(0.0f, 0.0f, 5.0f);
	const glm::vec3 CAMERA_TARGET = glm::vec3(0.0f, 0.0f, 0.0f);
	const glm::vec3 CAMERA_UP = glm::vec3(0.0f, 1.0f, 0.0f);
	const glm::vec3 CAMERA_RIGHT = glm::vec3(1.0f, 0.0f, 0.0f);
	const glm::vec2 VIEWPORT_POSITION = glm::vec2(0.0f, 0.0f);
	const glm::vec3 OBJECT_COLOR = glm::vec3(0.5f, 0.25f, 0.1f);
	const glm::vec3 LIGHT_COLOR = glm::vec3(1.0f, 1.0f, 1.0f);

	constexpr float FOV = 45.0f;
	constexpr float NEAR_PLANE_DISTANCE = 0.01f;
	constexpr float FAR_PLANE_DISTANCE = 1000.0f;
	constexpr float AMBIENT_STRENGTH = 0.1f;
	constexpr float SPECULAR_STRENGTH = 0.5f;
	constexpr float LIGHT_SOURCE_TO_CAMERA_DISTANCE = 25.0f;
	constexpr float ZOOM_STEP_KOEF = 0.1f;
	constexpr float ORBIT_SPEED_KOEF = 3.0f;
	constexpr int CAMERA_DIST_TO_BBOX_KOEF = 4;
	constexpr int LIGHT_SHININESS = 32;
}
