#ifdef _IMPLEMENT_QUEUE_H_

#include <stdio.h>
#include <stdlib.h>

struct queueItem {
  int row, col, steps;
};

struct queue {
  struct queueItem *items;
  int front, rear;
  int size;
};

void queueInit(struct queue *q, int size) {
  q->items = (struct queueItem *)malloc(size * sizeof(struct queueItem));
  q->size = size;
  q->front = q->rear = 0;
}

void queueFree(struct queue *q) { free(q->items); }

void enqueue(struct queue *q, int row, int col, int steps) {
  if ((q->rear + 1) % q->size == q->front) {
    printf("Queue is full\n");
    return;
  }
  q->items[q->rear].row = row;
  q->items[q->rear].col = col;
  q->items[q->rear].steps = steps;

  q->rear = (q->rear + 1) % q->size;
}

struct queueItem *dequeue(struct queue *q) {
  if (q->front == q->rear) {
    return NULL;
  }
  struct queueItem *item = &q->items[q->front];
  q->front = (q->front + 1) % q->size;
  return item;
}

int isEmpty(struct queue *q) { return q->front == q->rear; }

#endif
