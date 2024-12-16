#include <stdio.h>

#define INPUT_FILE "../res/input.txt"

#define BOX 'O'
#define ROBOT '@'
#define WALL '#'
#define SPACE '.'

#define ROWS 50
#define COLS 50

#define MAX_BOXES 100

struct box {
  int row;
  int col;
};

char map[ROWS][COLS];

int cr, cc;
int dr, dc;

void readFile(FILE *fp) {
  int ch = 0, row = 0, col = 0;

  while (row < ROWS && (ch = getc(fp)) != EOF) {
    if (ch != '\n') {
      if (ch == ROBOT) {
        cr = row;
        cc = col;
      }
      map[row][col++] = ch;
      continue;
    }
    row++;
    col = 0;
  }
}

void directionFor(int ch) {
  switch (ch) {
  case '^':
    dr = -1;
    dc = 0;
    break;
  case 'v':
    dr = 1;
    dc = 0;
    break;
  case '>':
    dr = 0;
    dc = 1;
    break;
  case '<':
    dr = 0;
    dc = -1;
    break;
  default:
    dr = 0;
    dc = 0;
    break;
  }
}

int main(void) {
  FILE *fp = fopen(INPUT_FILE, "r");
  readFile(fp);

  int ch = 0;
  getc(fp);
  while ((ch = getc(fp)) != EOF) {
    directionFor(ch);
    int nr = cr + dr;
    int nc = cc + dc;

    struct box boxes[MAX_BOXES];
    int box_size = 0;

    while (map[nr][nc] == BOX && box_size < MAX_BOXES) {
      boxes[box_size].row = nr;
      boxes[box_size++].col = nc;
      nr += dr;
      nc += dc;
      if (nr < 0 || nr >= ROWS || nc < 0 || nc >= COLS)
        break;
    }

    if (map[nr][nc] == WALL)
      continue;

    for (int i = box_size - 1; i >= 0; i--) {
      int bc = boxes[i].col + dc;
      int br = boxes[i].row + dr;

      map[br][bc] = BOX;
      map[br - dr][bc - dc] = SPACE;
    }
    cr += dr;
    cc += dc;
  }

  int ans = 0;

  for (int i = 0; i < ROWS; i++) {
    for (int j = 0; j < COLS; j++) {
      if (map[i][j] == BOX)
        ans += i * 100 + j;
    }
  }

  printf("Answer = %d :)\n", ans);

  fclose(fp);
  return 0;
}
