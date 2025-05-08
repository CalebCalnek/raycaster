#include <SDL2/SDL.h>
#include <math.h>

#define WIDTH 640
#define HEIGHT 480
#define MAP_WIDTH 10
#define MAP_HEIGHT 10
#define TILE_SIZE 32
#define FOV 60
#define NUM_RAYS 512

#define DEG_TO_RAD(theta) ((theta) * (M_PI / 180.0))
#define RAD_TO_DEG(theta) ((theta) * (180.0 / M_PI))
#define WIN_TO_WORLD_X(X) ((X) / WIDTH * TILE_SIZE * MAP_WIDTH)
#define WIN_TO_WORLD_Y(Y) ((Y) / HEIGHT * TILE_SIZE * MAP_HEIGHT)

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
	int x, y, i;

	// draw tiles
	for (y = 0; y < MAP_HEIGHT; y++) {
		for (x = 0; x < MAP_WIDTH; x++) {
			if (map[y][x] == 1) {
				SDL_SetRenderDrawColor(renderer, 0, 255, 255, SDL_ALPHA_OPAQUE);
			} else {
				SDL_SetRenderDrawColor(renderer, 255, 0, 255, SDL_ALPHA_OPAQUE);
			}
			SDL_Rect tile = {x * TILE_SIZE, y * TILE_SIZE, TILE_SIZE, TILE_SIZE};
			SDL_RenderFillRect(renderer, &tile);
		}
	}

	// draw grid
	SDL_SetRenderDrawColor(renderer, 255, 255, 0, SDL_ALPHA_OPAQUE);
	for (i = 0; i <= MAP_WIDTH; i++) {
		SDL_RenderDrawLine(renderer, i * TILE_SIZE, 0, i * TILE_SIZE, MAP_HEIGHT * TILE_SIZE);
	}
	for (i = 0; i <= MAP_HEIGHT; i++) {
		SDL_RenderDrawLine(renderer, 0, i * TILE_SIZE, MAP_WIDTH * TILE_SIZE, i * TILE_SIZE);
	}
}

void draw_player() {
	int player_radius = 4;
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
	SDL_Rect player_rect = {
		WIN_TO_WORLD_X(player->x) - player_radius / 2,
		WIN_TO_WORLD_Y(player->y) - player_radius / 2,
		player_radius,
		player_radius
	};
	SDL_RenderFillRect(renderer, &player_rect);
}

void draw_rays(double ray_angle) {
	int length = 512;
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
	SDL_RenderDrawLine(
		renderer,
		WIN_TO_WORLD_X(player->x),
		WIN_TO_WORLD_Y(player->y),
		WIN_TO_WORLD_X(player->x + (length * cos(ray_angle))),
		WIN_TO_WORLD_Y(player->y + (length * sin(ray_angle)))
	);
}

void draw() {
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	SDL_RenderClear(renderer);

	draw_2d_map();
	draw_player();
	draw_rays(player->angle * M_PI / 180);
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
		} else if (event.type == SDL_KEYDOWN) {
			switch (event.key.keysym.sym) {
				case SDLK_UP:
					player->x += 4 * cos(DEG_TO_RAD(player->angle));
					player->y += 4 * sin(DEG_TO_RAD(player->angle));
					break;
				case SDLK_DOWN:
					player->x -= 4 * cos(DEG_TO_RAD(player->angle));
					player->y -= 4 * sin(DEG_TO_RAD(player->angle));
					break;
				case SDLK_LEFT:
					player->angle = (int) (player->angle - 5) % 360;
					break;
				case SDLK_RIGHT:
					player->angle = (int) (player->angle + 5) % 360;
					break;
			}
		}
		draw();
	}

	terminate();
	return 0;
}
