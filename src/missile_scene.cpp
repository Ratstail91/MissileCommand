#include "missile_scene.hpp"

#include <algorithm>
#include <cstdlib>

//utility functions
inline int clamp(int v, int lo, int hi) {
	if (v < lo) return lo;
	if (v > hi) return hi;
	return v;
}

MissileScene::MissileScene(SDL_Renderer* renderer) {
	background = LoadSprite(renderer, "rsc/background.bmp", {0, 0, 800, 600}, {0, 0, 800, 600});
	dot = LoadSprite(renderer, "rsc/dot.bmp", {0, 0, 8, 8}, {0, 0, 8, 8});
	city = LoadSprite(renderer, "rsc/city.bmp", {0, 0, 160, 50}, {0, 0, 160, 50});
	player = LoadSprite(renderer, "rsc/player.bmp", {0, 0, 20, 20}, {0, 0, 20, 20});

	for(int i = 0; i < 5; i++) {
		cities[i] = true;
	}
}

MissileScene::~MissileScene() {
	UnloadSprite(background);
	UnloadSprite(dot);
	UnloadSprite(city);
	UnloadSprite(player);
}

void MissileScene::HandleEvent(SDL_Event* event) {
	switch(event->type) {
		case SDL_KEYDOWN:
			if (event->key.keysym.sym == SDLK_a || event->key.keysym.sym == SDLK_LEFT) {
				playerVelocity = -3;
			}

			if (event->key.keysym.sym == SDLK_d || event->key.keysym.sym == SDLK_RIGHT) {
				playerVelocity = 3;
			}

			if (event->key.keysym.sym == SDLK_SPACE) {
				Bullet b;
				b.x = playerX + 10;
				b.y = 450; //TODO: fix magic numbers
				b.xvel = playerVelocity;
				b.yvel = -4;
				bullets.push_back(b);
			}
			break;
	}
}

void MissileScene::Update() {
	//create a new bullet every second
	if (std::chrono::steady_clock::now() - bulletTimer > std::chrono::duration<int, std::milli>(1000)) {
		CreateRandomBullet();
		bulletTimer = std::chrono::steady_clock::now();
	}

	//move existing bullets
	std::for_each(bullets.begin(), bullets.end(), [](Bullet& b) {
		b.x += b.xvel;
		b.y += b.yvel;
		b.trail.push_back({b.x, b.y});
	});

	//move the player
	playerX += playerVelocity;
	playerX = clamp(playerX, 0, 800 - 20); //clamp the player position to the screen

	//collision detections

	//std::remove_if simply shifts the elements around, so erase is needed
	bullets.erase(std::remove_if(bullets.begin(), bullets.end(), [&](Bullet b) {
		//exit the screen
		if (b.x < 0 || b.x >= 800) {
			return true;
		}

		//"collide" with the player
		if (b.y >= 460 && b.x > playerX && b.x < playerX + 20) { //TODO: fix magic numbers
			//TODO: end the game
			return true;
		}

		//"collide" with the ground or a city
		if (b.y >= 500) {
			cities[b.x / 160] = false;
			return true;
		}

		//"collide" with the ceiling
		if (b.y < -10) {
			return true;
		}

		//"collide" with other bullets
		auto other = std::find_if(bullets.begin(), bullets.end(), [&](Bullet c) {
			if (b.x == c.x && b.y == c.y) {
				return false;
			}

			if (abs(b.x - c.x) < 20 && abs(b.y - c.y) < 20) {
				return true;
			}

			return false;
		});

		if (other != bullets.end()) {
			return true;
		}

		return false;
	}), bullets.end());

	//TODO: end the game if no cities remain
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

	for(int i = 0; i < 5; i++) {
		if (cities[i]) {
			SDL_Rect rect = {i * 160, 500, 160, 50};
			city->SetDstRect(rect);
			city->Render(renderer);
		}
	}

	player->SetDstRect({playerX, 460, 20, 20});
	player->Render(renderer);
}

void MissileScene::CreateRandomBullet() {
	Bullet bullet;

	bullet.x = rand() % 800;
	bullet.y = 0;
	bullet.xvel = rand() % 4 - 2;
	bullet.yvel = 4; //difficulty

	bullets.push_back(bullet);
}