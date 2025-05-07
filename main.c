#include <SDL2/SDL.h>

#define WIDTH 640
#define HEIGHT 480

SDL_Window *window;
SDL_Renderer *renderer;

void init() {
	SDL_Init(SDL_INIT_EVERYTHING);
	window = SDL_CreateWindow(
		"myRaycaster",
		SDL_WINDOWPOS_UNDEFINED,
		SDL_WINDOWPOS_UNDEFINED,
		WIDTH,
		HEIGHT,
		SDL_WINDOW_SHOWN
	);
	renderer = SDL_CreateRenderer(window, -1, 0);
}

void draw() {
	SDL_SetRenderDrawColor(
		renderer,
		255,
		255,
		255,
		SDL_ALPHA_OPAQUE
	);
	/* SDL_RenderClear(renderer); */
	SDL_RenderDrawPoint(renderer, WIDTH / 2, HEIGHT / 2);
	SDL_RenderPresent(renderer);
}

void terminate() {
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
}

int main() {
	init();

	SDL_Event event;
	while (1) {
		SDL_PollEvent(&event);
		if (event.type == SDL_QUIT) {
			break;
		}
		draw();
	}

	terminate();
	return 0;
}
