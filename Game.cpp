// Game.cpp
#include "Game.h"
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <vector>
using namespace std;

void renderPlayer(SDL_Renderer *renderer, SDL_Rect player, int x, int y, int scale, vector<int> tailX, vector<int> tailY, int tailLength)
{
	SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
	player.w = scale;
	player.h = scale;

	// Gets x and y of all tail blocks and renders them
	for (int i = 0; i < tailLength; i++)
	{
		player.x = tailX[i];
		player.y = tailY[i];
		SDL_RenderFillRect(renderer, &player);
	}

	player.x = x;
	player.y = y;

	SDL_RenderFillRect(renderer, &player);
}

void renderFood(SDL_Renderer *renderer, SDL_Rect food)
{
	SDL_SetRenderDrawColor(renderer, 0, 255, 255, 255);
	SDL_RenderFillRect(renderer, &food);
}

void renderScore(SDL_Renderer *renderer, int tailLength, int scale, int wScale)
{
	SDL_Color Black = {0, 0, 0};

	// Get the font used for displaying text
	TTF_Font *font = TTF_OpenFont((char *)"font.ttf", 10);
	if (font == NULL)
	{
		cout << "Font loading error" << endl;
		return;
	}

	SDL_Surface *score = TTF_RenderText_Solid(font, (string("Score: ") + to_string(tailLength * 10)).c_str(), Black);
	SDL_Texture *scoreMessage = SDL_CreateTextureFromSurface(renderer, score);
	SDL_Rect scoreRect;
	scoreRect.w = 100;
	scoreRect.h = 30;
	scoreRect.x = ((scale * wScale) - 50) - (scoreRect.w / 2);
	scoreRect.y = 0;
	SDL_RenderCopy(renderer, scoreMessage, NULL, &scoreRect);

	TTF_CloseFont(font);
}

bool checkCollision(int foodx, int foody, int playerx, int playery)
{

	if (playerx == foodx && playery == foody)
	{
		return true;
	}

	return false;
}

pair<int, int> getFoodSpawn(vector<int> tailX, vector<int> tailY, int playerX, int playerY, int scale, int wScale, int tailLength)
{
	bool valid = false;
	int x = 0;
	int y = 0;
	srand(time(0));
	x = scale * (rand() % wScale);
	y = scale * (rand() % wScale);
	valid = true;

	// Check all tail blocks and player block
	for (int i = 0; i < tailLength; i++)
	{

		if ((x == tailX[i] && y == tailY[i]) || (x == playerX && y == playerY))
		{
			valid = false;
		}
	}

	if (!valid)
	{
		pair<int, int> foodLoc;
		foodLoc = make_pair(-100, -100);
		return foodLoc;
	}

	pair<int, int> foodLoc;
	foodLoc = make_pair(x, y);

	return foodLoc;
}

