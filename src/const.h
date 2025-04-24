#pragma once
namespace WINDOW
{
	constexpr int WIDTH = 800;
	constexpr int HEIGHT = 600;
}

namespace CAMERA
{
	constexpr float V_FOV_DEGREES = 45.0f;
	constexpr float NEAR = 0.1f;
	constexpr float FAR = 100.0f;
	constexpr float ORTHO_SIZE = 10.0f;
}

namespace SHADER
{
	constexpr char* MODEL_VIEW_PROJ = "modelViewProj";
}