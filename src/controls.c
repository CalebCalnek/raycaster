#include <SDL2/SDL.h>
#include <math.h>
#include "raycaster.h"

extern player_t *player;
extern int map[MAP_WIDTH][MAP_HEIGHT];

void move_player(int dir_x, int dir_y) {
	double player_dx = 4 * cos(DEG_TO_RAD(player->angle)) * dir_x;
	double player_dy = 4 * sin(DEG_TO_RAD(player->angle)) * dir_y;

	if (!map[
		(int) player->y / TILE_SIZE
	][
		(int) (player->x + player_dx) / TILE_SIZE
	]) {
		player->x += player_dx;
	}

	if (!map[
		(int) (player->y + player_dy) / TILE_SIZE
	][
		(int) player->x / TILE_SIZE
	]) {
		player->y += player_dy;
	}
}
