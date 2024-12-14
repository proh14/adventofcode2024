#include <math.h>
#include <stdio.h>

#define INPUT_FILE "../res/input.txt"
#define BUFFSIZE 512

int ax;
int ay;
int bx;
int by;
int px;
int py;

int readFile(FILE *fp) {
  char buffer[BUFFSIZE];
  if (!fgets(buffer, BUFFSIZE, fp))
    return EOF;
  sscanf(buffer, "Button A: X%d, Y%d\n", &ax, &ay);

  if (!fgets(buffer, BUFFSIZE, fp))
    return EOF;
  sscanf(buffer, "Button B: X%d, Y%d\n", &bx, &by);

  if (!fgets(buffer, BUFFSIZE, fp))
    return EOF;
  sscanf(buffer, "Prize: X=%d, Y=%d", &px, &py);

  getc(fp);
  return 0;
}

int solveEqu(void) {
  double pa = (double)(px * by - py * bx) / (ax * by - ay * bx);
  double pb = (px - ax * pa) / bx;

  if (ceil(pa) != pa || ceil(pb) != pb)
    return 0;

  return (int)pa * 3 + pb;
}

int main(void) {
  FILE *fp = fopen(INPUT_FILE, "r");
  int ans = 0;
  while (readFile(fp) != EOF) {
    ans += solveEqu();
  }
  printf("Answer = %d :)\n", ans);
  fclose(fp);
  return 0;
}
