#pragma once
#include <glm/glm.hpp>
namespace WINDOW
{
	constexpr int WIDTH = 800;
	constexpr int HEIGHT = 600;
}

namespace TIME
{
	constexpr float FIXED_DELTA_TIME = 1/60.0f;
}

namespace CAMERA
{
	constexpr float V_FOV_DEGREES = 45.0f;
	constexpr float NEAR = 0.1f;
	constexpr float FAR = 1000.0f;
	constexpr float ORTHO_SIZE = 10.0f;
	const glm::vec4 CLEAR_COLOR = { 0.2f, 0.0f, 0.1f, 1.0f };
}

namespace SHADER
{
	constexpr char* MODEL_VIEW_PROJ = "modelViewProj";
}

namespace TEXTURE
{
	constexpr char* NO_TEX_PATH = "../assets/textures/notex.png";
}