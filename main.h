#ifndef MAIN_H
#define MAIN_H

#include "gba.h"

// TODO: Create any necessary structs

/*
* For example, for a Snake game, one could be:
*
* struct snake {
*   int heading;
*   int length;
*   int row;
*   int col;
* };
*
* Example of a struct to hold state machine data:
*
* struct state {
*   int currentState;
*   int nextState;
* };
*
*/
// top left corner coordinates
// size of bee is 30x30
typedef struct sprite {
	int row;
	int col;
} Sprite;

void moveSprite(Sprite *s, int row, int col);

#endif
