#pragma once
#include "Entity.h"
#include "EntitiesManager.h"
#include "ComponentsManager.h"
#include "SystemsManager.h"

namespace ECS
{
	class Registry
	{
	public:
		void initialize()
		{
			m_componentsManager.initialize();
		}

#pragma region ENTITY
		Entity createEntity();
		void destroyEntity(Entity entity);

		ComponentMask getComponentMask(Entity entity) const;
		bool getIsAlive(Entity entity) const;

		template<class Component_T>
		void addComponent(Entity entity, Component_T component)
		{
			assert(IS_IN_RANGE(entity) && "Invalid entity to add component.");
			assert(getIsAlive(entity) && "Cannot add component to dead entity.");

			ComponentMask mask = getComponentMask(entity);
			ComponentType maskBit = m_componentsManager.getComponentType<Component_T>();
			assert(!mask.test(maskBit) && "Component already added to entity.");
			mask.set(maskBit, true);
			m_entitiesManager.setComponentMask(entity, mask);
			m_componentsManager.addComponent(entity, component);

			m_systemsManager.onComponentAdded(entity, mask);
		}
#pragma endregion
#pragma region COMPONENT
		template<class Component_T>
		void removeComponent(Entity entity)
		{
			assert(IS_IN_RANGE(entity) && "Invalid entity to remove component.");
			assert(getIsAlive(entity) && "Cannot remove component to dead entity.");

			ComponentMask oldMask = getComponentMask(entity);
			ComponentMask mask = oldMask;
			ComponentType maskBit = m_componentsManager.getComponentType<Component_T>();
			assert(mask.test(maskBit) && "Entity donesn't have the component to remove.");
			mask.set(maskBit, false);
			m_entitiesManager.setComponentMask(entity, mask);

			m_systemsManager.onComponentRemoved(entity, oldMask);
		}

		template<class Component_T>
		bool hasComponent(Entity entity) const
		{
			assert(IS_IN_RANGE(entity) && "Invalid entity to check component.");
			assert(getIsAlive(entity) && "Cannot check component of dead entity.");
			ComponentMask mask = getComponentMask(entity);
			ComponentType maskBit = m_componentsManager.getComponentType<Component_T>();
			return mask.test(maskBit);
		}


		template<class Component_T>
		Component_T& getComponent(Entity entity) const
		{
			assert(IS_IN_RANGE(entity) && "Invalid entity to get component.");
			assert(getIsAlive(entity) && "Cannot get component from dead entity.");
#ifndef NDEBUG
			ComponentMask mask = getComponentMask(entity);
			ComponentType maskBit = m_componentsManager.getComponentType<Component_T>();
			assert(mask.test(maskBit) && "Entity doesn't have the component to get.");
#endif 
			return m_componentsManager.getComponent<Component_T>(entity);
		}

		template<class Component_T>
		bool tryGetComponent(Entity entity, Component_T* componentPtr) const
		{
			assert(IS_IN_RANGE(entity) && "Invalid entity to get component.");
			assert(getIsAlive(entity) && "Cannot get component from dead entity.");

			ComponentMask mask = getComponentMask(entity);
			ComponentType maskBit = m_componentsManager.getComponentType<Component_T>();
			if (mask.test(maskBit))
			{
				componentPtr = &m_componentsManager.getComponent<Component_T>(entity);
				return true;
			}
			else
			{
				componentPtr = nullptr;
				return false;
			}
		}
#pragma endregion
#pragma region SYSTEM
		template<class System_T>
		System_T& addSystem()
		{
			return m_systemsManager.addSystem<System_T>();
		}

	private:
		EntitiesManager m_entitiesManager;
		ComponentsManager m_componentsManager;
		SystemsManager m_systemsManager;
	};
}