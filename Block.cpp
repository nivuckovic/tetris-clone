#include "Block.h"
#include <math.h>
#include <algorithm>

LTimer Block::moveYTimer;
unsigned int Block::moveYCooldown = 1000;

LTimer Block::moveXTimer;
unsigned int Block::moveXCooldown = 80;

std::vector<Block*>* Block::mBlocks = NULL;
std::vector<Block*>* Block::mWalls = NULL;

std::vector<std::vector<Pair *>*> Block::wallKickTable;
std::vector<std::vector<Pair *>*> Block::wallKickTableI;

std::string Block::types = "ijlostz";
LTexture Block::mBlock;

int Block::moved = 0;

Block::Block()
{
	//ako je prvi put pokrenuto
	if (!wallKickTable.size()) {
		for (int i = 0; i < 4; ++i) {
			wallKickTable.push_back(new std::vector<Pair*>());
			wallKickTableI.push_back(new std::vector<Pair*>());
		}

		//za J,L,S,T,Z
		//0 -> L
		wallKickTable[0]->push_back(new Pair(0, 0));
		wallKickTable[0]->push_back(new Pair(-1, 0));
		wallKickTable[0]->push_back(new Pair(-1, -1));
		wallKickTable[0]->push_back(new Pair(0, 2));
		wallKickTable[0]->push_back(new Pair(-1, 2));

		//R->2
		wallKickTable[1]->push_back(new Pair(0, 0));
		wallKickTable[1]->push_back(new Pair(1, 0));
		wallKickTable[1]->push_back(new Pair(1, 1));
		wallKickTable[1]->push_back(new Pair(0, -2));
		wallKickTable[1]->push_back(new Pair(1, -2));

		// 2 -> L ovaj
		wallKickTable[2]->push_back(new Pair(0, 0));
		wallKickTable[2]->push_back(new Pair(1, 0));
		wallKickTable[2]->push_back(new Pair(1, -1));
		wallKickTable[2]->push_back(new Pair(0, 2));
		wallKickTable[2]->push_back(new Pair(1, 2));

		//L -> 0
		wallKickTable[3]->push_back(new Pair(0, 0));
		wallKickTable[3]->push_back(new Pair(-1, 0));
		wallKickTable[3]->push_back(new Pair(-1, 1));
		wallKickTable[3]->push_back(new Pair(0, -2));
		wallKickTable[3]->push_back(new Pair(-1, -2));

		//za I
		//0 -> L
		wallKickTableI[0]->push_back(new Pair(0, 0));
		wallKickTableI[0]->push_back(new Pair(-2, 0));
		wallKickTableI[0]->push_back(new Pair(1, 0));
		wallKickTableI[0]->push_back(new Pair(-2, 1));
		wallKickTableI[0]->push_back(new Pair(1, -2));

		//L -> 2
		wallKickTableI[1]->push_back(new Pair(0, 0));
		wallKickTableI[1]->push_back(new Pair(-1, 0));
		wallKickTableI[1]->push_back(new Pair(2, 0));
		wallKickTableI[1]->push_back(new Pair(-1, -2));
		wallKickTableI[1]->push_back(new Pair(2, 1));

		//2 -> R 
		wallKickTableI[2]->push_back(new Pair(0, 0));
		wallKickTableI[2]->push_back(new Pair(2, 0));
		wallKickTableI[2]->push_back(new Pair(-1, 0));
		wallKickTableI[2]->push_back(new Pair(2, -1));
		wallKickTableI[2]->push_back(new Pair(-1, 2));

		//R -> 0
		wallKickTableI[3]->push_back(new Pair(0, 0));
		wallKickTableI[3]->push_back(new Pair(1, 0));
		wallKickTableI[3]->push_back(new Pair(-2, 0));
		wallKickTableI[3]->push_back(new Pair(1, 2));
		wallKickTableI[3]->push_back(new Pair(-2, 1));

		mBlock.loadFromFile("blocks.png");
	}

	int randNumber = rand() % 7;
	
	clip = { randNumber * 32, 0, 32, 32 };

	this->type = types[randNumber];

	mPosX = 0;
	mPosY = 0;

	mVelX = 0;
	mVelY = 0;

	for (int i = 0; i < 4; ++i) {
		mCollider.push_back(new SDL_Rect());
		mCollider[i]->h = 32;
		mCollider[i]->w = 32;
	}

	switch (type) {
		case 'o': {
			flips = 0;

			blockHeight = 64;
			blockWidth = 64;

			mCollider[0]->x = mPosX;
			mCollider[0]->y = mPosY;
			
			mCollider[1]->x = mPosX + 32;
			mCollider[1]->y = mPosY;
			
			mCollider[2]->x = mPosX;
			mCollider[2]->y = mPosY + 32;

			mCollider[3]->x = mPosX + 32;
			mCollider[3]->y = mPosY + 32;
			break;
		}
		case 'j': {
			flips = 3;

			blockHeight = 64;
			blockWidth = 96;

			mCollider[0]->x = mPosX;
			mCollider[0]->y = mPosY;

			mCollider[1]->x = mPosX + 32;
			mCollider[1]->y = mPosY;

			mCollider[2]->x = mPosX + 64;
			mCollider[2]->y = mPosY;

			mCollider[3]->x = mPosX + 64;
			mCollider[3]->y = mPosY + 32;

			mPosY -= 32;
			break;
		}
		case 'i': {
			flips = 1;

			blockHeight = 32;
			blockWidth = 128;

			mCollider[0]->x = mPosX;
			mCollider[0]->y = mPosY;

			mCollider[1]->x = mPosX + 32;
			mCollider[1]->y = mPosY;

			mCollider[2]->x = mPosX + 64;
			mCollider[2]->y = mPosY;

			mCollider[3]->x = mPosX + 96;
			mCollider[3]->y = mPosY;

			mPosY -= 32;
			break;
		}
		case 'l': {
			flips = 3;

			blockHeight = 64;
			blockWidth = 96;

			mCollider[0]->x = mPosX;
			mCollider[0]->y = mPosY;

			mCollider[1]->x = mPosX + 32;
			mCollider[1]->y = mPosY;

			mCollider[2]->x = mPosX + 64;
			mCollider[2]->y = mPosY;

			mCollider[3]->x = mPosX;
			mCollider[3]->y = mPosY + 32;

			mPosY -= 32;
			break;
		}
		case 's': {
			flips = 1;

			blockHeight = 64;
			blockWidth = 96;

			mCollider[0]->x = mPosX;
			mCollider[0]->y = mPosY + 32;

			mCollider[1]->x = mPosX + 32;
			mCollider[1]->y = mPosY + 32;

			mCollider[2]->x = mPosX + 32;
			mCollider[2]->y = mPosY;

			mCollider[3]->x = mPosX + 64;
			mCollider[3]->y = mPosY;
			break;
		}
		case 't': {
			flips = 3;

			blockHeight = 64;
			blockWidth = 96;

			mCollider[0]->x = mPosX;
			mCollider[0]->y = mPosY;

			mCollider[1]->x = mPosX + 32;
			mCollider[1]->y = mPosY;

			mCollider[2]->x = mPosX + 64;
			mCollider[2]->y = mPosY;

			mCollider[3]->x = mPosX + 32;
			mCollider[3]->y = mPosY + 32;

			mPosY -= 32;
			break;
		}
		case 'z': {
			flips = 1;

			blockHeight = 64;
			blockWidth = 96;

			mCollider[0]->x = mPosX;
			mCollider[0]->y = mPosY;

			mCollider[1]->x = mPosX + 32;
			mCollider[1]->y = mPosY;

			mCollider[2]->x = mPosX + 32;
			mCollider[2]->y = mPosY + 32;

			mCollider[3]->x = mPosX + 64;
			mCollider[3]->y = mPosY + 32;
			break;
		}
	}

	moveXTimer.start();
	moveYTimer.start();

	moved = 0;
}

