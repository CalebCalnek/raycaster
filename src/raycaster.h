#ifndef RAYCASTER_H
#define RAYCASTER_H

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
#define NORMALIZE_ANGLE(A) fmod(fmod((A), 360.0) + 360.0, 360.0)

typedef struct {
	double x, y;
	double angle;
} player_t;

void draw_ray(double ray_angle, double ray_length);
void draw_2d_map();
void draw_player();

void draw_segment(double ray_length, int ray_index, double ray_angle);
void cast_ray(double ray_angle, int ray_index);

void move_player(int dir_x, int dir_y);

#endif
