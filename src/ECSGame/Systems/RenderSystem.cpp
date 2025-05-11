#include "RenderSystem.h"
#include "ECSGame/Utils/TransformUtil.h"
#include "ECSGame/Utils/CameraUtil.h"
#include <glm/gtc/matrix_transform.hpp>

namespace ECSGame
{
	void RenderSystem::render(glm::mat4 viewProjMatrix, glm::vec4 clearColor) const
	{
		glClearColor(clearColor.r, clearColor.g, clearColor.b, clearColor.a);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//store refs to last used mesh and material ptrs
		//only bind mesh or bind material if different from last ptr
		std::shared_ptr<const Mesh> lastMesh = nullptr;
		std::shared_ptr<Material> lastMat = nullptr;
		for (ECS::Entity e : getEntities())
		{
			//model view proj matrix
			const Transform& transform = m_registry->getComponent<Transform>(e);
			glm::mat4 modelMatrix = TransformUtil::calculateModelMatrix(transform);
			glm::mat4 modelViewProj = viewProjMatrix * modelMatrix;

			const MeshRenderer& renderer = m_registry->getComponent<MeshRenderer>(e);
			if (renderer.m_material != lastMat)
			{
				lastMat = renderer.m_material;
				lastMat->activate();
			}
			lastMat->setModelViewProjMatrix(modelViewProj);
			if (renderer.m_mesh != lastMesh)
			{
				lastMesh = renderer.m_mesh;
				lastMesh->bind();
			}
			lastMesh->render();
		}
		if (lastMesh)
			lastMesh->unbind();
	}


	bool RenderCameraParamsSystem::getRenderCameraParams(glm::mat4& viewProjMatrix, glm::vec4& clearColor) const
	{
		if (getEntities().empty())
			return false;

		ECS::Entity e = getEntities().back();

		//view matrix
		const Transform& transform = m_registry->getComponent<Transform>(e);
		glm::mat4 view = glm::lookAt(transform.m_position,
			transform.m_position + TransformUtil::getForward(transform),
			TransformUtil::getUp(transform));

		//projection matrix
		const Camera& camera = m_registry->getComponent<Camera>(e);
		CameraUtil::updateProjectionMatrix(camera);

		viewProjMatrix = camera.m_projMatrix * view;
		clearColor = camera.m_clearColor;
		return true;
	}

}