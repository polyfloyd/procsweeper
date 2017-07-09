/*
 * Copyright (c) 2014 PolyFloyd
 */

#ifndef __BOARD_H
#define __BOARD_H

#include <stdbool.h>

// The tile has been flagged by the player.
#define TILE_FLAG 0x01
// The flag has been turned over by the player.
#define TILE_TURNED 0x02
// The tile contains a mine.
#define TILE_MINE 0x04

typedef unsigned char tile_t;

typedef struct {
	int     width;
	int     height;
	int     mines;
	bool    seeded;
	tile_t* tiles;
} board_t;

void   board_init(board_t *board, int w, int h, int mines);
void   board_destroy(board_t *board);
tile_t board_get_tile(board_t *board, int x, int y);
tile_t board_turn_tiles(board_t *board, int x, int y);
int    board_get_adjacent_mine_count(board_t *board, int x, int y);
void   board_toggle_flagged(board_t *board, int x, int y);
bool   board_is_flagged(board_t *board, int x, int y);
void   board_set_flagged(board_t *board, int x, int y, bool flagged);

#endif
