#include "main.h"

#include <stdio.h>
#include <stdlib.h>

#include "gba.h"

#include "images/start.h"
#include "images/end.h"
#include "images/sprite.h"
#include "images/home.h"
#include "images/beach.h"
#include "images/lost.h"

Sprite b;

enum gba_state {
  START,
  START_TEXT,
  PLAY,
  UPDATEMOVEMENT,
  WIN,
  WIN_TEXT,
  LOSE,
  LOSE_TEXT,
  RESTART
};

int main(void) {
  REG_DISPCNT = MODE3 | BG2_ENABLE;

  // Save current and previous state of button input.
  u32 previousButtons = BUTTONS;
  u32 currentButtons = BUTTONS;

  // Load initial application state
  enum gba_state state = START;

  char title[] = "PRESS ENTER TO PLAY";
  char buffer[30];
  int speed = 3;

  //home is the GOAL, the DESITNATION 
  int homeRow = 130;
  int homeCol = 210;
  //beach is the STARTING POINT, it is where you (the black square) is at!
  int beachRow = 0;
  int beachCol = 0;
  //this is the timer
  int timer = 0;

  while (1) {
    currentButtons = BUTTONS; // Load the current state of the buttons


    switch (state) {
      //this state starts the beginning of the game, brings player to home screen
      case START:
        waitForVBlank();
        drawFullScreenImageDMA(start);
        timer = 0;
        state = START_TEXT;
        break;

      //this state dictates the game to either start to restart depending on what button the player chooses
      case START_TEXT:
        drawString(60, 65, title, BLACK);
          if (KEY_DOWN(BUTTON_START, currentButtons)) {
            state = PLAY;
          }
          if (KEY_DOWN(BUTTON_SELECT, currentButtons)) {
            state = RESTART;
          }
        break;

      //this state is the beginning of play, and draws the screen with the starting point and the goal point 
      case PLAY:
        waitForVBlank();
        fillScreenDMA(WHITE);
        Sprite *sptr, s;
        sptr = &s; 
        (*sptr).row = 0;
        (*sptr).col = 0;
        drawImageDMA((*sptr).row, (*sptr).col, 10, 10, sprite);
        drawImageDMA(homeRow, homeCol, 30, 30, home);
        drawImageDMA(beachRow, beachCol, 30, 30, beach);
        state = UPDATEMOVEMENT;
        break;
      //the second part of play, where the movement is updated based on the buttons the player chooses
      case UPDATEMOVEMENT:
        waitForVBlank();
        drawImageDMA(beachRow, beachCol, 30, 30, beach); //start
        drawRectDMA(5, 160, 80, 25, WHITE); //timer
        drawImageDMA((*sptr).row, (*sptr).col, 10, 10, sprite); //bee but not a bee
        sprintf(buffer, "TIMER: %d", (timer / 60)); //timer
        drawString(5, 160, buffer, BLACK);

        //updating the movement!!!
        if (KEY_DOWN(BUTTON_UP, currentButtons)) {
          moveSprite(sptr, -speed, 0);
        }
        if (KEY_DOWN(BUTTON_DOWN, currentButtons)) {
          moveSprite(sptr, speed, 0);
        }
        if (KEY_DOWN(BUTTON_RIGHT, currentButtons)) {
          moveSprite(sptr, 0, speed);
        }
        if (KEY_DOWN(BUTTON_LEFT, currentButtons)) {
          moveSprite(sptr, 0, -speed);
        }
        int X = (*sptr).col;
        int Y = (*sptr).row;
        //checking if you've reached the goal 
        int reachedHome = 0;

        // top right // top left // bottom right // bottom left, if you touch anywhere here, you will win!!!
        if (X + 10 >= homeCol && X + 10 <= homeCol + 10 && Y >= homeRow && Y <= homeRow + 10) {
          reachedHome = 1;
        } else if (X + 10 >= homeCol && X + 10 <= homeCol + 10 && Y + 10 >= homeRow && Y + 10 <= homeRow + 10) {
          reachedHome = 1;
        } else if (X >= homeCol && X <= homeCol + 10 && Y >= homeRow && Y <= homeRow + 10) {
          reachedHome = 1;
        } else if (X >= homeCol && X <= homeCol + 10 && Y + 10 >= homeRow && Y + 10 <= homeRow + 10) {
          reachedHome = 1;
        }
        if (timer == 600) {
          state = LOSE;
        }
        //this is the transition to the WIN SCREEN!!!
        if (reachedHome == 1) {
          fillScreenDMA(BLACK);
          drawImageDMA((*sptr).row, (*sptr).col, 10, 10, sprite);
          state = WIN;
        }
        //will restart the game
        if (KEY_DOWN(BUTTON_SELECT, currentButtons)) {
          state = RESTART;
        }

        timer++;
        break;
      //if you win, this transitions to the win screen!
      case WIN:
        waitForVBlank();
        drawFullScreenImageDMA(end);
        state = WIN_TEXT;
        break;
      //the win screen!!! 
      case WIN_TEXT:
        waitForVBlank();
        drawString(60, 65, "GOT HOME SAFELY :)", BLACK);
        sprintf(buffer, "TIME TAKEN: %d", timer / 60);

        if (KEY_DOWN(BUTTON_SELECT, currentButtons)) {
          state = RESTART;
        }
        break;
      //if you lose, this transitions to the lose screen!
      case LOSE:
        waitForVBlank();
        drawFullScreenImageDMA(lost);
        state = LOSE_TEXT;
        break;
      //the lose screen!!! 
      case LOSE_TEXT:
        drawString(60, 65, "LOST FOREVER :(( TRY AGAIN!!", WHITE);
        if (KEY_DOWN(BUTTON_SELECT, currentButtons)) {
          state = RESTART;
        }
        break;
      //the restart case!!!
      case RESTART:
        state = START;
        break;
    }
  }

  UNUSED(previousButtons); 
  return 0;
}

void moveSprite(Sprite *s, int row, int col) {
  drawRectDMA((*s).row, (*s).col, 10, 10, WHITE);
  if ((((*s).row + row) >= 0) && ((*s).row + row) <= (HEIGHT - 30)) {
    (*s).row += row;
  }
  if ((((*s).col + col) >= 0) && ((*s).col + col) <= (WIDTH - 30)) {
    (*s).col += col;
  }
  drawRectDMA((*s).row, (*s).col, 10, 10, BLACK);
}
