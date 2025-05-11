#pragma once
#include "Registry.h"

namespace ECS
{
	template<class Component_T>
	static ComponentType registerComponentType()
	{
		return ComponentsManager::registerComponentType<Component_T>();
	}

	template<class ...Component_T>
	static ComponentMask getMask()
	{
		ComponentMask mask;
		(mask.set(ComponentsManager::getComponentType<Component_T>(), true), ...);
		return mask;
	}

	/*
	* API:
	*
	* //component declaration
	* 
	* struct Component1
	* {
	*	int a, b;
	* };
	*
	* struct Component2
	* {
	*   float x, y;
	* };
	*
	* // component registration
	* 
	* ECS::registerComponentType<Component1>();
	* ECS::registerComponentType<Component2>();
	* 
	* 
	* // systems declaration
	* class C2System : public ECS::System
	* {
	* public:
	*	C2System() : ECS::System(ECS::getMask<Component2>()) {}
	* 
	*	void update()
	*   {
	*		for(auto entity : getEntities())
	*		{
	*			auto& c2 = getComponent<Component2>(entity);
	*           c2.x += 1.0f;
	*		}
	*	}
	* };
	*
	* 
	* struct Scene
	* {
	*   Registry reg;
	* }
	* 
	* Scene scene;
	* 
	* scene.reg.initialize(); ALWAYS initialize the registry after registering all the component types, NEVER before
	* 
	* //systems registration
	* C2System* c2SystemPtr = scene.reg.addSystem<C2System>();
	* C2System& c2System = *c2SystemPtr;
	* 
	* ECS::Entity entity1 = scene.reg.createEntity();
	* scene.reg.addComponent<Component1>(entity1, { 1, 2 });
	* scene.reg.addComponent<Component2>(entity1, { 3.0f, 4.0f });
	* ECS::Entity entity2 = scene.reg.createEntity();
	* scene.reg.addComponent<Component2>(entity2, { 0.5f, 10.0f });
	* 
	* //systems calls ..
	* for (int i = 0; i < 10000; i++)
	* {
	* 	c2System.update();
	* }
	*
	* scene.reg.destroyEntity(entity2);
	* scene.reg.removeComponent<Component2>(entity1);
	* 
	*/
}
