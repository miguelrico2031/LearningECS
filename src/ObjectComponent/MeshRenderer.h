#pragma once
#include "Component.h"
#include "Transform.h"
#include "Mesh.h"
#include "Material.h"
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
			m_modelViewProjLocation = glGetUniformLocation(m_material->getShader().getID(), SHADER::MODEL_VIEW_PROJ);
		}

		void render(glm::mat4 viewProjectionMatrix)
		{
			if (m_mesh == nullptr || m_material == nullptr)
				return;

			m_material->activate();

			Shader& shader = m_material->getShader();
			glm::mat4 modelViewProj = viewProjectionMatrix * m_gameObject->getTransform()->getModelMatrix();
			shader.setMat4(m_modelViewProjLocation, modelViewProj);

			m_mesh->render();
		}

	private:
		std::shared_ptr<Mesh> m_mesh = nullptr;
		std::shared_ptr<Material> m_material = nullptr;
		GLint m_modelViewProjLocation = -1;
	};
}