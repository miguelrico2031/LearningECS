#pragma once
#include "Entity.h"
#include <vector>

namespace ECS
{
	class Registry;
	class System
	{
		friend class SystemsManager;
	public:
		System(ComponentMask mask) : m_mask(mask) {}
		virtual ~System() = default;
	protected:
		const std::vector<Entity>& getEntities() const
		{
			return m_orderedEntities;
		}
	protected:
		Registry* m_registry = nullptr;
	private:
		std::vector<Entity> m_orderedEntities;
		ComponentMask m_mask;
	};

}
