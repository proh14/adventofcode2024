#include <stdio.h>
#include <stdlib.h>

#define INPUT_FILE "../res/input.txt"

#define BOX 'O'
#define ROBOT '@'
#define WALL '#'
#define SPACE '.'
#define WIDEBOX1 '['
#define WIDEBOX2 ']'

#define ROWS 50
#define COLS 100

#define MAX_BOXES 100

struct box {
  int row;
  int col;
  char ch;
};

struct box boxes[MAX_BOXES];
int box_size = 0;

char map[ROWS][COLS];

int cr = 0, cc = 0;
int dr = 0, dc = 0;

void readFile(FILE *fp) {
  int ch = 0, row = 0, col = 0;

  while (row < ROWS) {
    ch = getc(fp);
    if (ch == '\n' || col >= COLS) {
      col = 0;
      row++;
      continue;
    }
    switch (ch) {
    case ROBOT:
      cr = row;
      cc = col;
      map[row][col++] = ROBOT;
      map[row][col++] = SPACE;
      break;
    case BOX:
      map[row][col++] = WIDEBOX1;
      map[row][col++] = WIDEBOX2;
      break;
    default:
      map[row][col++] = ch;
      map[row][col++] = ch;
      break;
    }
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

int isInStack(int row, int col) {
  for (int i = 0; i < box_size; i++) {
    if (boxes[i].col == col && boxes[i].row == row)
      return 1;
  }
  return 0;
}

void push(int row, int col, char ch) {
  if (box_size >= MAX_BOXES) {
    printf("Overflow\n");
    return;
  }
  boxes[box_size].row = row;
  boxes[box_size].col = col;
  boxes[box_size].ch = ch;
  box_size++;
}

int goThrough(int row, int col) {
  if (isInStack(row, col))
    return 1;
  if (map[row][col] == WALL)
    return 0;
  if (map[row][col] == SPACE)
    return 1;

  push(row, col, map[row][col]);

  int through2 = 0;

  if (map[row][col] == WIDEBOX1) {
    through2 = goThrough(row, col + 1);
  }
  if (map[row][col] == WIDEBOX2) {
    through2 = goThrough(row, col - 1);
  }

  int through = goThrough(row + dr, col + dc);

  return through2 && through;
}

int comp(const void *a, const void *b) {
  const struct box *aa = a;
  const struct box *bb = b;
  if (dr == -1)
    return bb->row - aa->row;
  if (dr == 1)
    return aa->row - bb->row;
  if (dc == 1)
    return aa->col - bb->col;
  if (dc == -1)
    return bb->col - aa->col;
  return 0;
}

int countBoxes(void) {
  int ans = 0;
  for (int i = 0; i < ROWS; i++) {
    for (int j = 0; j < COLS; j++) {
      if (map[i][j] == WIDEBOX1)
        ans += 100 * i + j;
    }
  }
  return ans;
}

int main(void) {
  FILE *fp = fopen(INPUT_FILE, "r");
  readFile(fp);
  int ch = 0;
  getc(fp);
  while ((ch = getc(fp)) != EOF) {
    box_size = 0;
    directionFor(ch);
    int nr = cr + dr;
    int nc = cc + dc;

    int through = goThrough(nr, nc);

    if (!through)
      continue;

    qsort(boxes, box_size, sizeof(struct box), comp);

    for (int i = box_size - 1; i >= 0; i--) {
      int bc = boxes[i].col + dc;
      int br = boxes[i].row + dr;

      map[br][bc] = boxes[i].ch;
      map[br - dr][bc - dc] = SPACE;
    }
    map[cr][cc] = '.';
    cr += dr;
    cc += dc;
    map[cr][cc] = '@';
  }

  printf("Answer = %d :)", countBoxes());

  fclose(fp);
  return 0;
}
