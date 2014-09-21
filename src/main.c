#include "main.h"
#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>
#include "board.h"
#include "util.h"

#define BOARD_WIDTH  40
#define BOARD_HEIGHT 20

#define SYM_EMPTY    '.'
#define SYM_FLAG     '!'
#define SYM_MINE     'X'
#define SYM_UNKNOWN  '?'
#define SYM_UNTURNED '#'

int main(int argc, char **argv) {
	init();
	util_init();

	int cur_x = 0;
	int cur_y = 0;

	board_t board;
	board_init(&board, BOARD_WIDTH, BOARD_HEIGHT, BOARD_WIDTH * BOARD_HEIGHT / 4);

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
							col_f = COLOR_YELLOW;
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
				if (x == cur_x && y == cur_y) {
					col_b = COLOR_WHITE;
				}
				if (board_is_flagged(&board, x, y) && tile != TILE_MINE_TURNED) {
					col_b = COLOR_RED;
				}
				int col = util_color_get(col_f, col_b);
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
			break;
		}
	}

	board_destroy(&board);
	cleanup();
	return 0;
}

void init(void) {
	initscr();
	if (!has_colors()) {
		fprintf(stderr, "Your terminal does not support colors :(\n");
		cleanup();
		exit(1);
	}
	start_color();
	noecho();
	raw();
}

void cleanup(void) {
	endwin();
}
