#pragma once
#include "winapi.h"

typedef void (*KeyProc) (WORD key);

extern CHAR_INFO* console_target;

void init_console(int width, int height);
void drop_console();
void proc_console_input(KeyProc key_func);

void paint_target(int x, int y, WCHAR c);
void console_clear();
void console_swap();

