#include "missile_scene.hpp"

MissileScene::MissileScene(SDL_Renderer* renderer) {
	background = LoadSprite(renderer, "rsc/background.bmp", {0, 0, 800, 600}, {0, 0, 800, 600});
}

MissileScene::~MissileScene() {
	UnloadSprite(background);
}

void MissileScene::HandleEvent(SDL_Event* event) {
	//
}

void MissileScene::Render(SDL_Renderer* renderer) {
	background->Render(renderer);
}
