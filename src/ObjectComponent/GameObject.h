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

	class GameObject
	{
	public:
		GameObject();
		GameObject(const std::string& tag);
		GameObject(const std::string& tag, glm::vec3 position, glm::vec3 rotation, glm::vec3 scale);

		inline bool getActive() const { return m_active; }
		inline void setActive(bool active) { m_active = active; }

		inline const std::string& getTag() const { return m_tag; }
		inline void setTag(const std::string& tag) { m_tag = tag; }

		Transform* getTransform() const;

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
			return static_cast<Component_T*>(m_components.back().get());
		}

		template<class Component_T>
		bool removeComponent()
		{
			static_assert(std::is_base_of<Component, Component_T>::value, "Component_T must inherit from Component");
			for (auto it = m_components.begin(); it != m_components.end(); ++it)
			{
				if (dynamic_cast<Component_T*>(it->get()))
				{
					m_components.erase(it);
					return true;
				}
			}
			return false;
		}

	private:
		std::vector<std::unique_ptr<Component>> m_components;
		std::string m_tag;
		bool m_active = true;
		Transform* m_transform;
	};
};