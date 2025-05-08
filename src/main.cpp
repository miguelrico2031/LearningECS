#include "Game.h"
#include "Editors/ObjectComponentEditor.h"
#include "ECS/ECS.h"
int main()
{
	ObjectComponentEditor editor;
	Game::get().setUp();
	Game::get().attachEditor(&editor);
	Game::get().runLoop();
	Game::get().dispose();
}