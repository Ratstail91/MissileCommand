#pragma once

#include "sprite.hpp"

#include <SDL2/SDL.h>

class MissileScene {
public:
	MissileScene(SDL_Renderer*);
	~MissileScene();

	void HandleEvent(SDL_Event*);
	void Render(SDL_Renderer*);

private:
	Sprite* background = NULL;
};