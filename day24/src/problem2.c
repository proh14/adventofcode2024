#include "uthash.h"
#include <ctype.h>
#include <stdio.h>

#define MAX_CABLE_NAME 4
#define MAX_STEPS 256
#define BUFFSIZE 256

#define INPUT_FILE "../res/input.txt"

struct step {
  char result[MAX_CABLE_NAME];
  char cable1[MAX_CABLE_NAME];
  char cable2[MAX_CABLE_NAME];
  char opration;
  UT_hash_handle hh;
};

struct step *steps = NULL;

void addStep(char *result, char *cable1, char *cable2, char opration) {
  struct step *s;
  s = (struct step *)malloc(sizeof(struct step));
  strcpy(s->result, result);
  strcpy(s->cable1, cable1);
  strcpy(s->cable2, cable2);
  s->opration = opration;
  HASH_ADD_STR(steps, result, s);
}

struct step *getStep(char *result) {
  struct step *s;
  HASH_FIND_STR(steps, result, s);
  return s;
}

void skipInitialWires(FILE *fp) {
  char buff[BUFFSIZE];
  while (fgets(buff, BUFFSIZE, fp) != NULL) {
    if (*buff == '\n') {
      break;
    }
  }
}

void parseStep(char *buff) {
  char result[MAX_CABLE_NAME];
  char cable1[MAX_CABLE_NAME];
  char cable2[MAX_CABLE_NAME];
  char opration;
  char oprationString[3];
  buff[strcspn(buff, "\n")] = 0;
  sscanf(buff, "%3s %s %3s -> %3s", cable1, oprationString, cable2, result);
  opration = oprationString[0];
  addStep(result, cable1, cable2, opration);
}

void parseSteps(FILE *fp) {
  char buff[BUFFSIZE];
  while (fgets(buff, BUFFSIZE, fp) != NULL) {
    parseStep(buff);
  }
}

void prettyPrintStep(char *cableName, int spaces) {
  if ((*cableName == 'x' || *cableName == 'y') && isdigit(*(cableName + 1))) {
    printf("%*s%s\n", spaces, "", cableName);
    return;
  }
  struct step *s = getStep(cableName);
  if (!s) {
    return;
  }

  switch (s->opration) {
  case 'A':
    printf("%*s[%s] AND ->\n", spaces, "", cableName);
    break;
  case 'O':
    printf("%*s[%s] OR ->\n", spaces, "", cableName);
    break;
  case 'X':
    printf("%*s[%s] XOR ->\n", spaces, "", cableName);
    break;
  }

  prettyPrintStep(s->cable1, spaces + 2);
  prettyPrintStep(s->cable2, spaces + 2);
}

int isOutputToOrGate(char *cableName) {
  for (struct step *s = steps; s != NULL; s = (struct step *)(s->hh.next)) {
    if (s->opration == 'O' && (strcmp(s->cable1, cableName) == 0 ||
                               strcmp(s->cable2, cableName) == 0)) {
      return 1;
    }
  }
  return 0;
}

int isOutputToXorGate(char *cableName) {
  for (struct step *s = steps; s != NULL; s = (struct step *)(s->hh.next)) {
    if (s->opration == 'X' && (strcmp(s->cable1, cableName) == 0 ||
                               strcmp(s->cable2, cableName) == 0)) {
      return 1;
    }
  }
  return 0;
}

int validateAdder(struct step *s) {
  if (s->opration == 'X') {
    int is = isOutputToXorGate(s->result);
    if ((*s->cable1 == 'x' || *s->cable2 == 'x') && is) {
      if (*s->cable1 == 'y' || *s->cable2 == 'y') {
        return 1;
      }
    }
    if (*s->result == 'z') {
      return 1;
    }
  }

  if (s->opration == 'O') {
    if (*s->result != 'z') {
      return 1;
    }
    if (strcmp(s->result, "z45") == 0) {
      return 1;
    }
  }

  if (s->opration == 'A') {
    int is = isOutputToOrGate(s->result);
    if (*s->result != 'z' && is) {
      return 1;
    }
    if (strcmp(s->cable1, "x00") == 0 || strcmp(s->cable2, "x00") == 0) {
      return 1;
    }
  }

  return 0;
}

char swaps[8][MAX_CABLE_NAME];
int swapIndex = 0;

int cmp(const void *lhs, const void *rhs) { return strcmp(lhs, rhs); }

void validateAdders(void) {
  for (struct step *s = steps; s != NULL; s = (struct step *)(s->hh.next)) {
    if (!validateAdder(s)) {
      strcpy(swaps[swapIndex++], s->result);
    }
  }
}

int main(void) {
  FILE *fp = fopen(INPUT_FILE, "r");

  skipInitialWires(fp);
  parseSteps(fp);

  validateAdders();

  qsort(swaps, swapIndex, MAX_CABLE_NAME, cmp);

  printf("Answer = ");
  for (int i = 0; i < swapIndex; i++) {
    printf("%s", swaps[i]);
    if (i != swapIndex - 1) {
      printf(",");
    }
  }
  printf(" :)\n");

  fclose(fp);
  return 0;
}
