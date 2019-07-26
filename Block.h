#pragma once
#include "LWindow.h"
#include "LTexture.h"
#include "LTimer.h"
#include <string>
#include <vector>
#include <time.h>

struct Pair {
	Pair() {
		x = 0;
		y = 0;
	}
	Pair(int x, int y) {
		this->x = x;
		this->y = y;
	}

	int x, y;
};

class Block
{
public:
	Block();
	Block(int x, int y, char type = 'i');
	~Block();

	void handleEvent(SDL_Event& e);

	bool move(int x = 0, int y = 0, bool force = false);

	void render();

	char getType();

	int getPosX();
	int getPosY();

	int getBlockHeight();
	int getBlockWidth();

	void setVelY(int y);

	std::vector<SDL_Rect*>* getCollider();

	static int moved;

	static std::vector<Block*> *mBlocks;
	static std::vector<Block*> *mWalls;
private:
	static LTexture mBlock;
	SDL_Rect clip;

	char type;
	int mPosX, mPosY;
	int mVelX, mVelY;

	static std::string types;

	int blockHeight, blockWidth;

	void rotate();
	void moveCollider(int velX, int velY);

	std::vector<SDL_Rect*> mCollider;
	bool checkCollision();

	static LTimer moveYTimer;
	static unsigned int moveYCooldown;

	static LTimer moveXTimer;
	static unsigned int moveXCooldown;

	static std::vector<std::vector<Pair*>* > wallKickTable;
	static std::vector<std::vector<Pair*>* > wallKickTableI;

	bool isPressed;
	// 0, 2 horizontal, 1, 3 vertical
	int flips;
	//test
};

