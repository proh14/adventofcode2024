#ifdef IMPLEMENT_PRIORITYQUEUE_H

// Implementation of a Binary Heap Priority Queue
//  https://en.wikipedia.org/wiki/Binary_heap

#include <stdio.h>
#define MAX_DATA 100000

struct data {
  int cr;
  int cc;
  int dr;
  int dc;
  int cost;
};

struct priorityQueue {
  struct data items[MAX_DATA];
  int size;
};

void swap(struct data *a, struct data *b) {
  struct data temp = *a;
  *a = *b;
  *b = temp;
}

void heapify(struct priorityQueue *pq, int index) {
  int smallest = index;

  int left = 2 * index + 1;

  int right = 2 * index + 2;

  if (left < pq->size && pq->items[left].cost < pq->items[smallest].cost) {
    smallest = left;
  }

  if (right < pq->size && pq->items[right].cost < pq->items[smallest].cost) {
    smallest = right;
  }

  if (smallest != index) {
    swap(&pq->items[index], &pq->items[smallest]);
    heapify(pq, smallest);
  }
}

void swapIfNeeded(struct priorityQueue *pq, int index) {
  if (index && pq->items[(index - 1) / 2].cost > pq->items[index].cost) {
    swap(&pq->items[(index - 1) / 2], &pq->items[index]);
    swapIfNeeded(pq, (index - 1) / 2);
  }
}

void enqueue(struct priorityQueue *pq, struct data item) {
  if (pq->size >= MAX_DATA) {
    printf("No more space in queue\n");
    return;
  }
  pq->items[pq->size] = item;
  swapIfNeeded(pq, pq->size);
  pq->size++;
}

struct data dequeue(struct priorityQueue *pq) {
  if (pq->size <= 0) {
    printf("Queue is empty\n");
    return (struct data){0, 0, 0, 0, -1};
  }
  struct data item = pq->items[0];
  pq->items[0] = pq->items[--pq->size];
  heapify(pq, 0);
  return item;
}

#endif
