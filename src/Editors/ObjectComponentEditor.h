#pragma once
#include "BaseEditor.h"

namespace ObjectComponent
{
	class Scene;
}

class ObjectComponentEditor : public BaseEditor
{
public:
	virtual void onAttach(Game& game) override;
	virtual void update() override;
	virtual void fixedUpdate() override;
private:
	void setUpTestScene();
	void createFloor();
	void createSpheres();
	void createBox();
	void moveCamera();
	void rotateCamera();
private:
	Game* m_game = nullptr;
	ObjectComponent::Scene* m_scene = nullptr;
};
