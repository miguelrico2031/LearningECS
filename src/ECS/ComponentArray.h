#pragma once
#include "Entity.h"
#include <array>
#include <unordered_map>
#include <cassert>
#include <span>

namespace ECS
{
	class BaseComponentArray
	{
	public:
		virtual ~BaseComponentArray() = default;
	};

	template<class Component_T>
	class ComponentArray : public BaseComponentArray
	{
	public:

		//ComponentMask update handled outside of this class
		//component remove only needs ComponentMask bit to be set to 0, therefore also handled outside of this class
		
		
		void add(Entity entity, Component_T component)
		{
			assert(IS_IN_RANGE(entity) && "Invalid entity to add component.");
			m_components[(size_t)entity] = component; //TODO: move instead of copy?
			
		}

		//returns garbage if entity doesn't have component
		//therefore ComponentMask check is needed before calling get()
		Component_T& get(Entity entity) const 
		{
			return m_components[entity];
		}

	private:
		std::array<Component_T, MAX_ENTITIES> m_components;
	};
}
