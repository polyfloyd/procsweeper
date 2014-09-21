#include "util.h"
#include <ncurses.h>

#define COLOR_MAX COLOR_WHITE+1

color_t colors[COLOR_MAX][COLOR_MAX];

void util_init(void) {
	for (int f = 0; f < COLOR_MAX; f++) {
		for (int b = 0; b < COLOR_MAX; b++) {
			int i = f * 8 + b;
			init_pair(i, f, b);
			colors[f][b] = COLOR_PAIR(i);
		}
	}
}

color_t util_color_get(int front, int back) {
	return colors[front][back];
}
