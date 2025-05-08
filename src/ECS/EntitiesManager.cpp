#include "EntitiesManager.h"
#include <cassert>

namespace ECS
{

	Entity EntitiesManager::createEntity()
	{
		assert(m_existentEntitiesCount < MAX_ENTITIES && "Too many entities created, maximum reached.");
		m_existentEntitiesCount++;
		Entity entity = NULL_ENTITY;
		if (m_availables.empty())
		{
			Entity entity = m_nextEntity++;
			
		}
		else
		{
			Entity entity = m_availables.front();
			m_availables.pop();
			m_entityData[entity].m_componentMask.reset();
		}
		m_entityData[entity].m_isAlive = true;
		return entity;
	}

	void EntitiesManager::destroyEntity(Entity entity)
	{
		assert(IS_IN_RANGE(entity) && "Invalid entity to destroy.");
		assert(getIsAlive(entity) && "Cannot destroy dead entity.");
		m_existentEntitiesCount--;
		m_availables.push(entity);
		m_entityData[entity].m_isAlive = false;
	}

	ComponentMask EntitiesManager::getComponentMask(Entity entity) const
	{
		assert(IS_IN_RANGE(entity) && "Invalid entity to get mask.");
		assert(getIsAlive(entity) && "Cannot get mask from dead entity.");
		return m_entityData[entity].m_componentMask;
	}

	void EntitiesManager::setComponentMask(Entity entity, ComponentMask mask)
	{
		assert(IS_IN_RANGE(entity) && "Invalid entity to set mask.");
		assert(getIsAlive(entity) && "Cannot set mask to dead entity.");

		m_entityData[entity].m_componentMask = mask;
	}

	bool EntitiesManager::getIsAlive(Entity entity) const
	{
		return m_entityData[entity].m_isAlive;
	}
}
