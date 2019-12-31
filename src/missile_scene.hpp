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

	struct Bullet {
		int x, y;
		int xvel, yvel;
		std::vector<std::pair<int, int>> trail;
	};

	std::vector<Bullet> bullets;
	bool cities[5];
	std::chrono::steady_clock::time_point bulletTimer = std::chrono::steady_clock::now();
	int playerX = 390;
	int playerVelocity = 0;

	void CreateRandomBullet();
};

//TODO: cascading explosions
