#include "SnakeSnack.h"

void panle_clear(char c) {
	for (int i = 0; i < PANLE_HEIGHT; i++) {
		for (int j = 0; j < PANLE_WIDTH; j++) {
			panle[i][j] = c;
		}
	}
}

void update_title() {
	wsprintfW(title, L"SnakeSnack score: %d", game.score);
	SetConsoleTitleW(title);
}

void game_render() {
	for (int i = 0; i < PANLE_HEIGHT; i++) {
		for (int j = 0; j < PANLE_WIDTH; j++) {
			WCHAR content;
			switch (panle[i][j])
			{
			case BLOCK_WALL: {
				content = L'■';
				break;
			}
			case BLOCK_VOID: {
				content = L' ';
				break;
			}
			case BLOCK_FOOD: {
				content = L'$';
				break;
			}
			case BLOCK_SNAKE_BODY: {
				content = L'□';
				break;
			}
			case BLOCK_SNAKE_HEAD: {
				content = L'△';
				break;
			}
			default:
				content = L'?';
				break;
			};
			paint_target(i, j, content);
		}
	}
	console_swap();
}

void snake_fifo_clear() {
	game.snake_fifo_last = 0;
	game.snake_fifo_next = 0;
}

void snake_fifo_push(struct Pos head) {
	game.snake_fifo_next++;
	if (game.snake_fifo_next == PANLE_HEIGHT * PANLE_WIDTH) {
		game.snake_fifo_next = 0;
	}
	game.snake_fifo[game.snake_fifo_next] = head;
}

struct Pos snake_fifo_pop() {
	game.snake_fifo_last++;

	if (game.snake_fifo_next == PANLE_HEIGHT * PANLE_WIDTH) {
		game.snake_fifo_next = 0;
	}

	return game.snake_fifo[game.snake_fifo_last];
}

void generate_snack() {
	struct Pos* vaild_pos = malloc(PANLE_HEIGHT * PANLE_WIDTH * sizeof(struct Pos));
	if (vaild_pos == NULL) {
		exit(-1);
	}
	int vaild_pos_num = 0;
	for (int i = 0; i < PANLE_HEIGHT; i++) {
		for (int j = 0; j < PANLE_WIDTH; j++) {
			if (panle[i][j] == BLOCK_VOID) {
				struct Pos p;
				p.y = i;//所在列数
				p.x = j;//所在行数
				vaild_pos[vaild_pos_num] = p;
				vaild_pos_num++;
			}
		}
	}

	int r = rand() % vaild_pos_num;

	panle[vaild_pos[r].y][vaild_pos[r].x] = BLOCK_FOOD;

	free(vaild_pos);
}

void init_snake_snack() {

	game.score = 0;
	game.interval = GAME_INTERVAL;
	game.snake_direction = DIRECTION_RIGHT;
	game.snake_head_x = 5;
	game.snake_head_y = 5;

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
	panle[5][5] = BLOCK_SNAKE_BODY;
	panle[5][4] = BLOCK_SNAKE_BODY;
	panle[5][3] = BLOCK_SNAKE_BODY;

	snake_fifo_clear();
	snake_fifo_push(build_pos(3, 5));
	snake_fifo_push(build_pos(4, 5));
	snake_fifo_push(build_pos(5, 5));

	update_title();

	generate_snack();

	init_console(PANLE_WIDTH, PANLE_HEIGHT);

}

void drop_snake_snack() {
	drop_console();
}

void update_snake_direction() {
	load_console_input();

	bool vaild = false;
	//从头读出记录的所有输入，当读出一个有效时即认为是本回合操作，剩下的留在缓冲区
	while (!key_fifo_empty() && !vaild) {
		switch (key_fifo_pop())
		{
		case 'W': {
			if (game.snake_direction != DIRECTION_DOWN && game.snake_direction != DIRECTION_UP) {
				game.snake_direction = DIRECTION_UP;
				vaild = true;
			}
			break;
		}
		case 'S': {
			if (game.snake_direction != DIRECTION_UP && game.snake_direction != DIRECTION_DOWN) {
				game.snake_direction = DIRECTION_DOWN;
				vaild = true;
			}
			break;
		}
		case 'A': {
			if (game.snake_direction != DIRECTION_RIGHT && game.snake_direction != DIRECTION_LEFT) {
				game.snake_direction = DIRECTION_LEFT;
				vaild = true;
			}
			break;
		}
		case 'D': {
			if (game.snake_direction != DIRECTION_LEFT && game.snake_direction != DIRECTION_RIGHT) {
				game.snake_direction = DIRECTION_RIGHT;
				vaild = true;
			}
			break;
		}
		default:
			break;
		}
	}
}

void run_snack_snake() {
	do
	{
		//获取输入
		update_snake_direction();

		panle[game.snake_head_y][game.snake_head_x] = BLOCK_SNAKE_BODY;

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

		char next_block = panle[game.snake_head_y][game.snake_head_x];
		panle[game.snake_head_y][game.snake_head_x] = BLOCK_SNAKE_HEAD;
		snake_fifo_push(build_pos(game.snake_head_x, game.snake_head_y));

		switch (next_block)
		{
		case BLOCK_VOID: {
			//尾巴缩短
			struct Pos tail = snake_fifo_pop();
			panle[tail.y][tail.x] = BLOCK_VOID;
			break;
		}
		case BLOCK_FOOD: {
			game.score += 10;
			generate_snack();//重新生成食物
			update_title();
			break;
		}
		case BLOCK_SNAKE_BODY:
		case BLOCK_WALL: {
			return;//GameOver
		}
		}

		game_render();

		Sleep(game.interval);

	} while (true);

}
