#pragma once
#include "Component.h"
#include "Transform.h"
#include "Rendering/Mesh.h"
#include "Rendering/Material.h"
#include "const.h"
#include <memory>

namespace ObjectComponent
{
	class MeshRenderer : public Component
	{
	public:
		MeshRenderer(GameObject* gameObject)
			: Component(gameObject)
		{
		}

		inline std::weak_ptr<Mesh> getMesh() const { return m_mesh; }
		inline std::weak_ptr<Material> getMaterial() const { return m_material; }

		inline void setMesh(std::shared_ptr<Mesh> mesh) { m_mesh = mesh; }
		inline void setMaterial(std::shared_ptr<Material> material)
		{
			m_material = material;
		}

		void render(glm::mat4 viewProjectionMatrix)
		{
			if (m_mesh == nullptr || m_material == nullptr)
				return;

			glm::mat4 modelViewProj = viewProjectionMatrix * m_gameObject->getTransform()->getModelMatrix();
			m_material->activate();
			m_material->setModelViewProjMatrix(modelViewProj);
			m_mesh->bind();
			m_mesh->render();
			m_mesh->unbind();
		}

	private:
		std::shared_ptr<Mesh> m_mesh = nullptr;
		std::shared_ptr<Material> m_material = nullptr;
	};
}