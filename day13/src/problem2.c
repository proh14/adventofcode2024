#include <stdint.h>
#include <stdio.h>

#define INPUT_FILE "../res/input.txt"
#define BUFFSIZE 512

int ax;
int ay;
int bx;
int by;
uint64_t px;
uint64_t py;

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
  sscanf(buffer, "Prize: X=%lu, Y=%lu", &px, &py);
  px += 10000000000000;
  py += 10000000000000;

  getc(fp);
  return 0;
}

uint64_t solveEqu(void) {

  long long denominator = (ax * by - ay * bx);

  long long numerator_pa = (px * by - py * bx);
  if (numerator_pa % denominator != 0) {
    return 0;
  }
  long long pa = numerator_pa / denominator;

  long long numerator_pb = (px - ax * pa);
  if (numerator_pb % bx != 0) {
    return 0;
  }
  long long pb = numerator_pb / bx;

  return (uint64_t)(pa * 3 + pb);
}

int main(void) {
  FILE *fp = fopen(INPUT_FILE, "r");
  uint64_t ans = 0;
  while (readFile(fp) != EOF) {
    ans += solveEqu();
  }
  printf("Answer = %lu :)\n", ans);
  fclose(fp);
  return 0;
}
