#include "BaseEditor.h"
#include "GameTime.h"
#include "Game.h"
void BaseEditor::onAttach()
{
	m_closeGameTimer.start(10.0f, []() { Game::get().quit(); });
}

void BaseEditor::update()
{
	float ts = GameTime::getDeltaTime(true);
	m_fpsCounter.update(ts);
	m_closeGameTimer.update(ts);
}

