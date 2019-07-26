#include <SDL.h>
#include "LWindow.h"
#include "Game.h"
#include <time.h>

int main(int argc, char* args[]) {
	srand(time(NULL));

	LWindow mWindow;
	Game game;

	game.run();

	return 0;
}