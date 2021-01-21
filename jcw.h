#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#define __USE_POSIX199309
#define _POSIX_C_SOURCE 199309L
#include <time.h>

uint8_t JCW_board[JCW_HEIGHT+2][JCW_WIDTH+2] = {0};

static void JCW_swap(uint8_t* a, uint8_t* b) {
  uint8_t temp = *a;
  *a = *b;
  *b = temp;
}

static void JCW_shuffle(uint8_t* array) {
  int min = 0;
  int max = JCW_WIDTH;
  for (int i = JCW_WIDTH - 1; i > 0; i--) {
    int j = min + (int)rand() % (max - min);
    JCW_swap(&array[i], &array[j]);
  }
}

static uint8_t JCW_countNeighbors(int y, int x) {
  // top-left
  return JCW_board[y-1][x-1] +
  // top-middle
  JCW_board[y-1][x] +
  // top-right
  JCW_board[y-1][x+1] +
  // left
  JCW_board[y][x-1] +
  // right
  JCW_board[y][x+1] +
  // bottom-left
  JCW_board[y+1][x-1] +
  // bottom-middle
  JCW_board[y+1][x] +
  // bottom-right
  JCW_board[y+1][x+1];
}

void JCW_nextGeneration(void) {
  uint8_t neighbors[JCW_HEIGHT+2][JCW_WIDTH+2];
  for (int y = 1; y < JCW_HEIGHT+1; y++) {
    for (int x = 1; x < JCW_WIDTH+1; x++) {
      neighbors[y][x] = JCW_countNeighbors(y, x);
    }
  }
  for (int y = 1; y < JCW_HEIGHT+1; y++) {
    for (int x = 1; x < JCW_WIDTH+1; x++) {
      switch(neighbors[y][x]) {
        case 2:
          // Do nothing
          break;
        case 3:
          JCW_board[y][x] = 1;
          break;
        default:
          JCW_board[y][x] = 0;
          break;
      }
    }
  }
}

void JCW_randomize(void) {
  srand((unsigned int)time(NULL));
}

void JCW_initBoard(void) {
  for (int y = 1; y < JCW_HEIGHT+1; y++) {
    for (int x = 1; x < JCW_LIVE_CELLS+1; x++) {
      JCW_board[y][x] = 1;
    }
    JCW_shuffle(&JCW_board[y][1]);
  }
}
