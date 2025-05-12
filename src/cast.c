#include <SDL2/SDL.h>
#include <math.h>
#include "raycaster.h"

extern SDL_Renderer *renderer;
extern player_t *player;
extern int map[MAP_WIDTH][MAP_HEIGHT];

void draw_segment(double ray_length, int ray_index, double ray_angle) {
	int ray_width = WIDTH / FOV;
	int ray_height = TILE_SIZE * HEIGHT / (ray_length * cos(ray_angle - DEG_TO_RAD(player->angle)));
	if (ray_height > HEIGHT) ray_height = HEIGHT;
	SDL_Rect segment = {
		WIN_TO_WORLD_X(WIDTH) + (FOV - ray_index) * ray_width,
		(HEIGHT - ray_height) / 2,
		ray_width,
		ray_height
	};
	// SDL_SetRenderDrawColor(renderer, 0, 255, 255, 255);
	SDL_RenderFillRect(renderer, &segment);
}

void cast_ray(double ray_angle, int ray_index) {
	double ray_length = 0;
	double ray_dx, ray_dy;
	double ray_radius_dx, ray_radius_dy;
	int dir_x, dir_y;

	// grid offsets
	if (ray_angle < M_PI) {
		// pointing up
		ray_dy = (int) player->y % TILE_SIZE;
		if (ray_dy == 0) ray_dy = TILE_SIZE;
		dir_y = -1;
	} else if (ray_angle >= M_PI) {
		// pointing down
		ray_dy = TILE_SIZE - (int) player->y % TILE_SIZE;
		dir_y = 1;
	} if (ray_angle < M_PI / 2 || ray_angle >= M_PI * 3/2) {
		// pointing right
		ray_dx = TILE_SIZE - (int) player->x % TILE_SIZE;
		dir_x = 1;
	} else if (ray_angle >= M_PI / 2 && ray_angle < M_PI * 3/2) {
		// pointing left
		ray_dx = (int) player->x % TILE_SIZE;
		if (ray_dx == 0) ray_dx = TILE_SIZE;
		dir_x = -1;
	}

	double ray_x = player->x;
	double ray_y = player->y;
	int x_tile, y_tile;
	int is_vertical_hit;

	for (int i = 0; i < 16; i++) {
		if (dir_x == 1) {
			x_tile = (int) (ray_x / TILE_SIZE);
		} else {
			x_tile = (int) ((ray_x - 1) / TILE_SIZE);
		}
		if (dir_y == 1) {
			y_tile = (int) (ray_y / TILE_SIZE);
		} else {
			y_tile = (int) ((ray_y - 1) / TILE_SIZE);
		}

		if (x_tile < 0 || x_tile >= MAP_WIDTH || y_tile < 0 || y_tile >= MAP_HEIGHT) {
			return;
		}

		if (map[y_tile][x_tile] == 1) {
			break;
		}

		ray_radius_dx = fabs(ray_dx / cos(ray_angle));
		ray_radius_dy = fabs(ray_dy / sin(ray_angle));
		if (ray_radius_dx <= ray_radius_dy) {
			ray_length += ray_radius_dx;
			ray_x += ray_dx * dir_x;
			ray_y += fabs(ray_dx * tan(ray_angle)) * dir_y;
			ray_dy -= fabs(ray_dx * tan(ray_angle));
			ray_dx = TILE_SIZE;
			is_vertical_hit = 0;
		} else if (ray_radius_dx > ray_radius_dy) {
			ray_length += ray_radius_dy;
			ray_x += fabs(ray_dy / tan(ray_angle)) * dir_x;
			ray_y += ray_dy * dir_y;
			ray_dx -= fabs(ray_dy / tan(ray_angle));
			ray_dy = TILE_SIZE;
			is_vertical_hit = 1;
		}
	}

	draw_ray(ray_angle, ray_length);
	if (is_vertical_hit) {
		SDL_SetRenderDrawColor(renderer, 0, 255, 255, 255);
	} else {
		SDL_SetRenderDrawColor(renderer, 0, 128, 128, 255);
	}
	draw_segment(ray_length, ray_index, ray_angle);
}
