#include "sprite.hpp"

#include <iostream>

Sprite::Sprite() {
	//
}

Sprite::~Sprite() {
	//
}

void Sprite::Render(SDL_Renderer* renderer) {
	SDL_RenderCopy(renderer, texture, &srcRect, &dstRect);
}

SDL_Rect Sprite::GetSrcRect() {
	return srcRect;
}

SDL_Rect Sprite::GetDstRect() {
	return dstRect;
}

void Sprite::SetSrcRect(SDL_Rect r) {
	srcRect = r;
}

void Sprite::SetDstRect(SDL_Rect r) {
	dstRect = r;
}

Sprite* LoadSprite(SDL_Renderer* renderer, std::string fname, SDL_Rect src, SDL_Rect dst) {
	//load the sprite from a bitmap
	SDL_Surface* surface = SDL_LoadBMP(fname.c_str());

	if (!surface) {
		std::cerr << "Failed to load a bitmap: " << SDL_GetError() << std::endl;
		return NULL;
	}

	//set the colorkey
	SDL_SetColorKey(surface, SDL_TRUE, SDL_MapRGB(surface->format, 255, 0, 255));

	//create the texture
	SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);

	if (!texture) {
		std::cerr << "Failed to create a texture: " << SDL_GetError() << std::endl;
		return NULL;
	}

	//create the sprite on the heap
	Sprite* sprite = new Sprite();

	sprite->texture = texture;
	sprite->srcRect = src;
	sprite->dstRect = dst;

	//finally
	return sprite;
}

void UnloadSprite(Sprite* sprite) {
	SDL_DestroyTexture(sprite->texture);
}