#pragma once

#include "Console.h"
#include <stdlib.h>

#define GAME_INTERVAL 500

#define PANLE_WIDTH 30
#define PANLE_HEIGHT 20

#define DIRECTION_UP 0
#define DIRECTION_LEFT 1
#define DIRECTION_RIGHT 2
#define DIRECTION_DOWN 3

#define BLOCK_WALL 0
#define BLOCK_SNAKE_BODY 1
#define BLOCK_SNAKE_HEAD 2
#define BLOCK_FOOD 3
#define BLOCK_VOID 4

char panle[PANLE_HEIGHT][PANLE_WIDTH];
WCHAR title[32];

struct Pos
{
	int x;
	int y;
};

inline struct Pos build_pos(int x, int y) {
	struct Pos p = {
		.x = x,
		.y = y
	};
	return p;
}

struct SnakeSnack
{
	int score;

	int interval;

	int snake_head_x;
	int snake_head_y;

	char snake_direction;

	struct Pos snake_fifo[PANLE_HEIGHT * PANLE_WIDTH];

	size_t snake_fifo_next;
	size_t snake_fifo_last;
} game;

void init_snake_snack();

void drop_snake_snack();

void run_snack_snake();
