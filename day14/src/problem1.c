#include <stdio.h>

#define INPUT_FILE "../res/input.txt"
#define BUFFSIZE 128
#define SECONDS 100

#define WIDTH 101
#define HEIGHT 103

int cx, cy, vx, vy;

int readLine(FILE *fp) {
  char buffer[BUFFSIZE];
  if (!fgets(buffer, BUFFSIZE, fp)) {
    return EOF;
  }
  sscanf(buffer, "p=%d,%d v=%d,%d\n", &cx, &cy, &vx, &vy);
  return 0;
}

void robotAfterSeconds(int seconds) {
  for (int i = 0; i < seconds; i++) {
    cx = (vx + cx + WIDTH) % WIDTH;
    cy = (vy + cy + HEIGHT) % HEIGHT;
  }
}

int main(void) {
  FILE *fp = fopen(INPUT_FILE, "r");
  int ans = 0;
  int quad[4] = {0};
  while (readLine(fp) != EOF) {
    robotAfterSeconds(SECONDS);
    if (cx < WIDTH / 2 && cy < HEIGHT / 2)
      quad[0]++;
    if (cx > WIDTH / 2 && cy < HEIGHT / 2)
      quad[1]++;
    if (cx < WIDTH / 2 && cy > HEIGHT / 2)
      quad[2]++;
    if (cx > WIDTH / 2 && cy > HEIGHT / 2)
      quad[3]++;
  }

  ans = quad[0] * quad[1] * quad[2] * quad[3];

  printf("Answer = %d :)\n", ans);

  fclose(fp);
  return 0;
}
