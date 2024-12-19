// Todo: Optimize memory usage
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define IMPLEMENT_PRIORITYQUEUE_H
#include <priorityqueue.h>

#define INPUT_FILE "../res/input.txt"

#define COLS 141
#define ROWS 141

#define INF INT_MAX

char **content;

int cr = 0, cc = 0;
int er = 0, ec = 0, ed = 0;

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
    if (ch == 'E') {
      er = row;
      ec = col;
    }
    content[row][col++] = ch;
  }
}

int dr[] = {0, 1, 0, -1};
int dc[] = {1, 0, -1, 0};

#define MAX_BACK 2100

struct behind {
  struct data *back;
  int back_index;
};

struct behind ***behinds;
int ***costs;

void setCosts(void) {
  for (int i = 0; i < ROWS; i++) {
    for (int j = 0; j < COLS; j++) {
      for (int k = 0; k < 4; k++) {
        costs[i][j][k] = INF;
      }
    }
  }
}

int directionToNumber(int dy, int dx) {
  for (int i = 0; i < 4; i++) {
    if (dr[i] == dy && dc[i] == dx)
      return i;
  }
  return -1;
}

void setNextdata(struct data nextdata[3], struct data item, int dir) {
  int cw = (dir + 1) % 4;
  int ccw = (dir + 3) % 4;
  nextdata[0] = (struct data){item.cr + item.dr, item.cc + item.dc, item.dr,
                              item.dc, item.cost + 1};
  nextdata[1] =
      (struct data){item.cr, item.cc, dr[cw], dc[cw], item.cost + 1000};

  nextdata[2] = (struct data){
      item.cr, item.cc, dr[ccw], dc[ccw], item.cost + 1000,
  };
}

int performDijkstra(void) {
  struct priorityQueue pq = {0};
  pq.items = malloc(sizeof(struct data) * MAX_DATA * 2);
  enqueue(&pq, (struct data){cr, cc, 0, 1, 0});

  setCosts();
  int best_cost = INF;

  while (pq.size > 0) {
    struct data item = dequeue(&pq);
    int dir = directionToNumber(item.dr, item.dc);

    if (item.cost > costs[item.cr][item.cc][dir])
      continue;

    if (content[item.cr][item.cc] == 'E') {
      if (item.cost > best_cost) {
        free(pq.items);
        return item.cost;
      }
      best_cost = item.cost;
      ed = dir;
    }

    struct data nextdata[3];
    setNextdata(nextdata, item, dir);

    for (int i = 0; i < 3; i++) {
      dir = directionToNumber(nextdata[i].dr, nextdata[i].dc);
      if (nextdata[i].cr < 0 || nextdata[i].cc < 0 || nextdata[i].cr >= ROWS ||
          nextdata[i].cc >= COLS)
        continue;
      if (content[nextdata[i].cr][nextdata[i].cc] == '#')
        continue;

      int lowest = costs[nextdata[i].cr][nextdata[i].cc][dir];

      if (nextdata[i].cost > lowest)
        continue;
      if (nextdata[i].cost < lowest) {
        behinds[nextdata[i].cr][nextdata[i].cc][dir].back_index = 0;
        costs[nextdata[i].cr][nextdata[i].cc][dir] = nextdata[i].cost;
      }

      int back_index = behinds[nextdata[i].cr][nextdata[i].cc][dir].back_index;

      if (back_index >= MAX_BACK)
        printf(":sob:");

      behinds[nextdata[i].cr][nextdata[i].cc][dir].back[back_index] = item;
      behinds[nextdata[i].cr][nextdata[i].cc][dir].back_index++;
      enqueue(&pq, nextdata[i]);
    }
  }

  free(pq.items);
  return -1;
}

#define MAX_QUEUE_SIZE 20000

typedef struct {
  int cr, cc, cd; // Current row and column
} queueItem;

typedef struct {
  queueItem *items;
  int front, rear;
} queue;

void initqueue(queue *q) {
  q->items = malloc(MAX_QUEUE_SIZE * sizeof(queueItem));
  q->front = 0;
  q->rear = 0;
}

