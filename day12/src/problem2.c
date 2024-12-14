#include <stdio.h>
#include <stdlib.h>

#define INPUT_FILE "../res/input.txt"

#define COLS 140
#define ROWS 140
#define MAX_EDGES 1000

char content[ROWS][COLS];
int visited[ROWS][COLS] = {0};

struct edge {
  int row;
  int col;
  int dirx;
  int diry;
};

struct edge edges[MAX_EDGES];
int edges_size = 0;

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

void performDFS(int row, int col, char ch, int *area) {
  visited[row][col] = 1;
  (*area)++;

  for (int i = 0; i < 4; i++) {
    int nr = row + dr[i];
    int nc = col + dc[i];

    if (isValid(nr, nc, ch))
      performDFS(nr, nc, ch, area);
    else if (nr < 0 || nr >= ROWS || nc < 0 || nc >= COLS ||
             content[nr][nc] != ch) {
      if (edges_size >= MAX_EDGES) {
        printf("cooked, allocate more memory in line 8 im lazy :)\n");
        exit(1);
      }
      edges[edges_size].row = nr;
      edges[edges_size].col = nc;
      edges[edges_size].dirx = col - nc;
      edges[edges_size++].diry = row - nr;
    }
  }
}

int areConnected(struct edge a, struct edge b) {
  return ((a.row == b.row && abs(a.col - b.col) == 1) ||
          (a.col == b.col && abs(a.row - b.row) == 1)) &&
         a.dirx == b.dirx && a.diry == b.diry;
}

void performEdgeDFS(int current, int *visited_edges) {
  visited_edges[current] = 1;
  for (int i = 0; i < edges_size; i++) {
    if (!visited_edges[i] && areConnected(edges[current], edges[i])) {
      performEdgeDFS(i, visited_edges);
    }
  }
}

int countSides(void) {
  int *visited_edges = calloc(edges_size, sizeof(int));
  int sides = 0;
  for (int i = 0; i < edges_size; i++) {
    if (visited_edges[i] == 1)
      continue;
    sides++;
    performEdgeDFS(i, visited_edges);
  }
  return sides;
}

int main(void) {
  FILE *fp = fopen(INPUT_FILE, "r");

  readFile(fp);

  int ans = 0;

  for (int i = 0; i < ROWS; i++) {
    for (int j = 0; j < COLS; j++) {
      if (visited[i][j])
        continue;
      edges_size = 0;
      int area = 0, sides = 0;
      performDFS(i, j, content[i][j], &area);
      sides = countSides();
      ans += area * sides;
    }
  }

  printf("Answer = %d :)\n", ans);

  fclose(fp);
  return 0;
}
