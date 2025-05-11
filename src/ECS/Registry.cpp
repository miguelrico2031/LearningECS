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
		ComponentMask mask = getComponentMask(entity);
		for (size_t i = 0; i < MAX_COMPONENTS; i++)
		{
			if(mask.test(i))
			{
				ComponentMask oldMask = mask;
				mask.set(i, false);
				m_entitiesManager.setComponentMask(entity, mask);
				m_componentsManager.removeComponent(entity, i);
				m_systemsManager.onComponentRemoved(entity, oldMask);
			}
		}

		m_entitiesManager.destroyEntity(entity);
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