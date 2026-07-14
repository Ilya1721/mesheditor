#pragma once

#include <array>
#include <filesystem>
#include <string>
#include <unordered_set>

#include "LightParams.h"
#include "Material.h"

namespace RenderSystem
{
  inline const BlinnPhongMaterial RUBY_MATERIAL {
    glm::vec3(0.1745f, 0.01175f, 0.01175f), glm::vec3(0.61424f, 0.04136f, 0.04136f),
    glm::vec3(0.727811f, 0.626959f, 0.626959f), 2.0f
  };

  inline const BlinnPhongMaterial EMERALD_MATERIAL {
    glm::vec3(0.0215f, 0.1745f, 0.0215f), glm::vec3(0.07568f, 0.61424f, 0.07568f),
    glm::vec3(0.633f, 0.727811f, 0.633f), 0.6f
  };

  inline const BlinnPhongMaterial RED_MATERIAL {
    glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f),
    0.6f
  };

  inline const BlinnPhongMaterial GREEN_MATERIAL {
    glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f),
    0.6f
  };

  inline const BlinnPhongMaterial BLUE_MATERIAL {
    glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f),
    0.6f
  };

  inline const BlinnPhongMaterial BLACK_MATERIAL {
    glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f),
    0.6f
  };

  inline const BlinnPhongMaterial PEARL_MATERIAL {
    glm::vec3(0.25f, 0.20725f, 0.20725f), glm::vec3(1.0f, 0.829f, 0.829f),
    glm::vec3(0.296648f, 0.296648f, 0.296648f), 11.264f
  };

  inline const BlinnPhongMaterial GOLD_MATERIAL {
    glm::vec3(0.24725f, 0.1995f, 0.0745f), glm::vec3(0.75164f, 0.60648f, 0.22648f),
    glm::vec3(0.628281f, 0.555802f, 0.366065f), 2.0f
  };

  inline const GlassMaterial GLASS_MATERIAL {
    1.52f, 0.2f, 0.5f, 0.1f, glm::vec3(0.8f, 0.9f, 1.0f)
  };

  inline constexpr DirectionalLightParams DIR_LIGHT_PARAMS {
    glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(0.1f, 0.1f, 0.1f)
  };

  inline constexpr glm::vec3 LIGHT_COLOR(1.0f, 1.0f, 1.0f);

  inline constexpr PointLightParams POINT_LIGHT_PARAMS {
    glm::vec3(1.0f, 1.0f, 1.0f),
    glm::vec3(1.0f, 1.0f, 1.0f),
    glm::vec3(0.1f, 0.1f, 0.1f),
    5.0f,
    0.009f,
    0.003f
  };

  inline constexpr glm::vec4 BACKGROUND_COLOR = glm::vec4(0.725f, 0.9f, 1.0f, 1.0f);
  inline constexpr glm::vec3 DEFAULT_CAMERA_POSITION = glm::vec3(0.0f, 0.0f, 5.0f);
  inline constexpr glm::vec3 DEFAULT_CAMERA_TARGET = glm::vec3(0.0f, 0.0f, 0.0f);
  inline constexpr glm::vec3 DEFAULT_CAMERA_UP = glm::vec3(0.0f, 1.0f, 0.0f);
  inline constexpr glm::vec3 DEFAULT_CAMERA_RIGHT = glm::vec3(1.0f, 0.0f, 0.0f);
  inline constexpr glm::vec2 VIEWPORT_POSITION = glm::vec2(0.0f, 0.0f);

  inline constexpr float FOV = 45.0f;
  inline constexpr float NEAR_PLANE_DISTANCE = 0.01f;
  inline constexpr float FAR_PLANE_DISTANCE = 1000.0f;
  inline constexpr float LIGHT_SOURCE_TO_CAMERA_DISTANCE = 25.0f;
  inline constexpr float ZOOM_STEP_COEF = 0.5f;
  inline constexpr float ORBIT_SPEED_COEF = 4.0f;
  inline constexpr float SURFACE_EXTRUCTION_SPEED_COEF = 1.0f;
  inline constexpr float CAMERA_DIST_TO_BBOX_COEF = 3.75f;
  inline constexpr float DEFAULT_Z_VALUE = 1.0f;
  inline constexpr float ORTHO_ZOOM_STEP = 0.25f;
  inline constexpr float FLOOR_BBOX_HEIGHT_COEF = 1.0f;
  inline constexpr float LIGHT_SOURCE_POS_X = FAR_PLANE_DISTANCE * -0.3f;
  inline constexpr float LIGHT_SOURCE_POS_Y = FAR_PLANE_DISTANCE * 0.4f;
  inline constexpr float LIGHT_SOURCE_POS_Z = FAR_PLANE_DISTANCE * 0.6f;
  inline constexpr float ARCBALL_SENSITIVITY = 0.01f;
  inline constexpr float SHADOW_BIAS = 0.00025f;
  inline constexpr float POINT_LIGHT_BBOX_Y_COEF = 0.2f;
  inline constexpr float POINT_LIGHT_RADIUS = 1.0f;
  inline constexpr float ANISOTROPIC_FILTERING_LEVEL = 8.0f;
  inline constexpr int SHADOW_MAP_SIZE = 2048;
  inline const std::string WINDOW_TITLE = "Mesh Editor";

  inline constexpr bool DEBUG_RENDER = true;

  inline const std::string SHADOW_MAP_VERTEX_SHADER_PATH =
    R"(./RenderSystem/Shaders/ShadowMapShader.vert)";
  inline const std::string SHADOW_MAP_FRAGMENT_SHADER_PATH =
    R"(./RenderSystem/Shaders/ShadowMapShader.frag)";
  inline const std::string TAA_DEPTH_MAP_VERTEX_SHADER_PATH =
    R"(./RenderSystem/Shaders/TAADepthMapShader.vert)";
  inline const std::string TAA_DEPTH_MAP_FRAGMENT_SHADER_PATH =
    R"(./RenderSystem/Shaders/TAADepthMapShader.frag)";
  inline const std::string TAA_MOTION_VECTORS_VERTEX_SHADER_PATH =
    R"(./RenderSystem/Shaders/TAAMotionVectorsShader.vert)";
  inline const std::string TAA_MOTION_VECTORS_FRAGMENT_SHADER_PATH =
    R"(./RenderSystem/Shaders/TAAMotionVectorsShader.frag)";
  inline const std::string TAA_RESOLVE_VERTEX_SHADER_PATH =
    R"(./RenderSystem/Shaders/TAAResolveShader.vert)";
  inline const std::string TAA_RESOLVE_FRAGMENT_SHADER_PATH =
    R"(./RenderSystem/Shaders/TAAResolveShader.frag)";
  inline const std::string BLINN_PHONG_VERTEX_SHADER_PATH =
    R"(./RenderSystem/Shaders/BlinnPhongShader.vert)";
  inline const std::string BLINN_PHONG_FRAGMENT_SHADER_PATH =
    R"(./RenderSystem/Shaders/BlinnPhongShader.frag)";
  inline const std::string PBR_VERTEX_SHADER_PATH =
    R"(./RenderSystem/Shaders/PBRShader.vert)";
  inline const std::string PBR_FRAGMENT_SHADER_PATH =
    R"(./RenderSystem/Shaders/PBRShader.frag)";
  inline const std::string POINT_CLOUD_VERTEX_SHADER_PATH =
    R"(./RenderSystem/Shaders/PointCloudShader.vert)";
  inline const std::string POINT_CLOUD_FRAGMENT_SHADER_PATH =
    R"(./RenderSystem/Shaders/PointCloudShader.frag)";
  inline const std::string COLOR_VERTEX_SHADER_PATH =
    R"(./RenderSystem/Shaders/ColorShader.vert)";
  inline const std::string COLOR_FRAGMENT_SHADER_PATH =
    R"(./RenderSystem/Shaders/ColorShader.frag)";
  inline const std::string GLASS_VERTEX_SHADER_PATH =
    R"(./RenderSystem/Shaders/GlassShader.vert)";
  inline const std::string GLASS_FRAGMENT_SHADER_PATH =
    R"(./RenderSystem/Shaders/GlassShader.frag)";
  inline const std::string SHADOW_VERTEX_SHADER_PATH =
    R"(./RenderSystem/Shaders/ShadowShader.vert)";
  inline const std::string SHADOW_FRAGMENT_SHADER_PATH =
    R"(./RenderSystem/Shaders/ShadowShader.frag)";
  inline const std::string SCREEN_VERTEX_SHADER_PATH =
    R"(./RenderSystem/Shaders/ScreenShader.vert)";
  inline const std::string SCREEN_FRAGMENT_SHADER_PATH =
    R"(./RenderSystem/Shaders/ScreenShader.frag)";
  inline const std::filesystem::path SKYBOX_VERTEX_SHADER_PATH =
    R"(./RenderSystem/Shaders/SkyboxShader.vert)";
  inline const std::filesystem::path SKYBOX_FRAGMENT_SHADER_PATH =
    R"(./RenderSystem/Shaders/SkyboxShader.frag)";
  inline const std::filesystem::path PARTICLES_VERTEX_SHADER_PATH =
    R"(./RenderSystem/Shaders/ParticlesShader.vert)";
  inline const std::filesystem::path PARTICLES_FRAGMENT_SHADER_PATH =
    R"(./RenderSystem/Shaders/ParticlesShader.frag)";
  inline const std::filesystem::path WATER_VERTEX_SHADER_PATH =
    R"(./RenderSystem/Shaders/WaterShader.vert)";
  inline const std::filesystem::path WATER_FRAGMENT_SHADER_PATH =
    R"(./RenderSystem/Shaders/WaterShader.frag)";
  inline const std::array<std::filesystem::path, 6> SKYBOX_CUBEMAP_TEXTURES = {
    R"(./ThirdParty/resources/textures/skybox/right.png)",
    R"(./ThirdParty/resources/textures/skybox/left.png)",
    R"(./ThirdParty/resources/textures/skybox/top.png)",
    R"(./ThirdParty/resources/textures/skybox/bottom.png)",
    R"(./ThirdParty/resources/textures/skybox/front.png)",
    R"(./ThirdParty/resources/textures/skybox/back.png)",
  };
  inline const std::string FLOOR_MESH_PATH = R"(./ThirdParty/Models/Floor/Obj/Floor.obj)";
  inline const std::string FIRE_FLIPBOOK_PATH =
    R"(./ThirdParty/resources/textures/fire/FireFlipbook.png)";
  inline const std::string WATER_NORMAL_MAP_PATH =
    R"(./ThirdParty/resources/textures/water/NormalMap.jpg)";

  inline constexpr int SAMPLE_COUNT_TAA = 8;

  inline constexpr glm::vec3 DIR_LIGHT_POS(
    LIGHT_SOURCE_POS_X, LIGHT_SOURCE_POS_Y, LIGHT_SOURCE_POS_Z
  );

  inline const std::vector<float> SCREEN_QUAD_VERTICES = {
    -1.0f, -1.0f, 0.0f, 0.0f, 1.0f, -1.0f, 1.0f, 0.0f, -1.0f, 1.0f, 0.0f, 1.0f,
    -1.0f, 1.0f,  0.0f, 1.0f, 1.0f, -1.0f, 1.0f, 0.0f, 1.0f,  1.0f, 1.0f, 1.0f
  };

  inline const std::vector<float> PARTICLE_QUAD_VERTICES = {
    -0.5f, -0.5f, 0.0f, 0.0f, 0.5f,  -0.5f, 1.0f, 0.0f, 0.5f,  0.5f,  1.0f, 1.0f,
    0.5f,  0.5f,  1.0f, 1.0f, -0.5f, 0.5f,  0.0f, 1.0f, -0.5f, -0.5f, 0.0f, 0.0f
  };

  inline constexpr int FLIPBOOK_ROWS = 6;
  inline constexpr int FLIPBOOK_COLS = 6;
  inline constexpr int FLIPBOOK_TOTAL_FRAMES = FLIPBOOK_ROWS * FLIPBOOK_COLS;

  inline constexpr int STL_HEADER_SIZE = 80;
  inline const std::string DELIMITERS = " ,\t\n/";
  inline const std::unordered_set<std::string> SUPPORTED_TEXTURE_EXTENSIONS = {
    ".png", ".jpg", ".jpeg", ".bmp", ".tga"
  };
  inline constexpr size_t MAX_PARTICLES = 1000;
  inline constexpr size_t MIN_PARTICLES = 1;
  inline constexpr float PARTICLES_PER_SECOND = 200.0f;

  inline constexpr glm::vec3 WATER_COLOR = {0.0f, 0.0f, 1.0f};

  inline constexpr float CLOUD_POINT_SCALE = 2000.0f;
  inline constexpr float CLOUD_POINT_MIN_SIZE = 1.0f;
  inline constexpr float CLOUD_POINT_MAX_SIZE = 5.0f;

  inline constexpr float DEPTH_TEXTURE_BORDER_COLOR[] = {1.0f, 1.0f, 1.0f, 1.0f};
}  // namespace RenderSystem
