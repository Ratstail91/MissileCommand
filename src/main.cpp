#include "missile_scene.hpp"

#include <SDL2/SDL.h>

#include <iostream>

int SDL_main(int argc, char* argv[]) {
	//initialize SDL2
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
		std::cerr << "Failed to initialize SDL2: " << SDL_GetError() << std::endl;
		return 1;
	}

	//create a window
	SDL_Window* window = SDL_CreateWindow("Missile Command",
		SDL_WINDOWPOS_UNDEFINED,
		SDL_WINDOWPOS_UNDEFINED,
		800, //width
		600, //height
		0
	);

	if (!window) {
		std::cerr << "Failed to create a window: " << SDL_GetError() << std::endl;
		SDL_Quit();
		return 1;
	}

	//create the SDL renderer
	SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

	if (!renderer) {
		std::cerr << "Failed to create a renderer: " << SDL_GetError() << std::endl;
		SDL_DestroyWindow(window);
		SDL_Quit();
		return 1;
	}

	//the scene to use
	MissileScene* scene = new MissileScene(renderer);

	//game loop
	bool running = true;

	while(running) {
		//event loop
		SDL_Event event;
		if (SDL_PollEvent(&event)) {
			switch (event.type) {
				case SDL_QUIT:
					running = false;
					break;

				//input, etc.
				default:
					scene->HandleEvent(&event);
					break;
			}
		}

		//render event
		SDL_RenderClear(renderer);

		scene->Render(renderer);

		SDL_RenderPresent(renderer);

		//give the machine a break
		SDL_Delay(10);
	}

	//cleanup
	delete scene;

	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();

	return 0;
}