#include <stdio.h>

#define _IMPLEMENT_QUEUE_H_
#include "queue.h"

#define INPUT_FILE "../res/input.txt"

#define ROWS 141
#define COLS 141

int sr, sc, er, ec;

char map[ROWS][COLS];

void readFile(FILE *fp) {
  int row = 0, col = 0;
  int ch;
  while ((ch = fgetc(fp)) != EOF) {
    if (ch == '\n') {
      row++;
      col = 0;
      continue;
    }
    if (ch == 'S') {
      sr = row;
      sc = col;
    }
    if (ch == 'E') {
      er = row;
      ec = col;
    }
    map[row][col] = ch;
    col++;
  }
}

int performBFS(void) {
  int dr[] = {0, 0, 1, -1};
  int dc[] = {1, -1, 0, 0};

  int visited[ROWS][COLS] = {0};

  int steps = 0;

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
        return item->steps + 1;
      }

      enqueue(&q, nr, nc, item->steps + 1);
    }
  }
  queueFree(&q);
  return steps;
}

int main(void) {
  FILE *fp = fopen(INPUT_FILE, "r");

  readFile(fp);

  int ans = 0;
  int base = performBFS();

  for (int i = 0; i < ROWS; i++) {
    for (int j = 0; j < COLS; j++) {
      if (map[i][j] == '#') {
        map[i][j] = '.';
        int temp = performBFS();
        if (base - temp >= 100) {
          ans++;
        }
        map[i][j] = '#';
      }
    }
  }

  printf("Answer =  %d :)\n", ans);

  fclose(fp);
  return 0;
}
