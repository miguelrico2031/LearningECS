#pragma once
#include <stdint.h>
#include <bitset>

namespace ECS
{
	using Entity = uint32_t;
	constexpr Entity NULL_ENTITY = 0;
	constexpr Entity MAX_ENTITIES = 4096*2;
	using ComponentType = uint8_t;
	constexpr ComponentType MAX_COMPONENTS = 32;
	using ComponentMask = std::bitset<MAX_COMPONENTS>;

	#define IS_IN_RANGE(entity) 0 < entity && entity < MAX_ENTITIES

}