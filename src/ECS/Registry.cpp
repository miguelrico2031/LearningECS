#include "Registry.h"

namespace ECS
{
	Entity Registry::createEntity()
	{
		Entity entity = m_entitiesManager.createEntity();
		return entity;
	}
	void Registry::destroyEntity(Entity entity)
	{
		m_entitiesManager.destroyEntity(entity);
		//TODO: remove entity from all systems with matching component mask
	}

	ComponentMask Registry::getComponentMask(Entity entity) const
	{
		return m_entitiesManager.getComponentMask(entity);
	}

	bool Registry::getIsAlive(Entity entity) const
	{
		return m_entitiesManager.getIsAlive(entity);
	}

}