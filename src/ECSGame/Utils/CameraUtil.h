	#pragma once
#include "ECSGame/Components/Camera.h"

namespace ECSGame
{
	class CameraUtil
	{
	public:
		static void updateProjectionMatrix(const ECSGame::Camera& camera);
	};
}