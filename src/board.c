/*
 * Copyright (c) 2014 PolyFloyd
 */

#include "board.h"
#include <stdio.h>
#include <stdlib.h>

void board_init(board_t *board, int w, int h, int mines) {
	board->width  = w;
	board->height = h;
	board->mines  = mines;
	board->seeded = 0;
	board->tiles  = calloc(w * h, sizeof(tile_t));
}

void board_destroy(board_t *board) {
	free(board->tiles);
}

tile_t *board_tile(board_t *board, int x, int y) {
	if (x < 0 || x >= board->width || y < 0 || y >= board->height) {
		printf("Board position out of range: (%d, %d)", x, y);
		exit(1);
	}
	return &board->tiles[y * board->width + x];
};

tile_t board_get_tile(board_t *board, int x, int y) {
	return *board_tile(board, x, y);
};

int board_get_adjacent_mine_count(board_t *board, int x, int y) {
	int count = 0;
	for (int lx = -1; lx <= 1; lx++) {
		for (int ly = -1; ly <= 1; ly++) {
			if (x + lx < 0 || x + lx >= board->width || y + ly < 0 || y + ly >= board->height) {
				continue;
			}
			if (board_get_tile(board, x + lx, y + ly) & TILE_MINE) {
				count++;
			}
		}
	}
	return count;
};

void board_toggle_flagged(board_t *board, int x, int y) {
	*board_tile(board, x, y) ^= TILE_FLAG;
}

tile_t board_turn_tiles(board_t *board, int x, int y) {
	if (!board->seeded) {
		for (int i = 0; i < board->mines; i++) {
			int mx, my;
			do {
				mx = rand() % board->width;
				my = rand() % board->height;
			} while ((mx == x && my == y) || board_get_tile(board, x, y) & TILE_MINE);
			*board_tile(board, mx, my) |= TILE_MINE;
		}
		board->seeded = 1;
	}

	tile_t *tile = board_tile(board, x, y);
	if (!(*tile & TILE_TURNED)) {
		*tile |= TILE_TURNED;
		for (int lx = -1; lx <= 1; lx++) {
			for (int ly = -1; ly <= 1; ly++) {
				if (x + lx < 0 || x + lx >= board->width || y + ly < 0 || y + ly >= board->height) {
					continue;
				}
				if (board_get_adjacent_mine_count(board, x, y) == 0) {
					board_turn_tiles(board, x + lx, y + ly);
				}
			}
		}
	}
	return *tile;
}
