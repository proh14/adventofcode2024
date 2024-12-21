#include <limits.h>
#include <stdio.h>
#include <string.h>

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

int performBFS(int sr, int sc, int er, int ec, int dists[ROWS][COLS]) {
  int dr[] = {0, 0, 1, -1};
  int dc[] = {1, -1, 0, 0};

  int visited[ROWS][COLS] = {0};

  struct queue q;
  queueInit(&q, 1024);
  enqueue(&q, sr, sc, 0);

  visited[sr][sc] = 1;
  dists[sr][sc] = 0;

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

      dists[nr][nc] = dists[cr][cc] + 1;

      if (nr == er && nc == ec) {
        queueFree(&q);
        return item->steps + 1;
      }

      enqueue(&q, nr, nc, item->steps + 1);
    }
  }
  queueFree(&q);
  return -1;
}

int manhattanDistance(int r1, int c1, int r2, int c2) {
  return abs(r1 - r2) + abs(c1 - c2);
}

int main(void) {
  FILE *fp = fopen(INPUT_FILE, "r");

  int distancesFromStart[ROWS][COLS];
  int distancesFromEnd[ROWS][COLS];

  memset(distancesFromStart, -1, sizeof(distancesFromStart));
  memset(distancesFromEnd, -1, sizeof(distancesFromEnd));

  readFile(fp);

  int ans = 0;
  int base = performBFS(sr, sc, er, ec, distancesFromStart);
  performBFS(er, ec, sr, sc, distancesFromEnd);

  for (int i = 0; i < ROWS; i++) {
    for (int j = 0; j < COLS; j++) {
      for (int k = 0; k < ROWS; k++) {
        for (int l = 0; l < COLS; l++) {
          if (distancesFromStart[i][j] == -1 || distancesFromEnd[k][l] == -1) {
            continue;
          }
          if (manhattanDistance(k, l, i, j) <= 20 &&
              manhattanDistance(k, l, i, j) > 0) {
            int dist = distancesFromStart[i][j] + distancesFromEnd[k][l] +
                       manhattanDistance(i, j, k, l);
            if (dist > 0 && dist <= base - 100) {
              ans++;
            }
          }
        }
      }
    }
  }

  printf("Answer =  %d :)\n", ans);

  fclose(fp);
  return 0;
}
