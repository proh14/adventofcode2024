#include <stdio.h>
#include <stdlib.h>

#define IMPLEMENT_PRIORITYQUEUE_H
#include <priorityqueue.h>

#define INPUT_FILE "../res/input.txt"

#define COLS 141
#define ROWS 141

char content[ROWS][COLS];

int cr = 0, cc = 0;

void readFile(FILE *fp) {
  int ch = 0;
  int row = 0;
  int col = 0;

  while ((ch = getc(fp)) != EOF) {
    if (ch == '\n') {
      row++;
      col = 0;
      continue;
    }
    if (ch == 'S') {
      cr = row;
      cc = col;
    }
    content[row][col++] = ch;
  }
}

int dr[] = {0, 1, 0, -1};
int dc[] = {1, 0, -1, 0};

int seen[ROWS][COLS][4] = {0};

int directionToNumber(int dy, int dx) {
  for (int i = 0; i < 4; i++) {
    if (dr[i] == dy && dc[i] == dx)
      return i;
  }
  return -1;
}

int performDijkstra(void) {
  struct priorityQueue pq = {0};
  pq.items = malloc(sizeof(struct data) * MAX_DATA);
  enqueue(&pq, (struct data){cr, cc, 0, 1, 0});

  while (pq.size > 0) {
    struct data item = dequeue(&pq);
    int dir = directionToNumber(item.dr, item.dc);
    seen[item.cr][item.cc][dir] = 1;

    if (content[item.cr][item.cc] == 'E') {
      free(pq.items);
      return item.cost;
    }

    int clockwise = (dir + 1) % 4;
    int counterclockwise = (dir + 3) % 4;
    struct data nextdata[3] = {
        {item.cr + item.dr, item.cc + item.dc, item.dr, item.dc, item.cost + 1},
        {item.cr, item.cc, dr[clockwise], dc[clockwise], item.cost + 1000},
        {item.cr, item.cc, dr[counterclockwise], dc[counterclockwise],
         item.cost + 1000},
    };

    for (int i = 0; i < 3; i++) {
      dir = directionToNumber(nextdata[i].dr, nextdata[i].dc);
      if (nextdata[i].cr < 0 || nextdata[i].cc < 0 || nextdata[i].cr >= ROWS ||
          nextdata[i].cc >= COLS)
        continue;
      if (seen[nextdata[i].cr][nextdata[i].cc][dir] == 1)
        continue;
      if (content[nextdata[i].cr][nextdata[i].cc] == '#')
        continue;
      enqueue(&pq, nextdata[i]);
    }
  }

  free(pq.items);
  return -1;
}

int main(void) {
  FILE *fp = fopen(INPUT_FILE, "r");

  readFile(fp);

  printf("Answer = %d :)\n", performDijkstra());

  fclose(fp);
  return 0;
}
