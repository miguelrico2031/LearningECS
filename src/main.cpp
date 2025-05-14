#include "Game.h"
#include "Editors/ObjectComponentEditor.h"
#include "Editors/ECSEditor.h"
#include "Log.h"
#include "Random.h"

int main()
{
	for (size_t i = 0; i < 50; i++)
	{
		BaseEditor::EditorParams params =
		{
			500,//Random::intInRange(1000, 4000),	// sphereCount
			{0.0f, 40.0f, 0.0f},			// spheresOrigin
			10.0f,							// spheresSpread
			-1,//Random::floatInRange(2, 7),		// simDurationSeconds
			5.0f,							// cameraMoveSpeed
			25.0f,							// cameraSprintSpeed
			0.04f							// cameraSensitivity
		};

		{
			ECSEditor editor;
			editor.setParams(params);
			Game game;
			game.setUp();
			game.attachEditor(&editor);
			game.runLoop();
			game.dispose();
		}
		{
			ObjectComponentEditor editor;
			editor.setParams(params);
			Game game;
			game.setUp();
			game.attachEditor(&editor);
			game.runLoop();
			game.dispose();
		}
	}
}