Block::Block(int x, int y, char type)
{
	this->type = type;

	mPosX = x;
	mPosY = y;

	int clipIndex = types.find(type);
	clip = { clipIndex * 32, 0, 32, 32 };

	mCollider.push_back(new SDL_Rect());
	mCollider[0]->x = x;
	mCollider[0]->y = y;
	mCollider[0]->h = 32;
	mCollider[0]->w = 32;

}


Block::~Block()
{
	for (int i = 0; i < mCollider.size(); ++i) {
		delete mCollider[i];
		mCollider[i] = NULL;
	}
	mCollider.clear();
	
	mBlocks = NULL;
	mWalls = NULL;
}

void Block::handleEvent(SDL_Event & e)
{
	if (e.type == SDL_KEYDOWN && e.key.repeat == 0) {
		switch (e.key.keysym.sym) {
			case SDLK_LEFT: mVelX -= 32; isPressed = true; break;
			case SDLK_RIGHT: mVelX += 32; isPressed = true; break;
			case SDLK_DOWN: moveYCooldown = 50; break;
			case SDLK_UP: rotate(); break;
		}
	}
	else if(e.type == SDL_KEYUP && e.key.repeat == 0){
		switch (e.key.keysym.sym) {
			case SDLK_LEFT: if(isPressed) mVelX += 32; break;
			case SDLK_RIGHT: if(isPressed) mVelX -= 32; break;
			case SDLK_DOWN: moveYCooldown = 1000; break;
		}
	}
}

