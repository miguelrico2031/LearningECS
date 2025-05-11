#pragma once
#include "Instrumentalization.h"

class BaseEditor
{
public:
	virtual ~BaseEditor() = default;
	virtual void onAttach();
	virtual void update();
	virtual void fixedUpdate() {}
protected:
	Instrumentalization::FPSCounter m_fpsCounter;
	Instrumentalization::EventTimer m_closeGameTimer;

};

