#ifndef __TUI_H__
#define __TUI_H__

#include <curses.h>

typedef struct window_t {
	WINDOW *w;
	char *name;
} window_t;

void tui_init(void (*sigint_handler) ());

void tui_redraw_borders(window_t *windows, int count);

void tui_refresh_windows(window_t *windows, int count);

#define ctrl(x)           ((x) & 0x1f)

#endif

