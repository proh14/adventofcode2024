#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define _IMPLEMENT_QUEUE_H_
#include "queue.h"

#define INPUT_FILE "../res/input.txt"

#define ROWS 71
#define COLS 71

#define FALLING 1024

char **map;

int **visited;

void allocateMap(void) {
  map = malloc(ROWS * sizeof(char *));
  for (int i = 0; i < ROWS; i++) {
    map[i] = malloc(COLS * sizeof(char));
    memset(map[i], '.', COLS);
  }

  visited = malloc(ROWS * sizeof(int *));
  for (int i = 0; i < ROWS; i++) {
    visited[i] = malloc(COLS * sizeof(int));
    memset(visited[i], 0, COLS * sizeof(int));
  }
}

void freeMap(void) {
  for (int i = 0; i < ROWS; i++) {
    free(map[i]);
  }
  free(map);
  for (int i = 0; i < ROWS; i++) {
    free(visited[i]);
  }
  free(visited);
}

void constructMap(FILE *fp) {
  for (int i = 0; i < FALLING; i++) {
    int x, y;
    fscanf(fp, "%d,%d\n", &x, &y);
    map[x][y] = '#';
  }
}

int performBFS(int sr, int sc, int er, int ec) {
  int dr[] = {0, 0, 1, -1};
  int dc[] = {1, -1, 0, 0};

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

  allocateMap();
  constructMap(fp);

  printf("Answer = %d :)\n", performBFS(0, 0, ROWS - 1, COLS - 1));

  freeMap();
  fclose(fp);
  return 0;
}
