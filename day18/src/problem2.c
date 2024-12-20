#include <stdio.h>

#define _IMPLEMENT_QUEUE_H_
#include "queue.h"

#define INPUT_FILE "../res/input.txt"

#define ROWS 71
#define COLS 71

#define FALLING 1024
#define MAX_LINES 3450

void constructMap(FILE *fp, char map[ROWS][COLS], int n) {
  for (int i = 0; i < n; i++) {
    int x, y;
    fscanf(fp, "%d,%d\n", &x, &y);
    map[x][y] = '#';
  }
}

int performBFS(int sr, int sc, int er, int ec, int n, FILE *fp) {
  int dr[] = {0, 0, 1, -1};
  int dc[] = {1, -1, 0, 0};

  fseek(fp, 0, SEEK_SET);

  int visited[ROWS][COLS] = {0};
  char map[ROWS][COLS] = {0};

  constructMap(fp, map, n);

  struct queue q;
  queueInit(&q, 1024);
  enqueue(&q, sr, sc, 0);

  visited[sr][sc] = 1;

  while (!isEmpty(&q)) {
    struct queueItem *item = dequeue(&q);
    int cr = item->row;
    int cc = item->col;

    for (int i = 0; i < 4; i++) {
      int nr = cr + dr[i];
      int nc = cc + dc[i];

      if (nr < 0 || nr >= ROWS || nc < 0 || nc >= COLS) {
        continue;
      }

      if (map[nr][nc] == '#') {
        continue;
      }

      if (visited[nr][nc]) {
        continue;
      }

      visited[nr][nc] = 1;

      if (nr == er && nc == ec) {
        return 1;
      }

      enqueue(&q, nr, nc, 0);
    }
  }
  queueFree(&q);
  return 0;
}

void lineNumberToCoords(FILE *fp, int lines, int *x, int *y) {
  fseek(fp, 0, SEEK_SET);
  for (int i = 0; i < lines; i++) {
    fscanf(fp, "%d,%d\n", x, y);
  }
}

int main(void) {

  FILE *fp = fopen(INPUT_FILE, "r");

  int l = FALLING, r = MAX_LINES - 1;

  while (l < r) {
    int m = (l + r) / 2;
    if (!performBFS(0, 0, ROWS - 1, COLS - 1, m, fp)) {
      r = m;
    } else {
      l = m + 1;
    }
  }

  int x, y;

  lineNumberToCoords(fp, l, &x, &y);

  printf("Answer = %d,%d :)\n", x, y);

  fclose(fp);
  return 0;
}
