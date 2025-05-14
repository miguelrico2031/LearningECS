#pragma once
#include "BaseEditor.h"

namespace ECSGame
{
	class Scene;
	class FreeMovementSystem;
}

class ECSEditor : public BaseEditor
{
public:
	virtual void onAttach(Game& game) override;
	virtual void update() override;
	virtual void fixedUpdate() override;
private:
	void initializeECSRegistry();
	void setUpTestScene();
	void createFloor();
	void createSpheres();
	void createBox();
private:
	Game* m_game = nullptr;
	ECSGame::Scene* m_scene = nullptr;
	ECSGame::FreeMovementSystem* m_freeMovementSystem = nullptr;
};