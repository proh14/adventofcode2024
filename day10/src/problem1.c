#include <stdio.h>
#include <stdlib.h>

#define INPUT_FILE "../res/input.txt"

#define COLS 8
#define ROWS 8

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
  if (content[row][col] != h) {
    return 1;
  }

  int up = isTrail(row + 1, col, h + 1);
  int down = isTrail(row - 1, col, h + 1);
  int left = isTrail(row, col + 1, h + 1);
  int right = isTrail(row, col - 1, h + 1);

  return up + down + left + right;
}

int countTrails(int row, int col) {

  int score = 0;

  score += isTrail(row + 1, col, 1);
  score += isTrail(row - 1, col, 1);
  score += isTrail(row, col + 1, 1);
  score += isTrail(row, col - 1, 1);

  return score;
}

int main(void) {
  FILE *fp = fopen(INPUT_FILE, "r");

  readFile(fp);

  int ans = 0;

  for (int i = 0; i < ROWS; i++) {
    for (int j = 0; j < COLS; j++) {
      if (content[i][j] == 0)
        ans += countTrails(i, j);
    }
  }

  printf("Answer = %d :)", ans);

  fclose(fp);
  return 0;
}
