#include "Game.h"
#include <algorithm>
#include <set>

Game::Game() : open(true), mActiveBlock(NULL), queueSize(2), queueSquarePosY(96), queueOffset(3), score(0)
{
	Block::mBlocks = &mBlocks;
	Block::mWalls = &mWalls;

	makeWalls();

	createNewBlock();
}

void Game::run()
{
	while (isOpen()) {
		handleInput();
		update();
		render();
	}
}

void Game::handleInput()
{
	SDL_Event e;

	while (SDL_PollEvent(&e) != 0) {
		if (e.type == SDL_QUIT) {
			open = false;
		}
		mActiveBlock->handleEvent(e);
	}
}

void Game::update()
{
	bool blockHit = mActiveBlock->move();
	
	if (blockHit) {
		if (!Block::moved) {
			restart();
			return;
		}
		storageBlock();
		destroyBlocks();
		createNewBlock();
	}
}

void Game::render()
{
	SDL_SetRenderDrawColor(LWindow::getRenderer(), 0x00, 0x00, 0x00, 0xFF);
	SDL_RenderClear(LWindow::getRenderer());

	//render grid
	
	drawGrid();
	drawBlocks();

	SDL_RenderPresent(LWindow::getRenderer());
}

void Game::drawGrid()
{
	SDL_SetRenderDrawColor(LWindow::getRenderer(), 0x1c, 0x12, 0x1a, 0xFF);
	SDL_Rect gridRect = { 0, 0, 32, 32 };
	for (int i = 0; i < 320 / 32; ++i) {
		for (int j = 0; j < 20; ++j) {
			gridRect.x = i * 32;
			gridRect.y = j * 32;

			SDL_RenderDrawRect(LWindow::getRenderer(), &gridRect);
		}
	}
	
	SDL_SetRenderDrawColor(LWindow::getRenderer(), 0x00, 0xFF, 0x00, 0xFF);

	SDL_Rect gameRect = { 1, 1, 319, 639 };
	SDL_RenderDrawRect(LWindow::getRenderer(), &gameRect);

	SDL_Rect queueRect = { 352, queueSquarePosY, 4 * 32, queueSize * queueOffset * 32 };
	SDL_RenderDrawRect(LWindow::getRenderer(), &queueRect);

	//debug rectangle
	/*int size = std::max(mActiveBlock->getBlockHeight(), mActiveBlock->getBlockWidth());
	SDL_Rect debugBox = { mActiveBlock->getPosX(), mActiveBlock->getPosY(), size, size };
	SDL_SetRenderDrawColor(LWindow::getRenderer(), 0x00, 0xFF, 0x00, 0xFF);
	SDL_RenderDrawRect(LWindow::getRenderer(), &debugBox);
	*/
}

void Game::drawBlocks()
{
	for (int i = 0; i < mBlockQueue.size(); ++i) {
		mBlockQueue[i]->render();
	}

	for (int i = 0; i < mBlocks.size(); ++i) {
		mBlocks[i]->render();
	}

	mActiveBlock->render();
}

void Game::storageBlock()
{
	if (mActiveBlock != NULL) {
		for (int i = 0; i < mActiveBlock->getCollider()->size(); ++i) {
			int x = mActiveBlock->getCollider()->at(i)->x;
			int y = mActiveBlock->getCollider()->at(i)->y;

			mBlocks.push_back(new Block(x, y, mActiveBlock->getType()));
		}
	}
}

void Game::destroyBlocks()
{
	std::set<int> rows;
	std::vector<SDL_Rect*> *activeBlockCollider = mActiveBlock->getCollider();


	for (int i = 0; i < activeBlockCollider->size(); ++i) {
		int width = 0;

		for (int j = 0; j < mBlocks.size(); ++j) {
			if (activeBlockCollider->at(i)->y == mBlocks[j]->getPosY()) {
				width += 32;
			}
		}

		if (width == 320) {
			rows.insert(activeBlockCollider->at(i)->y);
		}
	}

	score += rows.size();

	for (std::set<int>::iterator jt = rows.begin(); jt != rows.end(); ++jt) {
		for (std::vector<Block *>::iterator it = mBlocks.begin(); it != mBlocks.end();) {
			if (*jt == (*it)->getPosY()) {
				it = mBlocks.erase(it);
				continue;
			}
			else {
				if ((*it)->getPosY() < *jt) {
					(*it)->move(0, 32, true);
				}

				++it;
			}
		}
	}
}

void Game::createNewBlock()
{
	if (mActiveBlock == NULL) {
		for (int i = 0; i < queueSize; ++i) {
			mBlockQueue.insert(mBlockQueue.begin(), new Block());
			mBlockQueue[0]->move(352 , (queueSquarePosY + i * queueOffset * 32) , true);
		}

		mActiveBlock = new Block();
		mActiveBlock->move(96, 0, true);
		mActiveBlock->setVelY(32);

	}
	else {
		mActiveBlock = mBlockQueue[mBlockQueue.size() - 1];
		mBlockQueue.pop_back();
		mActiveBlock->move(-256, -queueSquarePosY, true);
		mActiveBlock->setVelY(32);

		for (int i = 0; i < mBlockQueue.size(); ++i) {
			mBlockQueue[i]->move(0, -queueOffset * 32, true);
		}

		mBlockQueue.insert(mBlockQueue.begin(), new Block());
		mBlockQueue[0]->move(352, queueSquarePosY + (queueOffset * 32 * (mBlockQueue.size() -1)), true);
	}
}

void Game::restart()
{
	free();

	Block::mBlocks = &mBlocks;
	Block::mWalls = &mWalls;

	makeWalls();

	createNewBlock();
}

void Game::free()
{
	delete mActiveBlock;
	mActiveBlock = NULL;

	for (int i = 0; i < mBlocks.size(); ++i) {
		delete mBlocks[i];
		mBlocks[i] = NULL;
	}
	mBlocks.clear();


	for (int i = 0; i < mBlockQueue.size(); ++i) {
		delete mBlockQueue[i];
		mBlockQueue[i] = NULL;
	}
	mBlockQueue.clear();

	for (int i = 0; i < mWalls.size(); ++i) {
		delete mWalls[i];
		mWalls[i] = NULL;
	}
	mWalls.clear();
}

bool Game::isOpen()
{
	return open;
}

Game::~Game()
{
	free();
}

void Game::makeWalls()
{
	for (int i = 0; i < 20; ++i) {
		mWalls.push_back(new Block(-32, i * 32));
		mWalls.push_back(new Block(320, i * 32));
	}

	for (int i = 0; i < 10; ++i) {
		mWalls.push_back(new Block(i * 32, 640));
		mWalls.push_back(new Block(i * 32, -32));
	}
}
