#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define COLSIZE 131
#define INPUT_FILE "../res/input.txt"

static char *firstelm;
static char *lastelm;

char *move(int y, int x, char *str) {
  char *newpos = str + (x * COLSIZE + y);
  if (newpos > lastelm || newpos < firstelm || *newpos == '\n')
    return NULL;
  return newpos;
}

int countNumX(char *string) {
  char *ptr = string;
  int numX = 0;
  while (*ptr) {
    if (*ptr == 'X')
      numX++;
    ptr++;
  }
  return numX;
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

  *guard = 'X';

  while (1) {
    char *front = move(directions[dir][1], directions[dir][0], guard);

    if (!front || *front == '#') {
      dir = (dir + 1) % 4;
    } else {
      guard = front;
      if (*guard == '.') {
        *guard = 'X';
      }
    }

    if (!front || *guard == '\0') {
      break;
    }
  }

  printf("Answer = %d :)\n", countNumX(content));

  fclose(fp);
  free(content);
  return 0;
}
