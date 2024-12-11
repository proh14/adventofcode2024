#include <stdio.h>

#define INPUT_FILE "../res/input.txt"

#define COLS 45
#define ROWS 45

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

int isTrail(int row, int col, int h) {

  if (row < 0 || row >= ROWS || col < 0 || col >= COLS)
    return 0;
  if (content[row][col] != h)
    return 0;
  if (content[row][col] == 9) {
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
        ans += isTrail(i, j, 0);
      }
    }
  }

  printf("Answer = %d :)\n", ans);

  fclose(fp);
  return 0;
}
