#pragma once
#include "Instrumentalization.h"

class Game;
class BaseEditor
{
public:
	virtual ~BaseEditor() = default;
	virtual void onAttach(Game& game);
	virtual void update();
	virtual void fixedUpdate() {}
public:
	struct EditorParams
	{
		size_t m_sphereCount;
		float m_spheresOrigin[3];
		float m_spheresSpread;
		float m_simDurationSeconds; //-1 to run inf
		float m_cameraMoveSpeed;
		float m_cameraSprintSpeed;
		float m_cameraSensitivity;
	};
	void setParams(EditorParams params) { m_params = params; }
protected:
	Instrumentalization::FPSCounter m_fpsCounter;
	Instrumentalization::EventTimer m_closeGameTimer;
	EditorParams m_params;

};

