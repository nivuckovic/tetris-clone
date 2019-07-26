#include "LWindow.h"

const int LWindow::SCREEN_WIDTH = 512;
const int LWindow::SCREEN_HEIGHT = 640;

SDL_Window* LWindow::mWindow = NULL;
SDL_Renderer* LWindow::mRenderer = NULL;

LWindow::LWindow()
{
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		printf("SDL could not initialize!");
	}
	else {
		mWindow = SDL_CreateWindow("Tetris", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
		if (mWindow == NULL) {
			printf("Window could not be created!");
		}
		else {
			mRenderer = SDL_CreateRenderer(mWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
			if (mRenderer == NULL) {
				printf("Renderer could not be created!");
			}
			else {
				SDL_SetRenderDrawColor(mRenderer, 0xFF, 0xFF, 0xFF, 0xFF);

				int imgFlags = IMG_INIT_PNG;
				if (!(IMG_Init(imgFlags) & imgFlags)) {
					printf("SDL_image could not be initialize!");
				}
			}
		}
	}
}


SDL_Window * LWindow::getWindow()
{
	return mWindow;
}

SDL_Renderer * LWindow::getRenderer()
{
	return mRenderer;
}

LWindow::~LWindow()
{
	SDL_DestroyRenderer(mRenderer);
	SDL_DestroyWindow(mWindow);
	mRenderer = NULL;
	mWindow = NULL;

	IMG_Quit();
	SDL_Quit();
}
