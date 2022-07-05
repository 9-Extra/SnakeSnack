#include <stdio.h>
#include "SnakeSnack.h"

int main() {
	
	WCHAR msg[32];
	int r;
	do {
		init_snake_snack();

		run_snack_snake();

		//console_clear();
		drop_snake_snack();

		wsprintfW(msg, L"Score: %d  Continue?\n", game.score);

		r = MessageBoxW(NULL, msg, L"GameOver", MB_YESNO);
	} while (r == 6);

	return 0;
}