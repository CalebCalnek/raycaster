#include <SDL2/SDL.h>
#include <math.h>

#define WIDTH 640
#define HEIGHT 480
#define MAP_WIDTH 10
#define MAP_HEIGHT 10
#define TILE_SIZE 32
#define FOV 60

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

void draw_ray(double ray_angle, double ray_length) {
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
	SDL_RenderDrawLine(
		renderer,
		WIN_TO_WORLD_X(player->x),
		WIN_TO_WORLD_Y(player->y),
		WIN_TO_WORLD_X(player->x) + (ray_length * cos(ray_angle)),
		WIN_TO_WORLD_Y(player->y) - (ray_length * sin(ray_angle))
	);
}

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
		ray_dy = (int) WIN_TO_WORLD_Y(player->y) % TILE_SIZE;
		if (ray_dy == 0) ray_dy = TILE_SIZE;
		dir_y = -1;
	} else if (ray_angle >= M_PI) {
		// pointing down
		ray_dy = TILE_SIZE - (int) WIN_TO_WORLD_Y(player->y) % TILE_SIZE;
		dir_y = 1;
	} if (ray_angle < M_PI / 2 || ray_angle >= M_PI * 3/2) {
		// pointing right
		ray_dx = TILE_SIZE - (int) WIN_TO_WORLD_X(player->x) % TILE_SIZE;
		dir_x = 1;
	} else if (ray_angle >= M_PI / 2 && ray_angle < M_PI * 3/2) {
		// pointing left
		ray_dx = (int) WIN_TO_WORLD_X(player->x) % TILE_SIZE;
		if (ray_dx == 0) ray_dx = TILE_SIZE;
		dir_x = -1;
	}

	double ray_x = WIN_TO_WORLD_X(player->x);
	double ray_y = WIN_TO_WORLD_Y(player->y);
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

void draw() {
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	SDL_RenderClear(renderer);

	draw_2d_map();
	draw_player();
	for (int i = 0; i < FOV; i++) {
		cast_ray((player->angle - FOV / 2 + i) * M_PI / 180, i);
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
					player->y -= 4 * sin(DEG_TO_RAD(player->angle));
					break;
				case SDLK_DOWN:
					player->x -= 4 * cos(DEG_TO_RAD(player->angle));
					player->y += 4 * sin(DEG_TO_RAD(player->angle));
					break;
				case SDLK_LEFT:
					player->angle += 5;
					break;
				case SDLK_RIGHT:
					player->angle -= 5;
					break;
			}
			if (player->angle >= 360) player->angle -= 360;
			else if (player->angle < 0) player->angle += 360;
		}
		draw();
	}

	terminate();
	return 0;
}
