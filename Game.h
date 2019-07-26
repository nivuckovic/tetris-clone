#pragma once
#include "LWindow.h"
#include "Block.h"
#include <vector>
#include <string>
#include <stdlib.h>
#include <time.h>

class Game
{
public:
	Game();

	void run();

	void handleInput();
	void update();
	void render();

	void drawGrid();
	void drawBlocks();

	void storageBlock();
	void destroyBlocks();
	void createNewBlock();

	void restart();
	void free();

	bool isOpen();

	~Game();
private:
	bool open;
	void makeWalls();

	int table[20][10];

	Block* mActiveBlock;
	std::vector<Block*> mBlocks;
	std::vector<Block*> mWalls;

	std::vector<Block*> mBlockQueue;
	int queueSize;
	int queueSquarePosY;
	int queueOffset;

	int score;
};

