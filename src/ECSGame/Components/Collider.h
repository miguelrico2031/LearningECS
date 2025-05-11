#pragma once
#include <btBulletCollisionCommon.h>

namespace ECSGame
{
	struct Collider
	{
		enum class Type : uint8_t
		{
			Box,
			Sphere
		};
		Type m_type = Type::Box;
		btCollisionShape* m_btCollisionShape = nullptr;
	};
}