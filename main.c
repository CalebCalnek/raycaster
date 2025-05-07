#include <SDL2/SDL.h>
#include <math.h>

#define WIDTH 640
#define HEIGHT 480
#define MAP_WIDTH 10
#define MAP_HEIGHT 10
#define TILE_SIZE 32
#define FOV 60
#define NUM_RAYS 512

typedef struct {
	double x, y;
	double angle;
} player_t;

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
		"myRaycaster",
		SDL_WINDOWPOS_UNDEFINED,
		SDL_WINDOWPOS_UNDEFINED,
		WIDTH,
		HEIGHT,
		SDL_WINDOW_SHOWN
	);
	renderer = SDL_CreateRenderer(window, -1, 0);

	player = (player_t *) malloc(sizeof(player_t));
	player->x = WIDTH / 2;
	player->y = HEIGHT / 2;
	player->angle = 0;

	SDL_SetRenderDrawColor(
		renderer,
		255,
		255,
		255,
		SDL_ALPHA_OPAQUE
	);
}

void draw_2d_map() {
	for (int y = 0; y < MAP_HEIGHT; y++) {
		for (int x = 0; x < MAP_WIDTH; x++) {
			if (map[y][x] == 1) {
				SDL_SetRenderDrawColor(renderer, 0, 255, 255, SDL_ALPHA_OPAQUE);
			} else {
				SDL_SetRenderDrawColor(renderer, 255, 0, 255, SDL_ALPHA_OPAQUE);
			}
			SDL_RenderDrawPoint(renderer, x * TILE_SIZE, y * TILE_SIZE);
		}
	}
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
	SDL_RenderDrawPoint(renderer, player->x / WIDTH * TILE_SIZE * MAP_WIDTH, player->y / HEIGHT * TILE_SIZE * MAP_HEIGHT);
}

void draw() {
	draw_2d_map();
	for (int i = 0; i < NUM_RAYS; i++) {
		/* ... */
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
		}
		draw();
	}

	terminate();
	return 0;
}
