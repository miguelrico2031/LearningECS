#pragma once
#include "Entity.h"
#include <queue>
#include <array>

namespace ECS
{

	class EntitiesManager
	{
	public:

		Entity createEntity();
		void destroyEntity(Entity entity);
		ComponentMask getComponentMask(Entity entity) const;
		void setComponentMask(Entity entity, ComponentMask mask);
		bool getIsAlive(Entity entity) const;
	private:

		struct EntityData
		{
			ComponentMask m_componentMask = {};
			bool m_isAlive = false;
		};

		// Queue of destroyed entities to use when creating one instead of using the m_nextEntity++;
		std::queue<Entity> m_availables;
		std::array<EntityData, MAX_ENTITIES> m_entityData = {};
		Entity m_nextEntity = 1;
		unsigned int m_existentEntitiesCount = 0;
	};
}