bool Block::move(int x, int y, bool force)
{
	bool collided = false;

	if (moveXTimer.getTicks() >= moveXCooldown || force) {
		if (!force) {
			moveXTimer.start();
		}

		mPosX += mVelX + x;
		moveCollider(mVelX + x, 0);
		if (!force && (mVelX)) {
			if (checkCollision()) {
				mPosX -= mVelX;
				moveCollider(-mVelX, 0);
			}
		}
	}

	if (moveYTimer.getTicks() >= moveYCooldown || force) {

		mPosY += mVelY + y;
		moveCollider(0, mVelY + y);

		if (!force) {
			moveYTimer.start();
			if (checkCollision()) {
				mPosY -= mVelY;
				moveCollider(0, -mVelY);
				collided = true;
			}
			else {
				++moved;
			}
		}
	}

	return collided;
}

void Block::render()
{
	for (int i = 0; i < mCollider.size(); ++i) {
		mBlock.render(mCollider[i]->x, mCollider[i]->y, &clip);
	}
}

char Block::getType()
{
	return type;
}

int Block::getPosX()
{
	return mPosX;
}

int Block::getPosY()
{
	return mPosY;
}

int Block::getBlockHeight()
{
	return blockHeight;
}

int Block::getBlockWidth()
{
	return blockWidth;
}

void Block::setVelY(int y)
{
	mVelY = y;
}

std::vector<SDL_Rect*>* Block::getCollider()
{
	return &mCollider;
}

