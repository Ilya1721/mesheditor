#pragma once

#include <glm/glm.hpp>
#include <string>

#include "GeometryCore/Typedefs.h"

#include "Material.h"
#include "Light.h"
#include "Typedefs.h"

using namespace GeometryCore;

namespace RenderSystem
{
	static inline constexpr Material GOLD_MATERIAL{
		RGB(0.24725f, 0.1995f, 0.0745f),
		RGB(0.75164f, 0.60648f, 0.22648f),
		RGB(0.628281f, 0.555802f, 0.366065f),
		2.0f
	};

	static inline constexpr Material RUBY_MATERIAL{
		RGB(0.1745f, 0.01175f, 0.01175f),
		RGB(0.61424f, 0.04136f, 0.04136f),
		RGB(0.727811f, 0.626959f, 0.626959f),
		2.0f
	};

	static inline constexpr Material EMERALD_MATERIAL{
		RGB(0.0215f, 0.1745f, 0.0215f),
		RGB(0.07568f, 0.61424f, 0.07568f),
		RGB(0.633f, 0.727811f, 0.633f),
		0.6f
	};

	static inline constexpr Material RED_MATERIAL{
		RGB(1.0f, 0.0f, 0.0f),
		RGB(0.0f, 0.0f, 0.0f),
		RGB(0.0f, 0.0f, 0.0f),
		0.6f
	};

	static inline constexpr Material GREEN_MATERIAL{
		RGB(0.0f, 1.0f, 0.0f),
		RGB(0.0f, 0.0f, 0.0f),
		RGB(0.0f, 0.0f, 0.0f),
		0.6f
	};

	static inline constexpr Material BLUE_MATERIAL{
		RGB(0.0f, 0.0f, 1.0f),
		RGB(0.0f, 0.0f, 0.0f),
		RGB(0.0f, 0.0f, 0.0f),
		0.6f
	};

	static inline constexpr Material BLACK_MATERIAL{
		RGB(0.0f, 0.0f, 0.0f),
		RGB(0.0f, 0.0f, 0.0f),
		RGB(0.0f, 0.0f, 0.0f),
		0.6f
	};

	static inline constexpr Material PEARL_MATERIAL{
		RGB(0.25f, 0.20725f, 0.20725f),
		RGB(1.0f, 0.829f, 0.829f),
		RGB(0.296648f, 0.296648f, 0.296648f),
		11.264f
	};

	static inline constexpr Light DEFAULT_LIGHT{
		RGB(1.0f, 1.0f, 1.0f),
		RGB(1.0f, 1.0f, 1.0f),
		RGB(0.1f, 0.1f, 0.1f)
	};

	constexpr RGBA BACKGROUND_COLOR = RGBA(0.725f, 0.9f, 1.0f, 1.0f);
	constexpr Point3D DEFAULT_CAMERA_POSITION = Point3D(0.0f, 0.0f, 5.0f);
	constexpr Point3D DEFAULT_CAMERA_TARGET = Point3D(0.0f, 0.0f, 0.0f);
	constexpr Vector3D DEFAULT_CAMERA_UP = Vector3D(0.0f, 1.0f, 0.0f);
	constexpr Vector3D DEFAULT_CAMERA_RIGHT = Vector3D(1.0f, 0.0f, 0.0f);
	constexpr Point2D VIEWPORT_POSITION = Point2D(0.0f, 0.0f);

	constexpr float FOV = 45.0f;
	constexpr float NEAR_PLANE_DISTANCE = 0.01f;
	constexpr float FAR_PLANE_DISTANCE = 1000.0f;
	constexpr float LIGHT_SOURCE_TO_CAMERA_DISTANCE = 25.0f;
	constexpr float ZOOM_STEP_COEF = 0.5f;
	constexpr float ORBIT_SPEED_COEF = 4.0f;
	constexpr float SURFACE_EXTRUCTION_SPEED_COEF = 1.0f;
	constexpr float CAMERA_DIST_TO_BBOX_COEF = 3.75f;
	constexpr float DEFAULT_Z_VALUE = 1.0f;
	constexpr float BBOX_VIEWPORT_GAP_COEF = 2.0f;
	constexpr float ORTHO_ZOOM_STEP = 0.25f;
	constexpr float FLOOR_BBOX_HEIGHT_COEF = 1.0f;
	inline const std::string WINDOW_TITLE = "Mesh Editor";

	constexpr bool DEBUG_RENDER = true;
}
