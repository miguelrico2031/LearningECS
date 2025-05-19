#pragma once
#include <glm/glm.hpp>
namespace WINDOW
{
	inline constexpr int WIDTH = 1000;
	inline constexpr int HEIGHT = 800;
}


inline constexpr float FIXED_DELTA_TIME = 1/60.0f;


namespace CAMERA
{
	inline constexpr float V_FOV_DEGREES = 45.0f;
	inline constexpr float NEAR = 0.1f;
	inline constexpr float FAR = 1000.0f;
	inline constexpr float ORTHO_SIZE = 10.0f;
	inline const glm::vec4 CLEAR_COLOR_1 = { 0.2f, 0.0f, 0.1f, 1.0f };
	inline const glm::vec4 CLEAR_COLOR_2 = { 0.15f, 0.15f, 0.4f, 1.0f };
}

namespace SHADER
{
	inline const char* MODEL_VIEW_PROJ = "modelViewProj";
	inline const char* COLOR = "color";
	inline const char* TILING = "tiling";
}

namespace TEXTURE
{
	inline const char* NO_TEX_PATH = "assets/textures/notex.png";
}