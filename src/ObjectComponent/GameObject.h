#pragma once
#include <string>
#include <vector>
#include <memory> 
#include <type_traits>
#include <glm/glm.hpp>

namespace ObjectComponent
{
	class Component;
	class Transform;
	class Scene;

	class GameObject
	{
	public:
		GameObject(Scene* scene);
		GameObject(Scene* scene, const std::string& tag);

		GameObject(const GameObject&) = delete;
		GameObject& operator=(const GameObject&) = delete;

		GameObject(GameObject&&) = default;
		GameObject& operator=(GameObject&&) = default;

		inline bool getActive() const { return m_active; }
		inline void setActive(bool active) { m_active = active; }

		inline const std::string& getTag() const { return m_tag; }
		inline void setTag(const std::string& tag) { m_tag = tag; }

		Transform* getTransform() const { return m_transform; }

		template<class Component_T>
		Component_T* getComponent() const
		{
			static_assert(std::is_base_of<Component, Component_T>::value, "Component_T must inherit from Component");
			for (const auto& component : m_components)
			{
				if (auto casted = dynamic_cast<Component_T*>(component.get()))
					return casted;
			}
			return nullptr;
		}

		template<class Component_T>
		bool hasComponent() const
		{
			return getComponent<Component_T>() != nullptr;
		}

		template<class Component_T>
		Component_T* addComponent()
		{
			static_assert(std::is_base_of<Component, Component_T>::value, "Component_T must inherit from Component");
			static_assert(!std::is_base_of<Transform, Component_T>::value, "Component_T cannot be a Transform");
			m_components.push_back(std::make_unique<Component_T>(this));
			Component* newComponent = m_components.back().get();
			onComponentAdded(newComponent);
			return static_cast<Component_T*>(newComponent);
		}

		template<class Component_T>
		bool removeComponent()
		{
			static_assert(std::is_base_of<Component, Component_T>::value, "Component_T must inherit from Component");
			static_assert(!std::is_base_of<Transform, Component_T>::value, "Component_T cannot be a Transform");
			for (auto it = m_components.begin(); it != m_components.end(); ++it)
			{
				if (dynamic_cast<Component_T*>(it->get()))
				{
					onComponentRemoved(it->get());
					m_components.erase(it);
					return true;
				}
			}
			return false;
		}

	private:
		std::vector<std::unique_ptr<Component>> m_components;
		std::string m_tag;
		Scene* m_scene;
		Transform* m_transform;
		bool m_active = true;
	private:
		void onComponentAdded(Component* component);
		void onComponentRemoved(Component* component);
	};
};