int isqueueEmpty(queue *q) { return q->front == q->rear; }

void nenqueue(queue *q, int cr, int cc, int cd) {
  q->items[q->rear].cr = cr;
  q->items[q->rear].cc = cc;
  q->items[q->rear].cd = cd;
  q->rear = (q->rear + 1) % MAX_QUEUE_SIZE;
}

queueItem ndequeue(queue *q) {
  queueItem item = q->items[q->front];
  q->front = (q->front + 1) % MAX_QUEUE_SIZE;
  return item;
}

int ***visited;

void bfsFloodFill(void) {
  queue q;
  initqueue(&q);

  nenqueue(&q, er, ec, ed);

  while (!isqueueEmpty(&q)) {
    queueItem current = ndequeue(&q);
    int cr = current.cr;
    int cc = current.cc;
    int cd = current.cd;

    if (cr < 0 || cc < 0 || cr >= ROWS || cc >= COLS)
      continue;

    if (visited[cr][cc][cd])
      continue;

    visited[cr][cc][cd] = 1;
    content[cr][cc] = 'O';

    struct behind *prevPaths = &behinds[cr][cc][cd];
    for (int i = 0; i < prevPaths->back_index; i++) {
      struct data previous = prevPaths->back[i];

      int dir = directionToNumber(previous.dr, previous.dc);
      if (!visited[previous.cr][previous.cc][dir]) {
        nenqueue(&q, previous.cr, previous.cc, dir);
      }
    }
  }

  free(q.items);
}

void allocateMemory(void) {
  content = malloc(ROWS * sizeof(char *));
  for (int i = 0; i < ROWS; i++) {
    content[i] = malloc(COLS * sizeof(char));
  }

  behinds = malloc(ROWS * sizeof(struct behind **));
  for (int i = 0; i < ROWS; i++) {
    behinds[i] = malloc(COLS * sizeof(struct behind *));
    for (int j = 0; j < COLS; j++) {
      behinds[i][j] = malloc(4 * sizeof(struct behind));
      for (int k = 0; k < 4; k++) {
        behinds[i][j][k].back = malloc(MAX_BACK * sizeof(struct data));
        behinds[i][j][k].back_index = 0;
      }
    }
  }

  costs = malloc(ROWS * sizeof(int **));
  for (int i = 0; i < ROWS; i++) {
    costs[i] = malloc(COLS * sizeof(int *));
    for (int j = 0; j < COLS; j++) {
      costs[i][j] = malloc(4 * sizeof(int));
    }
  }

  visited = malloc(ROWS * sizeof(int **));
  for (int i = 0; i < ROWS; i++) {
    visited[i] = malloc(COLS * sizeof(int *));
    for (int j = 0; j < COLS; j++) {
      visited[i][j] = malloc(4 * sizeof(int));
      memset(visited[i][j], 0, 4 * sizeof(int));
    }
  }
}

void freeMemory(void) {
  for (int i = 0; i < ROWS; i++) {
    free(content[i]);
  }
  free(content);

  for (int i = 0; i < ROWS; i++) {
    for (int j = 0; j < COLS; j++) {
      for (int k = 0; k < 4; k++) {
        free(behinds[i][j][k].back);
      }
      free(behinds[i][j]);
    }
    free(behinds[i]);
  }
  free(behinds);

  for (int i = 0; i < ROWS; i++) {
    for (int j = 0; j < COLS; j++) {
      free(costs[i][j]);
    }
    free(costs[i]);
  }
  free(costs);

  for (int i = 0; i < ROWS; i++) {
    for (int j = 0; j < COLS; j++) {
      free(visited[i][j]);
    }
  }
  free(visited);
}

int main(void) {
  FILE *fp = fopen(INPUT_FILE, "r");

  allocateMemory();

  readFile(fp);

  performDijkstra();

  int ans = 0;

  bfsFloodFill();

  for (int i = 0; i < ROWS; i++) {
    for (int j = 0; j < COLS; j++) {
      if (content[i][j] == 'O')
        ans++;
    }
  }

  printf("Answer = %d :)\n", ans);

  fclose(fp);
  freeMemory();
  return 0;
}
