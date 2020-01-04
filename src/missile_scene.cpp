#include "missile_scene.hpp"

#include <algorithm>
#include <cmath>
#include <cstdlib>

//utility functions
inline int clamp(int v, int lo, int hi) {
	if (v < lo) return lo;
	if (v > hi) return hi;
	return v;
}

inline float distance(int x1, int y1, int x2, int y2) {
	// Calculating distance 
	return sqrt(pow(x2 - x1, 2) + pow(y2 - y1, 2) * 1.0); 
} 

MissileScene::MissileScene(SDL_Renderer* renderer) {
	background = LoadSprite(renderer, "rsc/background.bmp", {0, 0, 800, 600}, {0, 0, 800, 600});
	dot = LoadSprite(renderer, "rsc/dot.bmp", {0, 0, 8, 8}, {0, 0, 8, 8});
	city = LoadSprite(renderer, "rsc/city.bmp", {0, 0, 160, 50}, {0, 0, 160, 50});
	player = LoadSprite(renderer, "rsc/player.bmp", {0, 0, 20, 20}, {0, 0, 20, 20});
	explosion1 = LoadSprite(renderer, "rsc/explosion1.bmp", {0, 0, 64, 64}, {0, 0, 64, 64});
	explosion2 = LoadSprite(renderer, "rsc/explosion2.bmp", {0, 0, 64, 64}, {0, 0, 64, 64});
	gameOver = LoadSprite(renderer, "rsc/gameover.bmp", {0, 0, 800, 600}, {0, 0, 800, 600});

	for(int i = 0; i < 5; i++) {
		cities[i] = true;
	}
}

MissileScene::~MissileScene() {
	UnloadSprite(background);
	UnloadSprite(dot);
	UnloadSprite(city);
	UnloadSprite(player);
	UnloadSprite(explosion1);
	UnloadSprite(explosion2);
	UnloadSprite(gameOver);
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
	if (IsGameOver()) {
		return;
	}

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
			CreateExplosion(b.x, b.y);

			//end the game
			playerAlive = false;

			return true;
		}

		//"collide" with the ground or a city
		if (b.y >= 500) {
			cities[b.x / 160] = false;
			CreateExplosion(b.x, b.y);
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
			CreateExplosion(b.x, b.y);
			return true;
		}

		//"collide" with explosions
		auto expl = std::find_if(explosions.begin(), explosions.end(), [&](Explosion e) {
			return distance(b.x, b.y, e.x, e.y) < 32;
		});

		if (expl != explosions.end()) {
			CreateExplosion(b.x, b.y);
			return true;
		}

		return false;
	}), bullets.end());

	//prune the explosions that are too old
	explosions.erase(std::remove_if(explosions.begin(), explosions.end(), [&](Explosion e) {
		return std::chrono::steady_clock::now() - e.creationTime > std::chrono::duration<int, std::milli>(500);
	}), explosions.end());
}

void MissileScene::Render(SDL_Renderer* renderer) {
	if (IsGameOver()) {
		gameOver->Render(renderer);

		//render remaining cities
		for(int i = 0; i < 5; i++) {
			if (cities[i]) {
				SDL_Rect rect = {i * 160, 500, 160, 50};
				city->SetDstRect(rect);
				city->Render(renderer);
			}
		}

		//render remaining explosions
		std::for_each(explosions.begin(), explosions.end(), [&](Explosion e) {
			SDL_Rect rect = {e.x - 32, e.y - 32, 64, 64};

			if (rand() % 2) {
				explosion1->SetDstRect(rect);
				explosion1->Render(renderer);
			} else {
				explosion2->SetDstRect(rect);
				explosion2->Render(renderer);
			}
		});

		return;
	}

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

	//for each city
	for(int i = 0; i < 5; i++) {
		if (cities[i]) {
			SDL_Rect rect = {i * 160, 500, 160, 50};
			city->SetDstRect(rect);
			city->Render(renderer);
		}
	}

	//for each explosion
	std::for_each(explosions.begin(), explosions.end(), [&](Explosion e) {
		SDL_Rect rect = {e.x - 32, e.y - 32, 64, 64};

		if (rand() % 2) {
			explosion1->SetDstRect(rect);
			explosion1->Render(renderer);
		} else {
			explosion2->SetDstRect(rect);
			explosion2->Render(renderer);
		}
	});

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

void MissileScene::CreateExplosion(int x, int y) {
	Explosion e;
	e.x = x;
	e.y = y;
	explosions.push_back(e);
}

bool MissileScene::IsGameOver() {
	if (!playerAlive) {
		return true;
	}

	for(int i = 0; i < 5; i++) {
		if (cities[i]) {
			return false;
		}
	}

	return true;
}