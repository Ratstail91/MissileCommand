#pragma once

#include <SDL2/SDL.h>

#include <string>

class Sprite {
public:
	Sprite();
	~Sprite();

	void Render(SDL_Renderer*);

	SDL_Rect GetSrcRect();
	SDL_Rect GetDstRect();
	void SetSrcRect(SDL_Rect);
	void SetDstRect(SDL_Rect);

private:
	SDL_Texture* texture = NULL;
	SDL_Rect srcRect;
	SDL_Rect dstRect;

	friend Sprite* LoadSprite(SDL_Renderer*, std::string fname, SDL_Rect src, SDL_Rect dst);
	friend void UnloadSprite(Sprite*);
};

Sprite* LoadSprite(SDL_Renderer*, std::string fname, SDL_Rect src, SDL_Rect dst);
void UnloadSprite(Sprite*);
