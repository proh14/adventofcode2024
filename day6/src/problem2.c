#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define COLSIZE 131
#define INPUT_FILE "../res/input.txt"

static char *firstelm;
static char *lastelm;

static int visited_idx = 0;
static int visited_cap = 1000;

struct visited {
  char *pos;
  int dir;
};

char *move(int y, int x, char *str) {
  char *newpos = str + (x * COLSIZE + y);
  if (newpos > lastelm || newpos < firstelm || *newpos == '\n')
    return NULL;
  return newpos;
}

void visitedAdd(struct visited **visit, char *pos, int dir) {
  if (visited_idx >= visited_cap) {
    visited_cap *= 2; // Double the capacity each time
    *visit = realloc(*visit, sizeof(struct visited) * visited_cap);
    if (*visit == NULL) {
      perror("Realloc failed");
      exit(EXIT_FAILURE);
    }
  }

  (*visit)[visited_idx].pos = pos;
  (*visit)[visited_idx].dir = dir;
  visited_idx++;
}

int isVisited(struct visited *visit, char *pos, int dir) {
  for (int i = 0; i < visited_idx; i++) {
    if (visit[i].pos == pos && visit[i].dir == dir)
      return 1;
  }
  return 0;
}

int main(void) {
  FILE *fp = fopen(INPUT_FILE, "r");

  fseek(fp, 0, SEEK_END);
  long fsize = ftell(fp);
  fseek(fp, 0, SEEK_SET);

  char *content = malloc(fsize + 1);

  fread(content, 1, fsize, fp);

  firstelm = content;
  lastelm = &content[fsize - 1];

  char *guard = strchr(content, '^');

  int directions[4][2] = {
      {-1, 0}, // Up
      {0, 1},  // Right
      {1, 0},  // Down
      {0, -1}  // Left
  };

  int dir = 0;
  char *guardOrig = guard;

  int ans = 0;

  struct visited *visit = malloc(sizeof(struct visited) * visited_cap);

  for (int i = 0; i < fsize; i++) {
    guard = guardOrig;
    if (content[i] == '#' || content + i == guard || content[i] == '\n')
      continue;

    visited_idx = 0;
    dir = 0;

    printf("Testing: %d\n", i);

    while (1) {
      char *front = move(directions[dir][1], directions[dir][0], guard);
      if (isVisited(visit, front, dir)) {
        ans++;
        break;
      }

      visitedAdd(&visit, front, dir);

      if (!front || *front == '#' || front == content + i) {
        dir = (dir + 1) % 4;
      } else {
        guard = front;
      }

      if (!front || *guard == '\0') {
        break;
      }
    }
  }

  printf("Answer = %d :)\n", ans);

  free(visit);
  fclose(fp);
  free(content);

  return 0;
}
