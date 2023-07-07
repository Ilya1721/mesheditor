module;
#include "GeometryCore/Vector.h"
export module RenderSystemConsts;

export namespace RenderSystem
{
	const Geometry::Vector4D BACKGROUND_COLOR = Geometry::Vector4D(0.09f, 0.42f, 0.69f, 1.0f);
	const Geometry::Vector4D CAMERA_POSITION = Geometry::Vector3D(0.0f, 0.0f, 5.0f);
	const Geometry::Vector3D CAMERA_TARGET = Geometry::Vector3D(0.0f, 0.0f, 0.0f);
	const Geometry::Vector3D CAMERA_UP = Geometry::Vector3D(0.0f, 1.0f, 0.0f);
	const Geometry::Vector3D CAMERA_RIGHT = Geometry::Vector3D(1.0f, 0.0f, 0.0f);
	const Geometry::Vector2D VIEWPORT_POSITION = Geometry::Vector2D(0.0f, 0.0f);
	const Geometry::Vector3D DEFAULT_OBJECT_COLOR = Geometry::Vector3D(0.5f, 0.25f, 0.1f);
	const Geometry::Vector3D LIGHT_COLOR = Geometry::Vector3D(1.0f, 1.0f, 1.0f);

	constexpr float FOV = 45.0f;
	constexpr float NEAR_PLANE_DISTANCE = 0.01f;
	constexpr float FAR_PLANE_DISTANCE = 1000.0f;
	constexpr float AMBIENT_STRENGTH = 0.1f;
	constexpr float SPECULAR_STRENGTH = 0.5f;
	constexpr float LIGHT_TO_CAMERA_DISTANCE = 25.0f;
	constexpr float ZOOM_STEP_KOEF = 0.1f;
	constexpr int CAMERA_DIST_TO_BBOX_KOEF = 4;
	constexpr int LIGHT_SHININESS = 32;
}
