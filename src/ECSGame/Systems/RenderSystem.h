#pragma once
#include "ECS/ECS.h"
#include "ECSGame/Components/Transform.h"
#include "ECSGame/Components/MeshRenderer.h"
#include "ECSGame/Components/Camera.h"
#include <glm/glm.hpp>

namespace ECSGame
{
	class RenderSystem : public ECS::System
	{
	public:
		RenderSystem() : ECS::System(ECS::getMask<Transform, MeshRenderer>()) {}

		void render(glm::mat4 viewProjMatrix, glm::vec4 clearColor) const;
	};

	class CameraSystem : public ECS::System
	{
	public:
		CameraSystem() : ECS::System(ECS::getMask<Transform, Camera>()) {}

		bool getRenderCameraParams(glm::mat4& viewProjMatrix, glm::vec4& clearColor) const;

		void updateAspectRatio(float aspectRatio);
	};
}