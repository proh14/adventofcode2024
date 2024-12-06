#include <stdio.h>
#include <stdlib.h>

#define INPUT_FILE "../res/input.txt"
#define LINES 1000

void readLine(FILE *fp, int *left, int *right) {
  fscanf(fp, "%d   %d\n", left, right);
}

int compare(const void *x, const void *y) { return (*(int *)x - *(int *)y); }

int main(void) {
  int *leftSide = malloc(sizeof(int) * LINES);
  int *rightSide = malloc(sizeof(int) * LINES);

  FILE *fp = fopen(INPUT_FILE, "r");

  for (int i = 0; i < LINES; i++) {
    readLine(fp, &leftSide[i], &rightSide[i]);
  }

  qsort(leftSide, LINES, sizeof(int), compare);
  qsort(rightSide, LINES, sizeof(int), compare);

  int ans = 0;

  for (int i = 0; i < LINES; i++) {
    int dis = leftSide[i] - rightSide[i];
    dis = dis > 0 ? dis : -dis;
    ans += dis;
  }

  printf("Answer = %d :)\n", ans);

  free(leftSide);
  free(rightSide);
  fclose(fp);
  return 0;
}
