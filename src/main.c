#include "main.h"
#include <getopt.h>
#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "board.h"
#include "proc.h"
#include "util.h"

#define BOARD_WIDTH  80
#define BOARD_HEIGHT 40

#define SYM_EMPTY    '.'
#define SYM_MINE     'X'
#define SYM_UNKNOWN  '?'
#define SYM_UNTURNED '#'

char mode_system   = 0;
char mode_initonly = 0;
char mode_nokill   = 0;

int main(int argc, char **argv) {
	parse_cli(argc, argv);
	init();

	int cur_x = 0;
	int cur_y = 0;

	board_t board;
	board_init(&board, BOARD_WIDTH, BOARD_HEIGHT, BOARD_WIDTH * BOARD_HEIGHT / 8);

	bool run = true;
	while (run) {
		for (int x = 0; x < board.width; x++) {
			for (int y = 0; y < board.height; y++) {
				tile_t tile = board_get_tile(&board, x, y);
				int adjmines = board_get_adjacent_mine_count(&board, x, y);
				char sym = SYM_UNKNOWN;
				int col_f = COLOR_WHITE;
				int col_b = COLOR_BLACK;
				switch (tile) {
					case TILE_EMPTY:
						if (adjmines > 0) {
							if (adjmines == 1) {
								col_f = COLOR_BLUE;
							} else {
								col_f = COLOR_YELLOW;
							}
							sym = '0' + adjmines;
						} else {
							sym = SYM_EMPTY;
						}
						break;
					case TILE_UNTURNED:
					case TILE_MINE_UNTURNED:
						sym = SYM_UNTURNED;
						break;
					case TILE_MINE_TURNED:
						sym = SYM_MINE;
						col_f = COLOR_RED;
						break;
					default:
						break;
				}
				if (board_is_flagged(&board, x, y) && tile != TILE_MINE_TURNED) {
					col_b = COLOR_RED;
				}
				if (x == cur_x && y == cur_y) {
					col_b = COLOR_WHITE;
				}
				color_t col = util_color_get(col_f, col_b);
				attron(col);
				mvaddch(y, x, sym);
				attroff(col);
			}
		}
		refresh();

		tile_t tile;
		switch (getch()) {
		case 'q':
			run = false;
			break;
		case 'h':
			cur_x--;
			if (cur_x < 0) cur_x = 0;
			break;
		case 'j':
			cur_y++;
			if (cur_y >= board.height) cur_y = board.height - 1;
			break;
		case 'k':
			cur_y--;
			if (cur_y < 0) cur_y = 0;
			break;
		case 'l':
			cur_x++;
			if (cur_x >= board.width) cur_x = board.width - 1;
			break;
		case 'f':
			board_toggle_flagged(&board, cur_x, cur_y);
			break;
		case 'x':
			tile = board_turn_tiles(&board, cur_x, cur_y);
			if (tile == TILE_MINE_UNTURNED) {
				proc_t *proc = proc_get_random();
				char *cmd;
				if (proc->cmdline) {
					cmd = proc->cmdline[0];
				} else {
					cmd = &proc->cmd[0];
				}
				erase();
				int col = util_color_get(COLOR_RED, COLOR_BLACK);
				char *msg;
				if (mode_nokill) {
					msg = "(Pretending)";
				} else {
					msg = "Too bad!";
				}
				attron(col);
				mvprintw(board.height + 1, 2, msg);
				mvprintw(board.height + 2, 2, "Killing %d, %s", proc->tgid, cmd);
				attroff(col);
				if (!mode_nokill) {
					//proc_kill(proc);
				}
			}
			break;
		}
	}

	board_destroy(&board);
	cleanup();
	return 0;
}

void parse_cli(int argc, char **argv) {
	static struct option long_options[] = {
		{"hardcore", no_argument, 0, 'h'},
		{"pussy",    no_argument, 0, 'p'},
		{"system",   no_argument, 0, 's'},
	};
	int index = 0;
	int c;
	while ((c = getopt_long(argc, argv, "hps", long_options, &index)) != -1) {
		switch (c) {
		case 'h':
			mode_initonly = 1;
			if (geteuid() != 0) {
				printf("The --hardcore option requires root priviliges!\n");
				exit(1);
			}
			break;
		case 'p':
			mode_nokill = 1;
			break;
		case 's':
			mode_system = 1;
			if (geteuid() != 0) {
				printf("The --system option requires root priviliges!\n");
				exit(1);
			}
			break;
		}
	}
}

void init(void) {
	srand(time(NULL));
	proc_setfilter(procfilter);
	proc_init();
	initscr();
	if (!has_colors()) {
		fprintf(stderr, "Your terminal does not support colors :(\n");
		cleanup();
		exit(1);
	}
	start_color();
	util_init();
	noecho();
	raw();
}

void cleanup(void) {
	endwin();
	proc_cleanup();
}

char procfilter(proc_t *proc) {
	if (mode_system) {
		return 1;
	}
	if (mode_initonly) {
		return proc->tgid == 1;
	}
	return proc->euid == geteuid();
}
