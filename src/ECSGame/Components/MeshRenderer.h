#pragma once
#include "Rendering/Material.h"
#include "Rendering/Mesh.h"
#include <memory>

namespace ECSGame
{
	struct MeshRenderer
	{
		std::shared_ptr<Mesh> m_mesh = nullptr;
		std::shared_ptr<Material> m_material = nullptr;
	};
}