void Block::rotate()
{
	if (type == 'o') {
		return;
	}

	int h, w;
	h = w = std::max(blockHeight, blockWidth);

	int pivotX, pivotY;

	pivotX = (mPosX + (mPosX + w)) / 2;
	pivotY = (mPosY + (mPosY + h)) / 2;

	int offsetX, offsetY;
	int middleX, middleY;
	int newMiddleX, newMiddleY;

	Pair oldColliderPos[4];

	for (int i = 0; i < mCollider.size(); ++i) {
		oldColliderPos[i].x = mCollider[i]->x;
		oldColliderPos[i].y = mCollider[i]->y;

		//printf("X: %d Y: %d\n", mCollider[i]->x, mCollider[i]->y);

		middleX = (mCollider[i]->x + (mCollider[i]->x + mCollider[i]->w)) / 2;
		middleY = (mCollider[i]->y + (mCollider[i]->y + mCollider[i]->h)) / 2;

		//printf("middleX: %d middleY: %d\n", middleX, middleY);

		offsetX = std::abs(middleX - mCollider[i]->x);
		offsetY = std::abs(middleY - mCollider[i]->y);

		//printf("offsetX: %d offsetY: %d\n", offsetX, offsetY);

		newMiddleX = middleY - pivotY;
		newMiddleY = pivotX - middleX;

		//printf("rotated middleX: %d middleY: %d\n", newMiddleX + pivotX, newMiddleY + pivotY);

		mCollider[i]->x = (newMiddleX + pivotX) - offsetX;
		mCollider[i]->y = (newMiddleY + pivotY) - offsetY;

		//printf("rotated X: %d Y: %d\n\n", mCollider[i]->x, mCollider[i]->y);
	}

	flips = (++flips) % 4;
	std::swap(blockHeight, blockWidth);

	int oldVelX = mVelX;
	int oldVelY = mVelY;

	mVelX = 0;
	mVelY = 0;

	bool invalidMove = true;
	
	int cnt = 0;
	for (int i = 0; i < 5; ++i) {
		++cnt;
		if (type == 'i') {
			move(wallKickTableI[flips]->at(i)->x * 32, wallKickTableI[flips]->at(i)->y * 32, true);
		}
		else {
			move(wallKickTable[flips]->at(i)->x * 32, wallKickTable[flips]->at(i)->y * 32, true);
		}

		//debug
		//printf("flips: %d X: %d Y: %d\n", flips, wallKickTable[flips]->at(i)->x, wallKickTable[flips]->at(i)->y);
		/*if (i != 0) {
			SDL_RenderClear(LWindow::getRenderer());
			for (int i = 0; i < mBlocks->size(); ++i) {
				mBlocks->at(i)->render();
			}

			render();
			SDL_RenderPresent(LWindow::getRenderer());
			
			SDL_Delay(2000);
		}*/

		if (!checkCollision()) {
			invalidMove = false;
			break;
		}
		else {
			if (type == 'i') {
				move(-wallKickTableI[flips]->at(i)->x * 32, -wallKickTableI[flips]->at(i)->y * 32, true);
			}
			else {
				move(-wallKickTable[flips]->at(i)->x * 32, -wallKickTable[flips]->at(i)->y * 32, true);
			}
		}
	}

	if (invalidMove) {
		for (int i = 0; i < mCollider.size(); ++i) {
			mCollider[i]->x = oldColliderPos[i].x;
			mCollider[i]->y = oldColliderPos[i].y;
		}
		std::swap(blockHeight, blockWidth);
		
		--flips;
		if (flips == -1) {
			flips = 3;
		}
	}

	std::swap(oldVelX, mVelX);
	std::swap(oldVelY, mVelY);
}

void Block::moveCollider(int velX, int velY)
{
	for (int i = 0; i < mCollider.size(); ++i) {
		mCollider[i]->x += velX;
		mCollider[i]->y += velY;
	}
}


bool Block::checkCollision()
{
	bool sol = false;

	std::vector<Block*> allBlocks;
	for (int i = 0; i < mBlocks->size(); ++i) {
		allBlocks.push_back((*mBlocks)[i]);
	}
	for (int i = 0; i < mWalls->size(); ++i) {
		allBlocks.push_back((*mWalls)[i]);
	}

	SDL_Rect a, b;

	int leftA, leftB;
	int rightA, rightB;
	int topA, topB;
	int bottomA, bottomB;

	for (int i = 0; i < allBlocks.size(); ++i) {
		for (int j = 0; j < allBlocks[i]->getCollider()->size(); ++j) {
			for (int k = 0; k < 4; ++k) {
				bool collided = true;
				
				a = *(*allBlocks[i]->getCollider())[j];
				b = *mCollider[k];

				leftA = a.x;
				rightA = a.x + a.w;
				topA = a.y;
				bottomA = a.y + a.h;

				//Calculate the sides of rect B
				leftB = b.x;
				rightB = b.x + b.w;
				topB = b.y;
				bottomB = b.y + b.h;

				if (bottomA <= topB)
				{
					collided = false;
				}

				if (topA >= bottomB)
				{
					collided = false;
				}

				if (rightA <= leftB)
				{
					collided = false;
				}

				if (leftA >= rightB)
				{
					collided = false;
				}

				sol = sol || collided;
			}
		}
	}
	
	for (int i = 0; i < allBlocks.size(); ++i) {
		allBlocks[i] = NULL;
	}
	allBlocks.clear();
	
	return sol;
}
