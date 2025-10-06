#pragma once

#include <array>
#include <filesystem>
#include <glm/glm.hpp>
#include <string>

#include "GeometryCore/Typedefs.h"
#include "LightParams.h"
#include "MeshCore/MaterialParams.h"
#include "MeshCore/Typedefs.h"

using namespace GeometryCore;

namespace RenderSystem
{
  const MaterialParams RUBY_MATERIAL {
    RGB(0.1745f, 0.01175f, 0.01175f), RGB(0.61424f, 0.04136f, 0.04136f),
    RGB(0.727811f, 0.626959f, 0.626959f), 2.0f
  };

  const MaterialParams EMERALD_MATERIAL {
    RGB(0.0215f, 0.1745f, 0.0215f), RGB(0.07568f, 0.61424f, 0.07568f),
    RGB(0.633f, 0.727811f, 0.633f), 0.6f
  };

  const MaterialParams RED_MATERIAL {
    RGB(1.0f, 0.0f, 0.0f), RGB(0.0f, 0.0f, 0.0f), RGB(0.0f, 0.0f, 0.0f), 0.6f
  };

  const MaterialParams GREEN_MATERIAL {
    RGB(0.0f, 1.0f, 0.0f), RGB(0.0f, 0.0f, 0.0f), RGB(0.0f, 0.0f, 0.0f), 0.6f
  };

  const MaterialParams BLUE_MATERIAL {
    RGB(0.0f, 0.0f, 1.0f), RGB(0.0f, 0.0f, 0.0f), RGB(0.0f, 0.0f, 0.0f), 0.6f
  };

  const MaterialParams BLACK_MATERIAL {
    RGB(0.0f, 0.0f, 0.0f), RGB(0.0f, 0.0f, 0.0f), RGB(0.0f, 0.0f, 0.0f), 0.6f
  };

  const MaterialParams PEARL_MATERIAL {
    RGB(0.25f, 0.20725f, 0.20725f), RGB(1.0f, 0.829f, 0.829f),
    RGB(0.296648f, 0.296648f, 0.296648f), 11.264f
  };

  static inline constexpr DirectionalLightParams DIRECTIONAL_LIGHT_PARAMS {
    RGB(1.0f, 1.0f, 1.0f), RGB(1.0f, 1.0f, 1.0f), RGB(0.1f, 0.1f, 0.1f)
  };

  static inline constexpr PointLightParams POINT_LIGHT_PARAMS {RGB(1.0f, 1.0f, 1.0f),
                                                               RGB(1.0f, 1.0f, 1.0f),
                                                               RGB(0.1f, 0.1f, 0.1f),
                                                               1.0f,
                                                               0.009f,
                                                               0.003f};

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
  constexpr float LIGHT_SOURCE_POS_X = FAR_PLANE_DISTANCE * -0.3f;
  constexpr float LIGHT_SOURCE_POS_Y = FAR_PLANE_DISTANCE * 0.4f;
  constexpr float LIGHT_SOURCE_POS_Z = FAR_PLANE_DISTANCE * 0.6f;
  constexpr float ARCBALL_SENSITIVITY = 0.01f;
  constexpr float SHADOW_BIAS = 0.00025f;
  constexpr float POINT_LIGHT_BBOX_Y_COEF = 0.2f;
  constexpr float POINT_LIGHT_RADIUS = 1.0f;
  inline const std::string WINDOW_TITLE = "Mesh Editor";

  constexpr bool DEBUG_RENDER = true;

  const std::string DEPTH_MAP_VERTEX_SHADER_PATH =
    R"(./RenderSystem/Shaders/DepthMapShader.vert)";
  const std::string DEPTH_MAP_FRAGMENT_SHADER_PATH =
    R"(./RenderSystem/Shaders/DepthMapShader.frag)";
  const std::string SCENE_VERTEX_SHADER_PATH =
    R"(./RenderSystem/Shaders/VertexShader.vert)";
  const std::string SCENE_FRAGMENT_SHADER_PATH =
    R"(./RenderSystem/Shaders/FragmentShader.frag)";
  const std::filesystem::path SKYBOX_VERTEX_SHADER_PATH =
    R"(./RenderSystem/Shaders/SkyboxShader.vert)";
  const std::filesystem::path SKYBOX_FRAGMENT_SHADER_PATH =
    R"(./RenderSystem/Shaders/SkyboxShader.frag)";
  const std::array<std::filesystem::path, 6> SKYBOX_CUBEMAP_TEXTURES = {
    R"(./ThirdParty/resources/textures/skybox/right.png)",
    R"(./ThirdParty/resources/textures/skybox/left.png)",
    R"(./ThirdParty/resources/textures/skybox/top.png)",
    R"(./ThirdParty/resources/textures/skybox/bottom.png)",
    R"(./ThirdParty/resources/textures/skybox/front.png)",
    R"(./ThirdParty/resources/textures/skybox/back.png)",
  };

  constexpr int DEPTH_TEXTURE_SLOT = 0;
  constexpr int DIFFUSE_TEXTURE_SLOT = 1;
  constexpr int SKYBOX_TEXTURE_SLOT = 0;
}  // namespace RenderSystem
