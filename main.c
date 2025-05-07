#include <SDL2/SDL.h>

int main() {
	SDL_Init(SDL_INIT_EVERYTHING);
	SDL_Window *window = SDL_CreateWindow("myWindow", 100, 100, 640, 480, SDL_WINDOW_SHOWN);

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
