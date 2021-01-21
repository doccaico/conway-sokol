#include <stdlib.h>

#define JCW_WIDTH 40
#define JCW_HEIGHT 20
#define JCW_LIVE_CELLS 20
// #define WAIT = 150
#include "jcw.h"


void screenClear(void) {
  int ret = system("printf '\33c\033[3J\33c'");
  if (ret == -1) {
    fprintf(stderr, "ERROR: " "cannot clear the screen\n");
    exit(-1);
  }
}

void outputBoard(void) {
  for (int y = 1; y < JCW_HEIGHT+1; y++) {
    for (int x = 1; x < JCW_WIDTH+1; x++) {
      putchar(JCW_board[y][x] == 1 ? '*' : ' ');
    }
    putchar('\n');
  }
}

int main(void) {

  struct timespec req = {0, 100*1000000}; // 0.1 sec

  JCW_randomize();
  JCW_initBoard();

  for (;;) {
    screenClear();
    JCW_nextGeneration();
    outputBoard();
    if (nanosleep(&req, NULL) == -1) {
      perror("ERROR: ");
      return -1;
    }
  }
  return 0;
}
