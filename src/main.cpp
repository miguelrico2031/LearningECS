#include "Game.h"
#include "ObjectComponent/Editor.h"
int main()
{
	ObjectComponent::Editor editor;
	Game::get().setUp();
	Game::get().attachEditor(&editor);
	Game::get().runLoop();
	Game::get().dispose();
}