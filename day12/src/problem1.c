#include <stdio.h>

#define INPUT_FILE "../res/input.txt"

#define COLS 140
#define ROWS 140

char content[ROWS][COLS];
int visited[ROWS][COLS] = {0};

void readFile(FILE *fp) {
  int ch = 0;
  int row = 0;
  int col = 0;

  while ((ch = getc(fp)) != EOF) {
    if (ch != '\n') {
      content[row][col++] = ch;
      continue;
    }
    row++;
    col = 0;
  }
}

int dr[] = {0, 0, -1, 1};
int dc[] = {-1, 1, 0, 0};

int isValid(int row, int col, char ch) {
  return row >= 0 && row < ROWS && col >= 0 && col < COLS &&
         !visited[row][col] && content[row][col] == ch;
}

void performDFS(int row, int col, char ch, int *area, int *prem) {
  visited[row][col] = 1;
  (*area)++;

  for (int i = 0; i < 4; i++) {
    int nr = row + dr[i];
    int nc = col + dc[i];

    if (isValid(nr, nc, ch))
      performDFS(nr, nc, ch, area, prem);
    else if (nr < 0 || nr >= ROWS || nc < 0 || nc >= COLS ||
             content[nr][nc] != ch)
      (*prem)++;
  }
}

int main(void) {
  FILE *fp = fopen(INPUT_FILE, "r");

  readFile(fp);

  int ans = 0;

  for (int i = 0; i < ROWS; i++) {
    for (int j = 0; j < COLS; j++) {
      if (visited[i][j])
        continue;
      int area = 0, prem = 0;
      performDFS(i, j, content[i][j], &area, &prem);
      ans += area * prem;
    }
  }

  printf("Answer = %d :)\n", ans);

  fclose(fp);
  return 0;
}
