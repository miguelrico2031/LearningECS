#pragma once
#include "System.h"
#include <memory>

namespace ECS
{
	class SystemsManager
	{
	public:
		template <class System_T>
		System_T& addSystem()
		{
			static_assert(std::is_base_of<System, System_T>::value, "System_T must inherit from System");
			m_systems.push_back(std::make_unique<System_T>());

			//mask
			
			System* ptr = m_systems.back().get();
			System_T* derivedPtr = static_cast<System_T*>(ptr);
			return *derivedPtr;
		}

		void onComponentAdded(Entity entity, ComponentMask newMask);
		void onComponentRemoved(Entity entity, ComponentMask oldMask);
	private:
		void addEntityToSystemInOrder(Entity entity, System& system);
		void removeEntityFromSystem(Entity entity, System& system);
private:
	std::vector<std::unique_ptr<System>> m_systems = {};
	};
}