#include <stdio.h>

#define INPUT_FILE "../res/input.txt"
#define OUTPUT_FILE "./output.txt"

#define BUFFSIZE 128
#define SECONDS 10000
#define ROBOTS 500

#define WIDTH 101
#define HEIGHT 103

struct robot {
  int x;
  int y;
  int vx;
  int vy;
};

char map[WIDTH][HEIGHT];

struct robot robots[ROBOTS];
int robots_size = 0;

int readLine(FILE *fp) {
  char buffer[BUFFSIZE];
  if (!fgets(buffer, BUFFSIZE, fp)) {
    return EOF;
  }
  sscanf(buffer, "p=%d,%d v=%d,%d\n", &robots[robots_size].x,
         &robots[robots_size].y, &robots[robots_size].vx,
         &robots[robots_size].vy);
  robots_size++;
  return 0;
}

void calculateNewpos(int index) {
  robots[index].x = (robots[index].vx + robots[index].x + WIDTH) % WIDTH;
  robots[index].y = (robots[index].vy + robots[index].y + HEIGHT) % HEIGHT;
}

void emptyMap(void) {
  for (int i = 0; i < WIDTH; i++) {
    for (int j = 0; j < HEIGHT; j++) {
      map[i][j] = '.';
    }
  }
}

int main(void) {
  FILE *fp = fopen(INPUT_FILE, "r");
  FILE *output = fopen(OUTPUT_FILE, "w");
  while (readLine(fp) != EOF)
    ;

  emptyMap();
  for (int i = 0; i < SECONDS; i++) {
    for (int j = 0; j < robots_size; j++) {
      calculateNewpos(j);
      map[robots[j].x][robots[j].y] = '#';
    }
    fprintf(output, "Iteration: %d\n", i + 1);
    for (int col = 0; col < WIDTH; col++) {
      for (int row = 0; row < HEIGHT; row++) {
        putc(map[col][row], output);
      }
      putc('\n', output);
    }
    emptyMap();
  }

  printf("Answer = look at output.txt and find the tree smh :(\n");

  /* Really bad problem imo, it never specified what the tree looks like and it
   * wanted you to assume something. Anyways, I decided not to waste time on
   * this and just print the map out to output.txt on each iteration. To find
   * the tree just search about like 10 hashtags ('#') and scroll up until you
   * find Iteration: number and submit that ;) Be careful as the output file
   * takes alot of space.
   * */

  fclose(output);
  fclose(fp);
  return 0;
}
