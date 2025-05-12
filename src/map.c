#include <SDL2/SDL.h>
#include <math.h>
#include "raycaster.h"

extern SDL_Renderer *renderer;
extern player_t *player;
extern int map[MAP_WIDTH][MAP_HEIGHT];

void draw_ray(double ray_angle, double ray_length) {
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
	SDL_RenderDrawLine(
		renderer,
		player->x,
		player->y,
		player->x + (ray_length * cos(ray_angle)),
		player->y - (ray_length * sin(ray_angle))
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
		player->x - player_radius / 2,
		player->y - player_radius / 2,
		player_radius,
		player_radius
	};
	SDL_RenderFillRect(renderer, &player_rect);
}
