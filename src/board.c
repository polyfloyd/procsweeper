/*
 * Copyright (c) 2014 PolyFloyd
 */

#include "board.h"
#include <stdlib.h>

void board_init(board_t *board, int w, int h, int mines) {
	board->width  = w;
	board->height = h;
	board->mines  = mines;
	board->flags  = calloc(w * h, sizeof(char));
	board->tiles  = calloc(w * h, sizeof(tile_t));
	for (int x = 0; x < board->width; x++) {
		for (int y = 0; y < board->height; y++) {
			board_set_tile(board, x, y, TILE_UNTURNED);
		}
	}
}

void board_destroy(board_t *board) {
	free(board->tiles);
	free(board->flags);
}

tile_t board_get_tile(board_t *board, int x, int y) {
	if (x < 0 || x >= board->width || y < 0 || y >= board->height) {
		return TILE_EMPTY;
	}
	return board->tiles[y * board->width + x];
};

void board_set_tile(board_t *board, int x, int y, tile_t tile) {
	board->tiles[y * board->width + x] = tile;
};

int board_get_adjacent_mine_count(board_t *board, int x, int y) {
	int count = 0;
	for (int lx = -1; lx <= 1; lx++) {
		for (int ly = -1; ly <= 1; ly++) {
			tile_t tile = board_get_tile(board, x + lx, y + ly);
			if (tile == TILE_MINE_UNTURNED || tile == TILE_MINE_TURNED) count++;
		}
	}
	return count;
};

void board_toggle_flagged(board_t *board, int x, int y) {
	board_set_flagged(board, x, y, !board_is_flagged(board, x, y));
}

char board_is_flagged(board_t *board, int x, int y) {
	return board->flags[y * board->width + x];
}

void board_set_flagged(board_t *board, int x, int y, char flagged) {
	board->flags[y * board->width + x] = flagged;
}

tile_t board_turn_tiles(board_t *board, int x, int y) {
	if (!board->seeded) {
		for (int i = 0; i < board->mines; i++) {
			int mx, my;
			do {
				mx = rand() % board->width;
				my = rand() % board->height;
			} while ((mx == x && my == y) || board_get_tile(board, x, y) == TILE_MINE_UNTURNED);
			board_set_tile(board, mx, my, TILE_MINE_UNTURNED);
		}
		board->seeded = 1;
	}
	tile_t tile = board_get_tile(board, x, y);
	if (tile == TILE_UNTURNED) {
		board_set_tile(board, x, y, TILE_EMPTY);
		for (int lx = -1; lx <= 1; lx++) {
			for (int ly = -1; ly <= 1; ly++) {
				if (!board_get_adjacent_mine_count(board, x, y)) {
					board_turn_tiles(board, x + lx, y + ly);
				}
			}
		}
	} else if (tile == TILE_MINE_UNTURNED) {
		board_set_tile(board, x, y, TILE_MINE_TURNED);
	}
	return tile;
}
