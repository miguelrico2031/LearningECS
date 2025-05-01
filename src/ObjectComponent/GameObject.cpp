#include "GameObject.h"
#include "Transform.h"
#include "Log.h"
#include "Scene.h"

namespace ObjectComponent
{
	GameObject::GameObject(Scene* scene) : m_scene(scene), m_tag("Unnamed")
	{
		m_components.push_back(std::make_unique<Transform>(this));
		m_transform = static_cast<Transform*>(m_components[0].get());
	}

	GameObject::GameObject(Scene* scene, const std::string& tag) : m_scene(scene), m_tag(tag)
	{
		m_components.push_back(std::make_unique<Transform>(this));
		m_transform = static_cast<Transform*>(m_components[0].get());
	}

	void GameObject::onComponentAdded(Component* component)
	{
		m_scene->onComponentAdded(this, component);
	}
	void GameObject::onComponentRemoved(Component* component)
	{
		m_scene->onComponentRemoved(this, component);
	}
}