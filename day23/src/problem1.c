#include "uthash.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define INPUT_FILE "../res/input.txt"

#define MAX_NEIGHBORS 50
#define MAX_NAME_LEN 3
#define BUFFSIZE 16

struct node {
  char name[MAX_NAME_LEN];
  char neighbors[MAX_NEIGHBORS][MAX_NAME_LEN];
  int num_neighbors;
  UT_hash_handle hh;
};

struct triplet {
  char names[3][MAX_NAME_LEN];
  UT_hash_handle hh;
};

struct node *nodes = NULL;
struct triplet *triplets = NULL;

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

void sortTriplet(char triplet[3][MAX_NAME_LEN]) {
  for (int i = 0; i < 2; i++) {
    for (int j = i + 1; j < 3; j++) {
      if (strcmp(triplet[i], triplet[j]) > 0) {
        char temp[MAX_NAME_LEN];
        strcpy(temp, triplet[i]);
        strcpy(triplet[i], triplet[j]);
        strcpy(triplet[j], temp);
      }
    }
  }
}

int countTripletsWithT(void) {
  int count = 0;
  struct node *n, *n1, *n2;

  for (n = nodes; n != NULL; n = n->hh.next) {
    for (int i = 0; i < n->num_neighbors; i++) {
      n1 = findNode(n->neighbors[i]);
      if (!n1) {
        continue;
      }
      for (int j = 0; j < n1->num_neighbors; j++) {
        n2 = findNode(n1->neighbors[j]);
        if (!n2) {
          continue;
        }
        if (strcmp(n->name, n2->name) == 0) {
          continue;
        }
        if (!areConnected(n->name, n2->name)) {
          continue;
        }

        char triplet[3][MAX_NAME_LEN];
        strcpy(triplet[0], n->name);
        strcpy(triplet[1], n1->name);
        strcpy(triplet[2], n2->name);
        sortTriplet(triplet);

        struct triplet *t;
        HASH_FIND(hh, triplets, triplet, sizeof(triplet), t);
        if (t)
          continue;
        t = (struct triplet *)malloc(sizeof(struct triplet));
        memcpy(t->names, triplet, sizeof(triplet));

        HASH_ADD(hh, triplets, names, sizeof(triplet), t);
        if (*n->name == 't' || *n1->name == 't' || *n2->name == 't') {
          count++;
        }
      }
    }
  }

  return count;
}

int main(void) {
  FILE *fp = fopen(INPUT_FILE, "r");
  if (!fp) {
    perror("Error opening file");
    return 1;
  }

  readFile(fp);
  printf("Answer = %d :)\n", countTripletsWithT());

  fclose(fp);
  return 0;
}
