#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include <iostream>

class LWindow
{
public:
	LWindow();

	static const int SCREEN_HEIGHT;
	static const int SCREEN_WIDTH;

	static SDL_Window* getWindow();
	static SDL_Renderer* getRenderer();

	~LWindow();
private:
	static SDL_Window* mWindow;
	static SDL_Renderer* mRenderer;
	
};

