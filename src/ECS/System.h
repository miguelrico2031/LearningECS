#pragma once
#include "Entity.h"
#include <vector>

namespace ECS
{
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
	private:
		ComponentMask m_mask;
		std::vector<Entity> m_orderedEntities;
	};

}
