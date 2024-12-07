#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define INPUT_FILE "../res/input.txt"

#define COLSIZE 141

static char *lastelm;
static char *firstelm;

int checkNorth(char *ptr) {
  if (ptr - COLSIZE * 3 < firstelm)
    return 0;

  char c1 = *(ptr - COLSIZE * 1);
  char c2 = *(ptr - COLSIZE * 2);
  char c3 = *(ptr - COLSIZE * 3);

  if (c1 == 'M' && c2 == 'A' && c3 == 'S')
    return 1;

  return 0;
}
int checkSouth(char *ptr) {
  if (ptr + COLSIZE * 3 > lastelm)
    return 0;

  char c1 = *(ptr + COLSIZE * 1);
  char c2 = *(ptr + COLSIZE * 2);
  char c3 = *(ptr + COLSIZE * 3);

  if (c1 == 'M' && c2 == 'A' && c3 == 'S')
    return 1;

  return 0;
}
int checkEast(char *ptr) {
  if (ptr + 3 > lastelm)
    return 0;

  char c1 = *(ptr + 1);
  char c2 = *(ptr + 2);
  char c3 = *(ptr + 3);

  if (c1 == 'M' && c2 == 'A' && c3 == 'S')
    return 1;

  return 0;
}
int checkWest(char *ptr) {
  if (ptr - 3 < firstelm || ptr - 1 > lastelm)
    return 0;

  char c1 = *(ptr - 1);
  char c2 = *(ptr - 2);
  char c3 = *(ptr - 3);

  if (c1 == 'M' && c2 == 'A' && c3 == 'S')
    return 1;

  return 0;
}
int checkNorthEast(char *ptr) {
  if ((ptr + 1) - (COLSIZE * 3) < firstelm ||
      (ptr + 1) - (COLSIZE * 3) > lastelm)
    return 0;

  char c1 = *((ptr + 1) - (COLSIZE * 1));
  char c2 = *((ptr + 2) - (COLSIZE * 2));
  char c3 = *((ptr + 3) - (COLSIZE * 3));

  if (c1 == 'M' && c2 == 'A' && c3 == 'S')
    return 1;

  return 0;
}
int checkNorthWest(char *ptr) {
  if ((ptr - 3) - (COLSIZE * 3) < firstelm ||
      ((ptr - 1) - (COLSIZE * 1) > lastelm))
    return 0;

  char c1 = *((ptr - 1) - (COLSIZE * 1));
  char c2 = *((ptr - 2) - (COLSIZE * 2));
  char c3 = *((ptr - 3) - (COLSIZE * 3));

  if (c1 == 'M' && c2 == 'A' && c3 == 'S')
    return 1;

  return 0;
}
int checkSouthEast(char *ptr) {
  if ((ptr + 3) + (COLSIZE * 3) > lastelm ||
      ((ptr + 1) + (COLSIZE * 1) < firstelm))
    return 0;

  char c1 = *((ptr + 1) + (COLSIZE * 1));
  char c2 = *((ptr + 2) + (COLSIZE * 2));
  char c3 = *((ptr + 3) + (COLSIZE * 3));

  if (c1 == 'M' && c2 == 'A' && c3 == 'S')
    return 1;

  return 0;
}
int checkSouthWest(char *ptr) {
  if ((ptr - 3) + (COLSIZE * 3) > lastelm ||
      ((ptr - 1) + (COLSIZE * 1) < firstelm))
    return 0;

  char c1 = *((ptr - 1) + (COLSIZE * 1));
  char c2 = *((ptr - 2) + (COLSIZE * 2));
  char c3 = *((ptr - 3) + (COLSIZE * 3));

  if (c1 == 'M' && c2 == 'A' && c3 == 'S')
    return 1;

  return 0;
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
  while ((ptr = strchr(ptr, 'X')) != NULL) {
    ans += checkEast(ptr);
    ans += checkWest(ptr);
    ans += checkSouth(ptr);
    ans += checkNorth(ptr);
    ans += checkNorthWest(ptr);
    ans += checkNorthEast(ptr);
    ans += checkSouthWest(ptr);
    ans += checkSouthEast(ptr);
    ptr++;
  }

  printf("Answer = %d :)\n", ans);

  free(content);
  fclose(fp);
  return 0;
}
