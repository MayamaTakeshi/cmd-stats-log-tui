#include <signal.h>
#include "tui.h"

static int parent_y;
static int parent_x;

#define PAIR_CYAN_BLACK 1
#define PAIR_BLUE_BLACK 2
#define PAIR_GREEN_BLACK 3

void tui_init(void (*sigint_handler) ()) {
		initscr();
		start_color();
		init_pair(PAIR_CYAN_BLACK, COLOR_CYAN, COLOR_BLACK);
		init_pair(PAIR_BLUE_BLACK, COLOR_BLUE, COLOR_BLACK);
		init_pair(PAIR_GREEN_BLACK, COLOR_GREEN, COLOR_BLACK);

		keypad(stdscr, TRUE);
		nodelay(stdscr, TRUE);
		cbreak();
		noecho();
		nonl();
		curs_set(FALSE);

		parent_y = 0;
		parent_x = 0;

		(void) signal(SIGINT, sigint_handler);
}

void draw_borders(WINDOW *screen, char *name) {
	int x, y, i;
	getmaxyx(screen, y, x);

	wattron(screen, COLOR_PAIR(PAIR_CYAN_BLACK));

	// 4 corners
	mvwprintw(screen, 0, 0, "+");
	mvwprintw(screen, y - 1, 0, "+");
	mvwprintw(screen, 0, x - 1, "+");
	mvwprintw(screen, y - 1, x - 1, "+");

	// sides 
	for (i = 1; i < (y - 1); i++) {
		mvwprintw(screen, i, 0, "|");
		mvwprintw(screen, i, x - 1, "|");
	}

	char *p = name;

	// top and bottom
	for (i = 1; i < (x - 1); i++) {
		// top 
		if(i>1 && *p) {
			wattron(screen, COLOR_PAIR(PAIR_GREEN_BLACK));
			mvwprintw(screen, 0, i, "%c", *p++);
		} else {
			wattron(screen, COLOR_PAIR(PAIR_CYAN_BLACK));
			mvwprintw(screen, 0, i, "-");
		}
		// bottom	
		wattron(screen, COLOR_PAIR(PAIR_CYAN_BLACK));
		mvwprintw(screen, y - 1, i, "-");
	}

	wattroff(screen, COLOR_PAIR(PAIR_CYAN_BLACK));
}


void tui_redraw_borders(window_t *windows, int count) {
	int i;
	int panel_size;
	int new_y, new_x;
	int lines;
	int line_count = 0;

	getmaxyx(stdscr, new_y, new_x);
	if (new_y != parent_y || new_x != parent_x) {
		parent_x = new_x; parent_y = new_y;
		panel_size = parent_y / count;

		for(i=0 ; i<count ; i++) {
			if(i == 0) {
				// first window gets the extra lines (remaining of division or the screen real estate)
				lines =  panel_size + (parent_y % count);
				wresize(windows[i].w, lines, new_x);
			} else {
				lines = panel_size;
				wresize(windows[i].w, lines, new_x);
			}
			mvwin(windows[i].w, line_count, 0);
			line_count += lines;
		}

		wclear(stdscr);
		for(i=0 ; i<count ; i++) {
			wclear(windows[i].w);
		}
	}

	doupdate();

	for(i=0 ; i<count ; i++) {
		draw_borders(windows[i].w, windows[i].name);
	}
}


void tui_refresh_windows(window_t *windows, int count) {
	int i;
	for(i=0 ; i<count ; i++) {
		wrefresh(windows[i].w);
	}
}
