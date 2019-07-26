#pragma once
#include <SDL.h>

class LTimer
{
public:
	LTimer();

	void start();
	void stop();
	void pause();
	void unpause();

	unsigned int getTicks();

	bool isStarted();
	bool isPaused();
private:
	unsigned int mStartTicks;
	unsigned int mPausedTicks;

	bool mPaused;
	bool mStarted;
};

