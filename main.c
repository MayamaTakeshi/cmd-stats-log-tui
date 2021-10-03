#include <unistd.h> /* For getopt */
#include <signal.h>

#include <sys/time.h>

#include <sys/socket.h> 
#include <netinet/in.h> 

#include <errno.h>

#include "tui.h"

#include <stdlib.h>


int quit = 0;

static void finish(int sig) {
    quit = 1;
}


void usage(char *app_name) {
	printf("ADD_DESCRIPTION_HERE \n" \
"Usage: %s app_name param1 param2 paran3 ... paramN\n" \
, app_name);
}

int main(int argc, char *argv[])
{
	char local_uri[80];
	int opt;

	if(argc > 1) {
		printf("Invalid number of arguments\n");
		usage(argv[0]);
		exit(1);
	}

	{
		tui_init(finish);	

		char temp[2048];
		WINDOW *control = newwin(0, 0, 0, 0);
		WINDOW *stats = newwin(0, 0, 0, 0);
		WINDOW *log = newwin(0, 0, 0, 0);
		window_t windows[] = {
			{
				control,
				"CONTROL"
			},
			{
				stats,
				"STATS"
			},
			{
				log,
				"LOG"
			}
		};

		int windows_size = sizeof(windows)/sizeof(windows[0]);

		while(!quit) {
			int c;

			int no_sleep = 0;

			c = getch();
			switch (c) {
			case '+':
				break;
			case '-':
				break;
			case KEY_UP:
				break;
			case KEY_DOWN: 
				break;
			case KEY_RIGHT:
				break;
			case KEY_LEFT: 
				break;
			}

			// write data

			sprintf(temp, "traffic (+/-: 1, UP/DOWN: 10, LEFT/RIGHT: 100): %u               ", 10);
			mvwprintw(control, 1, 1, temp);

			mvwprintw(control, 2, 1, " terminate (Ctrl+c)");

			sprintf(temp, " inviting_calls: %u               ", 10);
			mvwprintw(stats, 1, 1, temp);

			sprintf(temp, " answered_calls: %u               ", 10);
			mvwprintw(stats, 2, 1, temp);

			tui_redraw_borders(windows, windows_size);

			tui_refresh_windows(windows, windows_size); 

			if(!no_sleep) {
				sleep(1);
			}
		}

		delwin(control);
		delwin(stats);
		delwin(log);
		endwin();
	} 

	puts("Bye bye..");
	return 0;

on_error:
	puts("An error has occurred. run a debugger..");
	return 1;
}

