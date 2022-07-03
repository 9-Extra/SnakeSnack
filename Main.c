#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>

#define PANLE_WIDTH 30
#define PANLE_HEIGHT 20

#define DIRECTION_UP 1
#define DIRECTION_LEFT -1
#define DIRECTION_RIGHT 2
#define DIRECTION_DOWN -2

#define BLOCK_WALL 'w'
#define BLOCK_SNAKE_BODY 'b'
#define BLOCK_SNAKE_HEAD 'h'
#define BLOCK_FOOD 'f'
#define BLOCK_VOID ' '

char panle[PANLE_HEIGHT][PANLE_WIDTH];

extern void Sleep(int time);//休眠time毫秒

void panle_clear(char c) {
	for (int i = 0; i < PANLE_HEIGHT; i++) {
		for (int j = 0; j < PANLE_WIDTH; j++) {
			panle[i][j] = c;
		}
	}
}

void console_clear() {
	puts("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n");
}

void panle_render() {
	puts("");//换行，保证光标在最左端
	for (int i = 0; i < PANLE_HEIGHT; i++) {
		for (int j = 0; j < PANLE_WIDTH; j++) {
			putchar(panle[j][i]);
		}
		puts("");
	}
}

struct SnakeSnack
{
	int score;

	int interval;

	int snake_head_x;
	int snake_head_y;

	int snake_tail_x;
	int snake_tail_y;

	char snake_direction;
} game;


void generate_snack() {
	struct Pos
	{
		int x;
		int y;
	};
	struct Pos* vaild_pos = malloc(PANLE_HEIGHT * PANLE_WIDTH * sizeof(struct Pos));
	int vaild_pos_num = 0;
	for (int i = 0; i < PANLE_HEIGHT; i++) {
		for (int j = 0; j < PANLE_WIDTH; j++) {
			if (panle[i][j] == BLOCK_VOID) {
				struct Pos p;
				p.x = i;
				p.y = j;
				vaild_pos[vaild_pos_num] = p;
				vaild_pos_num++;
			}
		}
	}

	int r = rand() % vaild_pos_num;

	panle[vaild_pos[r].x][vaild_pos[r].y] = BLOCK_FOOD;

	free(vaild_pos);
}

void init_snake_snack() {
	game.score = 0;
	game.interval = 1000;//1s
	game.snake_direction = DIRECTION_RIGHT;
	game.snake_head_x = 5;
	game.snake_head_y = 5;
	game.snake_tail_x = 3;
	game.snake_tail_y = 5;

	panle_clear(BLOCK_VOID);
	for (int i = 0; i < PANLE_HEIGHT; i++)
	{
		panle[i][0] = BLOCK_WALL;
		panle[i][PANLE_WIDTH - 1] = BLOCK_WALL;
	}
	for (int i = 0; i < PANLE_WIDTH; i++)
	{
		panle[0][i] = BLOCK_WALL;
		panle[PANLE_HEIGHT - 1][i] = BLOCK_WALL;
	}
	panle[5][5] = BLOCK_SNAKE_HEAD;
	panle[4][5] = BLOCK_SNAKE_BODY;
	panle[3][5] = BLOCK_SNAKE_BODY;

	generate_snack();
}

void run_snack_snake() {
	do
	{
		//获取输入
		if (_kbhit()) {
			switch (toupper(getch()))
			{
			case 'W': {
				if (game.snake_direction != DIRECTION_DOWN) {
					game.snake_direction = DIRECTION_UP;
				}
				break;
			}
			case 'S': {
				if (game.snake_direction != DIRECTION_UP) {
					game.snake_direction = DIRECTION_DOWN;
				}
				break;
			}
			case 'A': {
				if (game.snake_direction != DIRECTION_RIGHT) {
					game.snake_direction = DIRECTION_LEFT;
				}
				break;
			}
			case 'D': {
				if (game.snake_direction != DIRECTION_LEFT) {
					game.snake_direction = DIRECTION_RIGHT;
				}
				break;
			}
			default:
				break;
			}
		}

		panle[game.snake_head_x][game.snake_head_y] = BLOCK_SNAKE_BODY;

		//向前一步
		switch (game.snake_direction)
		{
		case DIRECTION_UP: {
			game.snake_head_y--;
			break;
		}
		case DIRECTION_DOWN: {
			game.snake_head_y++;
			break;
		}
		case DIRECTION_RIGHT: {
			game.snake_head_x++;
			break;
		}
		case DIRECTION_LEFT: {
			game.snake_head_x--;
			break;
		}
		}

		char next_block = panle[game.snake_head_x][game.snake_head_y];

		switch (next_block)
		{
		case BLOCK_VOID: {
			//尾巴缩短
			panle[game.snake_tail_x][game.snake_tail_y] = BLOCK_VOID;
			break;
		}
		case BLOCK_FOOD: {
			game.score += 10;
			break;
		}
		case BLOCK_SNAKE_BODY:
		case BLOCK_WALL: {
			return;
		}
		}

		panle[game.snake_head_x][game.snake_head_y] = BLOCK_SNAKE_HEAD;

		console_clear();
		panle_render();

		Sleep(game.interval);

	} while (true);

}

int main() {
	printf("Hello, world!");

	console_clear();
	init_snake_snack();

	run_snack_snake();

	printf("Score: %d\n", game.score);

	getchar();//pause

	return 0;
}