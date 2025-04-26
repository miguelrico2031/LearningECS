#include "Game.h"

int main()
{
	Editor editor;
	Game::get().setUp();
	Game::get().attachEditor(&editor);
	Game::get().runLoop();
	Game::get().dispose();
}