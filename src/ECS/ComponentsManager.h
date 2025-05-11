#pragma once
#include "ComponentArray.h"
#include "Event.h"
#include <memory>
#include <functional>

namespace ECS
{
	class ComponentsManager
	{
	public:
		template<class Component_T>
		static ComponentType registerComponentType()
		{
			assert(s_componentTypeCount < MAX_COMPONENTS && "Too many component types created, maximum reached.");
			ComponentType index = getComponentType<Component_T>();

			s_componentArraysFactory[index] = []() { return std::make_unique<ComponentArray<Component_T>>(); };
			return index;
		}


		template<class Component_T>
		static ComponentType getComponentType()
		{
			// 1 static index per template, this means for each different component type the m_componentCount will increment once
			//and the index will be the same for all subsequent calls of getComponentType with that component type as the template parameter
			//this means this should be always called the first time for every component type by registerComponentType().
			static ComponentType index = s_componentTypeCount++;
			return index;
		}


		void initialize()
		{
			for (size_t i = 0; i < s_componentTypeCount; i++)
			{
				assert(s_componentArraysFactory[i] != nullptr && "Component type used before being added.");
				m_componentArrays[i] = s_componentArraysFactory[i]();
			}
		}

		template<class Component_T>
		void addComponent(Entity entity, Component_T component)
		{
			ComponentArray<Component_T>& componentArray = getComponentArray<Component_T>();
			componentArray.add(entity, component);

			ComponentType index = getComponentType<Component_T>();
			Event<Entity>& event = m_componentAddedEvents[index];
			event.invoke(entity);
		}

		//only called to invoke events, the actual removal is not necessary with this implementation
		//needs to be called before componentmask is updated due to removal
		template<class Component_T>
		void removeComponent(Entity entity)
		{
			ComponentType index = getComponentType<Component_T>();
			assert(index < MAX_COMPONENTS && "Invalid component type.");
			assert(m_componentArrays[index] != nullptr && "Component type used before being added.");
			Event<Entity>& event = m_componentRemovedEvents[index];
			event.invoke(entity);
		}

		//version with index for destroyEntity
		void removeComponent(Entity entity, ComponentType index)
		{
			assert(index < MAX_COMPONENTS && "Invalid component type.");
			assert(m_componentArrays[index] != nullptr && "Component type used before being added.");
			Event<Entity>& event = m_componentRemovedEvents[index];
			event.invoke(entity);
		}

		template<class Component_T>
		const Component_T& getComponent(Entity entity) const
		{
			ComponentArray<Component_T>& componentArray = getComponentArray<Component_T>();
			return componentArray.get(entity);
		}

		template<class Component_T>
		Component_T& getComponent(Entity entity)
		{
			ComponentArray<Component_T>& componentArray = getComponentArray<Component_T>();
			return componentArray.get(entity);
		}


		template<class Component_T>
		Event<Entity>& getComponentAddedEvent()
		{
			ComponentType index = getComponentType<Component_T>();
			return m_componentAddedEvents[index];
		}

		template<class Component_T>
		Event<Entity>& getComponentRemovedEvent()
		{
			ComponentType index = getComponentType<Component_T>();
			return m_componentRemovedEvents[index];
		}

	private:
		static ComponentType s_componentTypeCount;
		static std::array<std::function<std::unique_ptr <BaseComponentArray>()>, MAX_COMPONENTS> s_componentArraysFactory;

		std::array<std::unique_ptr<BaseComponentArray>, MAX_COMPONENTS> m_componentArrays = {}; //ComponentType is the index
		std::array<Event<Entity>, MAX_COMPONENTS> m_componentAddedEvents = {};
		std::array<Event<Entity>, MAX_COMPONENTS> m_componentRemovedEvents = {};
	private:

		//helper method that returns the array downcasted
		template<class Component_T>
		ComponentArray<Component_T>& getComponentArray() const
		{
			ComponentType index = getComponentType<Component_T>();
			assert(index < MAX_COMPONENTS && "Invalid component type.");
			assert(m_componentArrays[index] != nullptr && "Component type used before being added.");

			BaseComponentArray* baseArray = m_componentArrays[index].get();
			ComponentArray<Component_T>* componentArray = static_cast<ComponentArray<Component_T>*>(baseArray);
			return *componentArray;
		}
	};
}