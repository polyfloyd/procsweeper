#ifndef __BOARD_H
#define __BOARD_H

typedef enum {
	TILE_EMPTY,
	TILE_MINE_TURNED,
	TILE_MINE_UNTURNED,
	TILE_UNTURNED,
} tile_t;

typedef struct {
	int     width;
	int     height;
	int     mines;
	char    seeded;
	char*   flags;
	tile_t* tiles;
} board_t;

void   board_init(board_t *board, int w, int h, int mines);
void   board_destroy(board_t *board);
tile_t board_get_tile(board_t *board, int x, int y);
void   board_set_tile(board_t *board, int x, int y, tile_t tile);
tile_t board_turn_tiles(board_t *board, int x, int y);
int    board_get_adjacent_mine_count(board_t *board, int x, int y);
void   board_toggle_flagged(board_t *board, int x, int y);
char   board_is_flagged(board_t *board, int x, int y);
void   board_set_flagged(board_t *board, int x, int y, char flagged);

#endif
