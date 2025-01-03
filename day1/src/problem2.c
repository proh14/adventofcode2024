#include <stdio.h>
#include <stdlib.h>

#define INPUT_FILE "../res/input.txt"
#define LINES 1000

void readLine(FILE *fp, int *left, int *right) {
  fscanf(fp, "%d   %d\n", left, right);
}

int compare(const void *x, const void *y) { return (*(int *)x - *(int *)y); }

int calculateScore(int *item, int *first, int *last) {
  if (item == NULL) {
    return 0;
  }
  int apeared = 1;
  int *temp = item + 1;
  while (temp <= last && *temp == *item) {
    apeared++;
    temp++;
  }
  temp = item - 1;
  while (temp >= first && *temp == *item) {
    apeared++;
    temp--;
  }
  return apeared * (*item);
}

int main(void) {
  int *leftSide = malloc(sizeof(int) * LINES);
  int *rightSide = malloc(sizeof(int) * LINES);

  FILE *fp = fopen(INPUT_FILE, "r");

  for (int i = 0; i < LINES; i++) {
    readLine(fp, &leftSide[i], &rightSide[i]);
  }

  qsort(rightSide, LINES, sizeof(int), compare);

  int ans = 0;

  for (int i = 0; i < LINES; i++) {
    int key = leftSide[i];
    int *found = bsearch(&key, rightSide, LINES, sizeof(int), compare);
    ans += calculateScore(found, rightSide, &rightSide[LINES - 1]);
  }

  printf("Answer = %d :)\n", ans);

  free(leftSide);
  free(rightSide);
  fclose(fp);
  return 0;
}
