#include "Console.h"

INPUT_RECORD buffer[32];
HANDLE h_in, h_out;

CHAR_INFO* console_target;

static COORD target_size;
const static COORD target_up_left = { .X = 0,.Y = 0 };
static SMALL_RECT draw_region;
const static CONSOLE_CURSOR_INFO cursor_config = { .bVisible = false,.dwSize = 10 };

#define KEY_FILO_LEN 32

WORD key_fifo[KEY_FILO_LEN];
int key_fifo_top;
int key_fifo_back;

void key_fifo_clear() {
	key_fifo_top = 0;
	key_fifo_back = 0;
}

bool key_fifo_empty() {
	return key_fifo_top == key_fifo_back;
}

void key_fifo_push(WORD key) {
	key_fifo[key_fifo_top] = key;
	key_fifo_top++;
	if (key_fifo_top == KEY_FILO_LEN) {
		key_fifo_top = 0;
	}
	//fifo满了，丢弃最后一个
	if (key_fifo_top == key_fifo_back) {
		key_fifo_pop();
	}
}

WORD key_fifo_pop() {
	WORD key = key_fifo[key_fifo_back];
	key_fifo_back++;
	if (key_fifo_back == KEY_FILO_LEN) {
		key_fifo_back = 0;
	}
	return key;
}

void init_console(int width, int height) {
	width = width * 2;
	h_in = GetStdHandle(STD_INPUT_HANDLE);
	h_out = GetStdHandle(STD_OUTPUT_HANDLE);
	if (h_in == INVALID_HANDLE_VALUE || h_out == INVALID_HANDLE_VALUE) {
		ExitProcess(-1);
	}

	target_size.X = width;
	target_size.Y = height;

	draw_region.Left = 0;
	draw_region.Top = 0;
	draw_region.Right = draw_region.Left + width;
	draw_region.Bottom = draw_region.Top + height;

	console_target = malloc(width * height * sizeof(CHAR_INFO));
	if (console_target == NULL) {
		exit(-1);
	}

	SetConsoleCursorInfo(h_out, &cursor_config);

	console_clear();
	FlushConsoleInputBuffer(h_in);

	key_fifo_clear();
}

void drop_console() {
	FlushConsoleInputBuffer(h_in);

	free(console_target);

	//CloseHandle(h_in);
	//CloseHandle(h_out);

}

void load_console_input() {
	DWORD n_read;

	while (true) {
		if (!GetNumberOfConsoleInputEvents(h_in, &n_read)) {
			ExitProcess(-1);
		}
		if (n_read == 0) {
			break;
		}
		{
			ReadConsoleInputA(h_in, buffer, 32, &n_read);
			for (DWORD i = 0; i < n_read; i++) {
				if (buffer[i].EventType == KEY_EVENT && buffer[i].Event.KeyEvent.bKeyDown)
				{
					key_fifo_push(buffer[i].Event.KeyEvent.wVirtualKeyCode);
				}
			}
		}
	}

}

void console_clear()
{
	for (int i = 0; i < target_size.Y; i++) {
		for (int j = 0; j < target_size.X; j++) {
			console_target[i * target_size.X + j].Attributes = BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_BLUE | BACKGROUND_INTENSITY;
			console_target[i * target_size.X + j].Char.UnicodeChar = L' ';
		}
	}
	console_swap();
}

void paint_target(int x, int y, WCHAR c)
{
	console_target[x * target_size.X + y * 2].Char.UnicodeChar = c;
}

void console_swap() {
	WriteConsoleOutputW(h_out,
		console_target,
		target_size,
		target_up_left,
		&draw_region
	);
	SetConsoleCursorPosition(h_out, target_up_left);
}
