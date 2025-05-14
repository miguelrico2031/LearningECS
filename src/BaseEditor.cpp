#include "BaseEditor.h"
#include "GameTime.h"
#include "Game.h"
void BaseEditor::onAttach(Game& game)
{
	if (m_params.m_simDurationSeconds >= 0)
	{
		m_closeGameTimer.start(m_params.m_simDurationSeconds, [&game]() { game.quit(); });
	}
}

void BaseEditor::update()
{
	float ts = GameTime::getDeltaTime(true);
	m_fpsCounter.update(ts);
	m_closeGameTimer.update(ts);
}

