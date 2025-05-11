#include "Game.h"
#include "Editors/ObjectComponentEditor.h"
#include "Editors/ECSEditor.h"
#include "Log.h"


int main()
{


	ObjectComponentEditor editor;
	//ECSEditor editor;
	Game::get().setUp();
	Game::get().attachEditor(&editor);
	Game::get().runLoop();
	Game::get().dispose();

}