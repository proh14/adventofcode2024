#include "uthash.h"
#include <stdint.h>
#include <stdio.h>

#define INPUT_FILE "../res/input.txt"

#define MAX_NAME_LEN 4
#define MAX_OUTPUT 50
#define MAX_CONNECTIONS 256
#define BUFSIZE 256

struct cable {
  char name[MAX_NAME_LEN];
  int value;
  UT_hash_handle hh;
};

struct cable *cables = NULL;

void addCable(char *name, int value) {
  struct cable *cable;
  HASH_FIND_STR(cables, name, cable);
  if (cable == NULL) {
    cable = (struct cable *)malloc(sizeof(struct cable));
    strncpy(cable->name, name, MAX_NAME_LEN);
    HASH_ADD_STR(cables, name, cable);
  }
  cable->value = value;
}

int getCableValue(char *name) {
  struct cable *cable;
  HASH_FIND_STR(cables, name, cable);
  if (cable == NULL) {
    return -1;
  }
  return cable->value;
}

int output[MAX_OUTPUT];
int numOutput = 0;

void initializeCables(FILE *fp) {
  char buffer[BUFSIZE];

  while (fgets(buffer, BUFSIZE, fp) != NULL && *buffer != '\n') {
    char name[MAX_NAME_LEN];
    int value;
    sscanf(buffer, "%3s: %d", name, &value);
    addCable(name, value);
  }
}

void calculateOutput(FILE *fp) {
  char buffer[BUFSIZE];

  int visited[MAX_CONNECTIONS] = {0};
  int loc = ftell(fp);
  int cl = 0;
  int goback = 0;
  while (1) {
    fgets(buffer, BUFSIZE, fp);

    if (feof(fp)) {
      if (goback) {
        fseek(fp, loc, SEEK_SET);
        cl = 0;
        goback = 0;
        continue;
      }
      break;
    }

    char opration[4];
    char cable1[MAX_NAME_LEN], cable2[MAX_NAME_LEN];
    char result[MAX_NAME_LEN];
    buffer[strcspn(buffer, "\n")] = 0;
    sscanf(buffer, "%3s %s %3s -> %3s", cable1, opration, cable2, result);

    int val1 = getCableValue(cable1);
    int val2 = getCableValue(cable2);

    if (visited[cl++] == 1) {
      continue;
    }

    if (val1 == -1 || val2 == -1) {
      goback = 1;
      continue;
    }

    visited[cl - 1] = 1;

    int res = 0;

    switch (*opration) {
    case 'A':
      res = val1 & val2;
      break;
    case 'O':
      res = val1 | val2;
      break;
    case 'X':
      res = val1 ^ val2;
      break;
    default:
      return;
      break;
    }

    addCable(result, res);

    if (*result == 'z') {
      int index = atoi(result + 1);
      if (index > MAX_OUTPUT) {
        printf("Output index out of bounds\n");
        exit(1);
      }
      output[index] = res;
      numOutput = index > numOutput ? index : numOutput;
    }
  }
}

uint64_t convertOutputToDecimal(void) {
  uint64_t res = 0;
  for (int i = numOutput; i >= 0; i--) {
    res = res * 2 + output[i];
  }
  return res;
}

int main(void) {
  FILE *fp = fopen(INPUT_FILE, "r");

  initializeCables(fp);
  calculateOutput(fp);

  printf("Answer = %lu :)\n", convertOutputToDecimal());

  fclose(fp);
  return 0;
}
