#pragma once
#include "GameObject.h"

namespace ObjectComponent
{
	class Component
	{
	public:
		Component(GameObject* gameObject) : m_gameObject(gameObject) {}
		virtual ~Component() = default;

		inline GameObject* getGameObject() const { return m_gameObject; }
		
		inline bool getEnabled() const { return m_enabled; }
		inline void setEnabled(bool enabled) { m_enabled = enabled; }

		template <class Component_T>
		Component_T* getComponent() const { return m_gameObject->getComponent<Component_T>(); }
		template <class Component_T>
		bool hasComponent() const { return m_gameObject->hasComponent<Component_T>(); }

	protected:
		GameObject* m_gameObject = nullptr;
		bool m_enabled = true;
	};
}