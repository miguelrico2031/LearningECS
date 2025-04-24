#include "GameObject.h"
#include "Transform.h"
#include "Log.h"

namespace ObjectComponent
{
	GameObject::GameObject() : m_active(true), m_tag("Unnamed")
	{
		LOG("creado game object!");
		m_components.push_back(std::make_unique<Transform>(this));
		m_transform = static_cast<Transform*>(m_components[0].get());
	}
	GameObject::GameObject(const std::string& tag) : m_tag(tag)
	{
		LOG("creado game object!");
		m_components.push_back(std::make_unique<Transform>(this));
		m_transform = static_cast<Transform*>(m_components[0].get());
	}
	GameObject::GameObject(const std::string& tag, glm::vec3 position, glm::vec3 rotation, glm::vec3 scale)
		: m_tag(tag)
	{
		LOG("creado game object!");
		m_components.push_back(std::make_unique<Transform>(this, position, rotation, scale));
		m_transform = static_cast<Transform*>(m_components[0].get());
	}

	Transform* GameObject::getTransform() const
	{
		return m_transform;
	}
}