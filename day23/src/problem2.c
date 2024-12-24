#include "uthash.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define INPUT_FILE "../res/input.txt"

#define MAX_NEIGHBORS 50
#define MAX_NAME_LEN 3
#define MAX_CLIQUE 300
#define BUFFSIZE 16

struct node {
  char name[MAX_NAME_LEN];
  char neighbors[MAX_NEIGHBORS][MAX_NAME_LEN];
  int num_neighbors;
  UT_hash_handle hh;
};

struct node *nodes = NULL;

void addNode(char *name, char *neighbor) {
  struct node *n;
  HASH_FIND_STR(nodes, name, n);
  if (n == NULL) {
    n = (struct node *)malloc(sizeof(struct node));
    strcpy(n->name, name);
    n->num_neighbors = 0;
    HASH_ADD_STR(nodes, name, n);
  }
  if (n->num_neighbors >= MAX_NEIGHBORS) {
    printf("Node %s has too many neighbors\n", name);
    exit(1);
  }
  strcpy(n->neighbors[n->num_neighbors++], neighbor);
}

struct node *findNode(char *name) {
  struct node *n;
  HASH_FIND_STR(nodes, name, n);
  return n;
}

int areConnected(char *name1, char *name2) {
  struct node *n1 = findNode(name1);
  struct node *n2 = findNode(name2);
  if (n1 == NULL || n2 == NULL) {
    return 0;
  }
  for (int i = 0; i < n1->num_neighbors; i++) {
    if (strcmp(n1->neighbors[i], name2) == 0) {
      return 1;
    }
  }
  return 0;
}

void readFile(FILE *fp) {
  char buff[BUFFSIZE];
  while (fgets(buff, BUFFSIZE, fp) != NULL) {
    buff[strcspn(buff, "\n")] = 0;
    char *name1 = strtok(buff, "-");
    char *name2 = strtok(NULL, "-");
    addNode(name1, name2);
    addNode(name2, name1);
  }
}

// Refrence: https://en.wikipedia.org/wiki/Bron%E2%80%93Kerbosch_algorithm
void BronKerbosch(struct node *R[MAX_CLIQUE], struct node *P[MAX_CLIQUE],
                  struct node *X[MAX_CLIQUE], int rSize, int pSize, int xSize,
                  struct node **maxClique, int *maxSize) {
  if (pSize == 0 && xSize == 0) {
    if (rSize > *maxSize) {
      *maxSize = rSize;
      memcpy(maxClique, R, rSize * sizeof(struct node *));
    }
    return;
  }

  for (int i = 0; i < pSize; i++) {
    struct node *v = P[i];
    struct node *newR[rSize + 1];
    struct node *newP[pSize];
    struct node *newX[xSize];
    int newRSize = rSize + 1;
    int newPSize = 0;
    int newXSize = 0;

    memcpy(newR, R, rSize * sizeof(struct node *));
    newR[rSize] = v;

    for (int j = 0; j < pSize; j++) {
      if (areConnected(v->name, P[j]->name)) {
        newP[newPSize++] = P[j];
      }
    }

    for (int j = 0; j < xSize; j++) {
      if (areConnected(v->name, X[j]->name)) {
        newX[newXSize++] = X[j];
      }
    }

    BronKerbosch(newR, newP, newX, newRSize, newPSize, newXSize, maxClique,
                 maxSize);

    X[xSize++] = v;
    for (int k = i; k < pSize - 1; k++) {
      P[k] = P[k + 1];
    }
    pSize--;
    i--;
  }
}

int main(void) {
  FILE *fp = fopen(INPUT_FILE, "r");
  if (!fp) {
    perror("Error opening file");
    return 1;
  }

  readFile(fp);

  struct node *R[MAX_CLIQUE];
  struct node *P[MAX_CLIQUE];
  struct node *X[MAX_CLIQUE];
  struct node *maxClique[MAX_CLIQUE];
  int maxSize = 0;
  int rSize = 0;
  int pSize = 0;
  int xSize = 0;

  struct node *n;
  for (n = nodes; n != NULL; n = n->hh.next) {
    P[pSize++] = n;
  }

  BronKerbosch(R, P, X, rSize, pSize, xSize, maxClique, &maxSize);

  printf("Answer = ");
  for (int i = 0; i < maxSize; i++) {
    printf("%s,", maxClique[i]->name);
  }
  printf(" :) \n");

  fclose(fp);
  return 0;
}
