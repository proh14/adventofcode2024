#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define INPUT_FILE "../res/input.txt"

#define COLSIZE 141

static char *lastelm;
static char *firstelm;

int isXMAS(char *ptr) {
  if (ptr + 1 - COLSIZE < firstelm || ptr - 1 - COLSIZE < firstelm ||
      ptr + 1 + COLSIZE < firstelm || ptr - 1 + COLSIZE < firstelm ||
      ptr + 1 - COLSIZE > lastelm || ptr - 1 - COLSIZE > lastelm ||
      ptr + 1 + COLSIZE > lastelm || ptr - 1 + COLSIZE > lastelm)
    return 0;

  char c[4];
  c[0] = *(ptr + 1 - COLSIZE); // NorthEast
  c[1] = *(ptr - 1 - COLSIZE); // NorthWest
  c[2] = *(ptr + 1 + COLSIZE); // SouthEast
  c[3] = *(ptr - 1 + COLSIZE); // SouthWest
  int numM = 0;
  int numS = 0;

  for (int i = 0; i < 4; i++) {
    if (c[i] == 'M')
      numM++;
    if (c[i] == 'S')
      numS++;
  }

  if (c[0] == c[3] && c[1] == c[2])
    return 0;

  if (numM != 2 || numS != 2)
    return 0;

  return 1;
}

int main(void) {
  FILE *fp = fopen(INPUT_FILE, "r");

  fseek(fp, 0, SEEK_END);
  long fsize = ftell(fp);
  fseek(fp, 0, SEEK_SET);

  char *content = malloc(sizeof(char) * fsize + 1);
  lastelm = &content[fsize + 1];
  firstelm = content;

  fread(content, fsize, 1, fp);

  char *ptr = content;

  int ans = 0;
  while ((ptr = strchr(ptr, 'A')) != NULL) {
    ans += isXMAS(ptr);
    ptr++;
  }

  printf("Answer = %d :)\n", ans);

  free(content);
  fclose(fp);
  return 0;
}
