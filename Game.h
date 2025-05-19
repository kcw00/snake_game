#pragma once

#include <SDL.h>
#include <SDL_ttf.h>
#include <vector>
#include <string>

class Game
{
public:
	Game();
	~Game();
	bool Initialize();
	void Run();
	void Shutdown();

private:
	void ProcessInput();
	void UpdateGame();
	void Render();
	void ResetGame();
	void RenderPlayer();
	void RenderFood();
	void RenderScore();
	void GameOverScreen();
	void YouWinScreen();
	bool CheckCollision(int x1, int y1, int x2, int y2);
	std::pair<int, int> GetFoodSpawn();

	SDL_Window *mWindow;
	SDL_Renderer *mRenderer;
	SDL_Event mEvent;
	TTF_Font *mFont;

	SDL_Rect mPlayer;
	SDL_Rect mFood;

	int mTailLength;
	std::vector<int> mTailX;
	std::vector<int> mTailY;

	int mScale;
	int mGridSize;

	int mX, mY;
	int mPrevX, mPrevY;

	bool mUp, mDown, mLeft, mRight;
	bool mInputThisFrame;
	bool mRedo;
	bool mIsRunning;

	int mTime;
	Uint32 lastMoveTime;
	Uint32 moveDelay = 150;
};
