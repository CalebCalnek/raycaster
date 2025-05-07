#include <SDL2/SDL.h>

#define WIDTH 640
#define HEIGHT 480

int main() {
	SDL_Init(SDL_INIT_EVERYTHING);
	SDL_Window *window = SDL_CreateWindow(
		"myRaycaster",
		SDL_WINDOWPOS_UNDEFINED,
		SDL_WINDOWPOS_UNDEFINED,
		WIDTH,
		HEIGHT,
		SDL_WINDOW_SHOWN
	);

	SDL_Event e;
	while (1) {
		SDL_PollEvent(&e);
		if (e.type == SDL_QUIT) {
			break;
		}
	}

	SDL_DestroyWindow(window);
	SDL_Quit();
	return 0;
}
