#include <stdio.h>

#define INPUT_FILE "../res/input.txt"

#define COLS 45
#define ROWS 45
#define NINES 500

struct nine {
  int row;
  int col;
};

struct nine nines[500];
int nine_size = 0;
int content[ROWS][COLS];

void readFile(FILE *fp) {
  int ch = 0;
  int row = 0;
  int col = 0;

  while ((ch = getc(fp)) != EOF) {
    if (ch != '\n') {
      content[row][col++] = ch - '0';
      continue;
    }
    row++;
    col = 0;
  }
}

int doesNineExist(int row, int col) {
  for (int i = 0; i < nine_size; i++) {
    if (nines[i].col == col && nines[i].row == row) {
      return 1;
    }
  }
  return 0;
}

int isTrail(int row, int col, int h) {

  if (row < 0 || row >= ROWS || col < 0 || col >= COLS)
    return 0;
  if (content[row][col] != h)
    return 0;
  if (content[row][col] == 9) {
    if (!doesNineExist(row, col)) {
      nines[nine_size].row = row;
      nines[nine_size++].col = col;
    }
    return 1;
  }

  int up = 0, down = 0, left = 0, right = 0;

  down = isTrail(row + 1, col, h + 1);
  up = isTrail(row - 1, col, h + 1);
  right = isTrail(row, col + 1, h + 1);
  left = isTrail(row, col - 1, h + 1);

  return up + down + left + right;
}

int main(void) {
  FILE *fp = fopen(INPUT_FILE, "r");

  readFile(fp);

  int ans = 0;
  for (int i = 0; i < ROWS; i++) {
    for (int j = 0; j < COLS; j++) {
      if (content[i][j] == 0) {
        nine_size = 0;
        isTrail(i, j, 0);
        ans += nine_size;
      }
    }
  }

  printf("Answer = %d :)\n", ans);

  fclose(fp);
  return 0;
}
