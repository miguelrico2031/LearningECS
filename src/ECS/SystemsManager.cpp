#include "SystemsManager.h"

namespace ECS
{
	void SystemsManager::onComponentAdded(Entity entity, ComponentMask newMask)
	{
		for (auto& system : m_systems)
		{
			if ((newMask & system->m_mask) == system->m_mask)
			{
				addEntityToSystemInOrder(entity, *system);
			}
		}
	}


	void SystemsManager::onComponentRemoved(Entity entity, ComponentMask oldMask)
	{
		for (auto& system : m_systems)
		{
			if ((oldMask & system->m_mask) == system->m_mask)
			{
				removeEntityFromSystem(entity, *system);
			}
		}
	}


	void SystemsManager::addEntityToSystemInOrder(Entity entity, System& system)
	{
		// Insert the entity into m_orderedEntities while maintaining order
		auto& entities = system.m_orderedEntities;
		auto it = std::lower_bound(entities.begin(), entities.end(), entity);
		if (it == entities.end() || *it != entity)
		{
			entities.insert(it, entity);
		}
	}


	void SystemsManager::removeEntityFromSystem(Entity entity, System& system)
	{
		// Remove the entity from m_orderedEntities if it exists
		auto& entities = system.m_orderedEntities;
		auto it = std::find(entities.begin(), entities.end(), entity);
		if (it != entities.end())
		{
			entities.erase(it);
		}
	}
}