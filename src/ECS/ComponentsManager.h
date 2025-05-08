#pragma once
#include "ComponentArray.h"
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
			//assert(m_componentArrays[index] == nullptr && "Component type already added.");
			//m_componentArrays[index] = std::make_unique<ComponentArray<Component_T>>();
			//return index;

			s_componentArraysFactory[index] = []() { return std::make_unique<ComponentArray<Component_T>>(); };
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
		}

		//template<class Component_T>
		//void removeComponent(Entity entity)
		//{
		//	ComponentArray<Component_T>& componentArray = getComponentArray<Component_T>();
		//	componentArray.remove(entity);
		//}

		template<class Component_T>
		Component_T& getComponent(Entity entity)
		{
			ComponentArray<Component_T>& componentArray = getComponentArray<Component_T>();
			return componentArray.get(entity);
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

	private:
		static ComponentType s_componentTypeCount;
		static std::array<std::function<std::unique_ptr <BaseComponentArray>()>, MAX_COMPONENTS> s_componentArraysFactory;

		std::array<std::unique_ptr<BaseComponentArray>, MAX_COMPONENTS> m_componentArrays = {}; //ComponentType is the index
	private:

		//helper method that returns the array downcasted
		template<class Component_T>
		ComponentArray<Component_T>& getComponentArray()
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