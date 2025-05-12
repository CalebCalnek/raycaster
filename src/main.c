#include <SDL2/SDL.h>
#include <math.h>
#include "raycaster.h"

SDL_Window *window;
SDL_Renderer *renderer;
player_t *player;

int map[MAP_WIDTH][MAP_HEIGHT] = {
	{1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
	{1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
	{1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
	{1, 0, 0, 1, 0, 0, 1, 0, 0, 1},
	{1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
	{1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
	{1, 0, 0, 1, 0, 0, 1, 0, 0, 1},
	{1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
	{1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
	{1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
};

void init() {
	SDL_Init(SDL_INIT_EVERYTHING);
	window = SDL_CreateWindow(
		"raycaster",
		SDL_WINDOWPOS_UNDEFINED,
		SDL_WINDOWPOS_UNDEFINED,
		WIDTH,
		HEIGHT,
		SDL_WINDOW_SHOWN
	);
	renderer = SDL_CreateRenderer(window, -1, 0);

	player = (player_t *) malloc(sizeof(player_t));
	player->x = WIN_TO_WORLD_X(WIDTH) / 2;
	player->y = WIN_TO_WORLD_Y(HEIGHT) / 2;
	player->angle = 0;

	SDL_SetRenderDrawColor(
		renderer,
		255,
		255,
		255,
		SDL_ALPHA_OPAQUE
	);
}

void draw() {
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	SDL_RenderClear(renderer);

	draw_2d_map();
	draw_player();
	for (int i = 0; i < FOV; i++) {
		cast_ray(DEG_TO_RAD(NORMALIZE_ANGLE(player->angle - FOV / 2 + i)), i);
	}

	SDL_RenderPresent(renderer);
}

void terminate() {
	free(player);
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
		} else if (event.type == SDL_KEYDOWN) {
			switch (event.key.keysym.sym) {
				case SDLK_UP:
					move_player(1, -1);
					break;
				case SDLK_DOWN:
					move_player(-1, 1);
					break;
				case SDLK_LEFT:
					player->angle += 5;
					break;
				case SDLK_RIGHT:
					player->angle -= 5;
					break;
			}
			player->angle = NORMALIZE_ANGLE(player->angle);
		}
		draw();
	}

	terminate();
	return 0;
}
