#include "missile_scene.hpp"

#include <algorithm>
#include <cstdlib>

MissileScene::MissileScene(SDL_Renderer* renderer) {
	background = LoadSprite(renderer, "rsc/background.bmp", {0, 0, 800, 600}, {0, 0, 800, 600});
	dot = LoadSprite(renderer, "rsc/dot.bmp", {0, 0, 8, 8}, {0, 0, 8, 8});

	//create a couple of bullets
	for (int i = 0; i < 3; i++) {
		CreateRandomBullet();
	}
}

MissileScene::~MissileScene() {
	UnloadSprite(background);
	UnloadSprite(dot);
}

void MissileScene::HandleEvent(SDL_Event* event) {
	//
}

void MissileScene::Update() {
	std::for_each(bullets.begin(), bullets.end(), [](Bullet& b) {
		b.x += b.xvel;
		b.y += b.yvel;
		b.trail.push_back({b.x, b.y});
	});
}

void MissileScene::Render(SDL_Renderer* renderer) {
	background->Render(renderer);

	//for each bullet
	std::for_each(bullets.begin(), bullets.end(), [&](Bullet b) {
		//for each element in the trail
		std::for_each(b.trail.begin(), b.trail.end(), [&](std::pair<int, int> p) {
			SDL_Rect rect = {p.first, p.second, 4, 4};
			dot->SetDstRect(rect);
			dot->Render(renderer);
		});
	});
}

void MissileScene::CreateRandomBullet() {
	Bullet bullet;

	bullet.x = rand() % 800;
	bullet.y = 0;
	bullet.xvel = rand() % 4 - 2;
	bullet.yvel = 4; //difficulty

	bullets.push_back(bullet);
}