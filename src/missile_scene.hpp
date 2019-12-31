#pragma once

#include "sprite.hpp"

#include <SDL2/SDL.h>

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

	struct Bullet {
		int x, y;
		int xvel, yvel;
		std::vector<std::pair<int, int>> trail;
	};

	std::vector<Bullet> bullets;

	void CreateRandomBullet();
};
