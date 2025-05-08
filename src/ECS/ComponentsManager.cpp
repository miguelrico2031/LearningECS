#include "ComponentsManager.h"

namespace ECS
{
	ComponentType ComponentsManager::s_componentTypeCount = 0;
	std::array<std::function<std::unique_ptr <BaseComponentArray>()>, MAX_COMPONENTS> ComponentsManager::s_componentArraysFactory = {};
}