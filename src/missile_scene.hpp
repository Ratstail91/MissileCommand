#pragma once

#include "sprite.hpp"

#include <SDL2/SDL.h>

#include <chrono>
#include <vector>

class MissileScene {
public:
	MissileScene(SDL_Renderer*);
	~MissileScene();

	void HandleEvent(SDL_Event*);
	void Update();
	void Render(SDL_Renderer*);

private:
	Sprite* background = NULL;
	Sprite* dot = NULL;
	Sprite* city = NULL;
	Sprite* player = NULL;
	Sprite* explosion1 = NULL;
	Sprite* explosion2 = NULL;
	Sprite* gameOver = NULL;

	struct Bullet {
		int x, y;
		int xvel, yvel;
		std::vector<std::pair<int, int>> trail;
	};

	struct Explosion {
		int x, y;
		//assume radius is 64
		std::chrono::steady_clock::time_point creationTime;

		Explosion() {
			creationTime = std::chrono::steady_clock::now();
		}
	};

	std::vector<Bullet> bullets;
	std::vector<Explosion> explosions;
	bool cities[5];
	std::chrono::steady_clock::time_point bulletTimer = std::chrono::steady_clock::now();
	int playerX = 390;
	int playerVelocity = 0;
	bool playerAlive = true;

	void CreateRandomBullet();
	void CreateExplosion(int x, int y);
	bool IsGameOver();
};
