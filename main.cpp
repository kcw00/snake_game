#include "Game.h"
#include <SDL.h>
#include <SDL_ttf.h>
#include <iostream>
#include <ctime>

Game::Game()
	: mWindow(nullptr), mRenderer(nullptr), mFont(nullptr),
	  mTailLength(0), mScale(24), mGridSize(24), mX(0), mY(0),
	  mPrevX(0), mPrevY(0), mUp(false), mDown(false),
	  mLeft(false), mRight(false), mInputThisFrame(false),
	  mRedo(false), mIsRunning(true), mTime(0)
{
	mPlayer = {0, 0, mScale, mScale};
	mFood = {0, 0, mScale, mScale};
}

Game::~Game()
{
	Shutdown();
}

bool Game::Initialize()
{
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
		return false;
	if (TTF_Init() < 0)
		return false;

	mFont = TTF_OpenFont("font.ttf", 16);
	if (!mFont)
	{
		std::cerr << "Font load error\n";
		return false;
	}

	mWindow = SDL_CreateWindow("Snake Game",
							   SDL_WINDOWPOS_CENTERED,
							   SDL_WINDOWPOS_CENTERED,
							   mScale * mGridSize,
							   mScale * mGridSize,
							   SDL_WINDOW_SHOWN);
	if (!mWindow)
		return false;

	mRenderer = SDL_CreateRenderer(mWindow, -1, SDL_RENDERER_ACCELERATED);
	if (!mRenderer)
		return false;

	ResetGame();
	return true;
}

void Game::Run()
{
	lastMoveTime = SDL_GetTicks();

	while (mIsRunning)
	{
		mInputThisFrame = false;
		ProcessInput();
		UpdateGame();
		Render();
		SDL_SetRenderDrawColor(mRenderer, 255, 255, 255, 255);
		SDL_RenderClear(mRenderer);
		SDL_Delay(10);
	}
}

void Game::Shutdown()
{
	if (mFont)
		TTF_CloseFont(mFont);
	if (mRenderer)
		SDL_DestroyRenderer(mRenderer);
	if (mWindow)
		SDL_DestroyWindow(mWindow);
	TTF_Quit();
	SDL_Quit();
}

void Game::ProcessInput()
{
	while (SDL_PollEvent(&mEvent))
	{
		// Simply exit the program when told to
		if (mEvent.type == SDL_QUIT)
			mIsRunning = false;
		// If a key is pressed
		if (mEvent.type == SDL_KEYDOWN && !mInputThisFrame)
		{
			switch (mEvent.key.keysym.scancode)
			{
			case SDL_SCANCODE_UP:
				if (!mDown)
				{
					mUp = true;
					mLeft = mRight = mDown = false;
				}
				break;
			case SDL_SCANCODE_DOWN:
				if (!mUp)
				{
					mDown = true;
					mLeft = mRight = mUp = false;
				}
				break;
			case SDL_SCANCODE_LEFT:
				if (!mRight)
				{
					mLeft = true;
					mUp = mDown = mRight = false;
				}
				break;
			case SDL_SCANCODE_RIGHT:
				if (!mLeft)
				{
					mRight = true;
					mUp = mDown = mLeft = false;
				}
				break;
			default:
				break;
			}
			mInputThisFrame = true;
		}
	}
}

void Game::UpdateGame()
{
	Uint32 now = SDL_GetTicks();
	if (now - lastMoveTime < moveDelay)
		return;
	lastMoveTime = now;
	// The previous position of the player block
	mPrevX = mX;
	mPrevY = mY;

	if (mUp)
		mY -= mScale;
	else if (mDown)
		mY += mScale;
	else if (mLeft)
		mX -= mScale;
	else if (mRight)
		mX += mScale;

	if (CheckCollision(mFood.x, mFood.y, mX, mY))
	{
		mTailLength++;
		mFood = {GetFoodSpawn().first, GetFoodSpawn().second, mScale, mScale};
	}

	if (mTailX.size() != mTailLength)
	{
		mTailX.push_back(mPrevX);
		mTailY.push_back(mPrevY);
	}

	for (int i = 0; i < mTailLength - 1; ++i)
	{
		mTailX[i] = mTailX[i + 1];
		mTailY[i] = mTailY[i + 1];
	}

	if (mTailLength > 0)
	{
		mTailX[mTailLength - 1] = mPrevX;
		mTailY[mTailLength - 1] = mPrevY;
	}

	for (int i = 0; i < mTailLength; ++i)
		if (mX == mTailX[i] && mY == mTailY[i])
			GameOverScreen();

	if (mX < 0 || mY < 0 || mX >= mScale * mGridSize || mY >= mScale * mGridSize)
		GameOverScreen();

	if (mTailLength >= 575)
		YouWinScreen();
}

void Game::Render()
{
	RenderFood();
	RenderPlayer();
	RenderScore();

	SDL_RenderDrawLine(mRenderer, 0, 0, 0, mScale * mGridSize);
	SDL_RenderDrawLine(mRenderer, 0, mScale * mGridSize, mScale * mGridSize, mScale * mGridSize);
	SDL_RenderDrawLine(mRenderer, mScale * mGridSize, mScale * mGridSize, mScale * mGridSize, 0);
	SDL_RenderDrawLine(mRenderer, mScale * mGridSize, 0, 0, 0);

	SDL_RenderPresent(mRenderer);
}

void Game::RenderPlayer()
{
	SDL_SetRenderDrawColor(mRenderer, 0, 0, 255, 255);
	mPlayer.x = mX;
	mPlayer.y = mY;
	SDL_RenderFillRect(mRenderer, &mPlayer);
	for (int i = 0; i < mTailLength; ++i)
	{
		mPlayer.x = mTailX[i];
		mPlayer.y = mTailY[i];
		SDL_RenderFillRect(mRenderer, &mPlayer);
	}
}

void Game::RenderFood()
{
	SDL_SetRenderDrawColor(mRenderer, 0, 255, 255, 255);
	SDL_RenderFillRect(mRenderer, &mFood);
}

void Game::RenderScore()
{
	SDL_Color black = {0, 0, 0};
	std::string scoreText = "Score: " + std::to_string(mTailLength * 10);
	SDL_Surface *surface = TTF_RenderText_Solid(mFont, scoreText.c_str(), black);
	SDL_Texture *texture = SDL_CreateTextureFromSurface(mRenderer, surface);
	SDL_Rect rect = {10, 10, 100, 30};
	SDL_RenderCopy(mRenderer, texture, NULL, &rect);
	SDL_FreeSurface(surface);
	SDL_DestroyTexture(texture);
}

void Game::GameOverScreen()
{
	SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "Game Over", "You collided. Game Over.", mWindow);
	ResetGame();
}

void Game::YouWinScreen()
{
	SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "You Win", "Congratulations!", mWindow);
	ResetGame();
}

void Game::ResetGame()
{
	mTailLength = 0;
	mTailX.clear();
	mTailY.clear();
	mX = mY = 0;
	mUp = mDown = mLeft = mRight = false;
	mFood = {GetFoodSpawn().first, GetFoodSpawn().second, mScale, mScale};
}

bool Game::CheckCollision(int x1, int y1, int x2, int y2)
{
	return (x1 == x2 && y1 == y2);
}

std::pair<int, int> Game::GetFoodSpawn()
{
	srand(static_cast<unsigned>(time(0)));
	int x = mScale * (rand() % mGridSize);
	int y = mScale * (rand() % mGridSize);
	return {x, y};
}

int main(int argc, char *argv[])
{
	Game game;
	if (game.Initialize())
	{
		game.Run();
	}
	return 0;
}