void gameOver(SDL_Renderer *renderer, SDL_Event event, int scale, int wScale, int tailLength)
{
	SDL_Color Red = {255, 0, 0};
	SDL_Color White = {255, 255, 255};
	SDL_Color Black = {0, 0, 0};

	// Get the font used for displaying text
	TTF_Font *font = TTF_OpenFont((char *)"font.ttf", 10);
	if (font == NULL)
	{
		cout << "Font loading error" << endl;
		return;
	}

	SDL_Surface *gameover = TTF_RenderText_Solid(font, "Game Over", Red);
	SDL_Surface *retry = TTF_RenderText_Solid(font, "Press Enter to retry", Black);
	SDL_Surface *score = TTF_RenderText_Solid(font, (string("Score: ") + to_string(tailLength * 10)).c_str(), Black);
	SDL_Texture *gameoverMessage = SDL_CreateTextureFromSurface(renderer, gameover);
	SDL_Texture *retryMessage = SDL_CreateTextureFromSurface(renderer, retry);
	SDL_Texture *scoreMessage = SDL_CreateTextureFromSurface(renderer, score);
	SDL_Rect gameoverRect;
	SDL_Rect retryRect;
	SDL_Rect scoreRect;
	gameoverRect.w = 200;
	gameoverRect.h = 100;
	gameoverRect.x = ((scale * wScale) / 2) - (gameoverRect.w / 2);
	gameoverRect.y = ((scale * wScale) / 2) - (gameoverRect.h / 2) - 50;
	retryRect.w = 300;
	retryRect.h = 50;
	retryRect.x = ((scale * wScale) / 2) - ((retryRect.w / 2));
	retryRect.y = (((scale * wScale) / 2) - ((retryRect.h / 2)) + 150);
	scoreRect.w = 100;
	scoreRect.h = 30;
	scoreRect.x = ((scale * wScale) / 2) - (scoreRect.w / 2);
	scoreRect.y = 0;
	SDL_RenderCopy(renderer, gameoverMessage, NULL, &gameoverRect);
	SDL_RenderCopy(renderer, retryMessage, NULL, &retryRect);
	SDL_RenderCopy(renderer, scoreMessage, NULL, &scoreRect);

	TTF_CloseFont(font);

	// Show game over screen while space has not been pressed
	while (true)
	{
		SDL_RenderPresent(renderer);

		if (SDL_PollEvent(&event))
		{

			if (event.type == SDL_QUIT)
			{
				exit(0);
			}

			if (event.key.keysym.scancode == SDL_SCANCODE_RETURN)
			{
				return;
			}
		}
	}
}

void youWin(SDL_Renderer *renderer, SDL_Event event, int scale, int wScale, int tailLength)
{
	SDL_Color Red = {255, 0, 0};
	SDL_Color White = {255, 255, 255};
	SDL_Color Black = {0, 0, 0};
	SDL_Color Yellow = {255, 255, 0};

	// Get the font used for displaying text
	TTF_Font *font = TTF_OpenFont((char *)"font.ttf", 10);
	if (font == NULL)
	{
		cout << "Font loading error" << endl;
		return;
	}

	SDL_Surface *gameover = TTF_RenderText_Solid(font, "You won!", Yellow);
	SDL_Surface *retry = TTF_RenderText_Solid(font, "Press Enter to play again", White);
	SDL_Surface *score = TTF_RenderText_Solid(font, (string("Score: ") + to_string(tailLength * 10)).c_str(), Black);
	SDL_Texture *gameoverMessage = SDL_CreateTextureFromSurface(renderer, gameover);
	SDL_Texture *retryMessage = SDL_CreateTextureFromSurface(renderer, retry);
	SDL_Texture *scoreMessage = SDL_CreateTextureFromSurface(renderer, score);
	SDL_Rect gameoverRect;
	SDL_Rect retryRect;
	SDL_Rect scoreRect;
	gameoverRect.w = 200;
	gameoverRect.h = 100;
	gameoverRect.x = ((scale * wScale) / 2) - (gameoverRect.w / 2);
	gameoverRect.y = ((scale * wScale) / 2) - (gameoverRect.h / 2) - 50;
	retryRect.w = 300;
	retryRect.h = 50;
	retryRect.x = ((scale * wScale) / 2) - ((retryRect.w / 2));
	retryRect.y = (((scale * wScale) / 2) - ((retryRect.h / 2)) + 150);
	scoreRect.w = 100;
	scoreRect.h = 30;
	scoreRect.x = ((scale * wScale) / 2) - (scoreRect.w / 2);
	scoreRect.y = 0;
	SDL_RenderCopy(renderer, gameoverMessage, NULL, &gameoverRect);
	SDL_RenderCopy(renderer, retryMessage, NULL, &retryRect);
	SDL_RenderCopy(renderer, scoreMessage, NULL, &scoreRect);

	TTF_CloseFont(font);

	// Show victory screen while space has not been pressed
	while (true)
	{
		SDL_RenderPresent(renderer);

		if (SDL_PollEvent(&event))
		{

			if (event.type == SDL_QUIT)
			{
				exit(0);
			}

			if (event.key.keysym.scancode == SDL_SCANCODE_RETURN)
			{
				return;
			}
		}
	}
}