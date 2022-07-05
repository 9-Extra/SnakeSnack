#pragma once
#include "winapi.h"
#include <stdbool.h>
#include <stdlib.h>

extern CHAR_INFO* console_target;

void key_fifo_clear();

bool key_fifo_empty();

void key_fifo_push(WORD key);

WORD key_fifo_pop();

void init_console(int width, int height);
void drop_console();
void load_console_input();

void paint_target(int x, int y, WCHAR c);
void console_clear();
void console_swap();

