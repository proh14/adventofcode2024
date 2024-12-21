#define _DEFAULT_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define INPUT_FILE "../res/input.txt"
#define MAX_PATTERNS 500
#define MAX_BUFFER 256

char *patterns[MAX_PATTERNS];
int patternCount = 0;

void readPatterns(FILE *fp) {
  char *buffer = malloc(4096 * sizeof(char));

  fgets(buffer, 4096, fp);
  char *token;
  char *saveptr = buffer;

  while ((token = strtok_r(saveptr, ", ", &saveptr))) {
    if (patternCount >= MAX_PATTERNS) {
      printf("Too many patterns\n");
      exit(1);
    }
    patterns[patternCount++] = strdup(token);
  }
  int len = strlen(patterns[patternCount - 1]);
  patterns[patternCount - 1][len - 1] = '\0';
  getc(fp);
  free(buffer);
}

int isValidDesign(char *design) {
  if (*design == '\n' || *design == '\0') {
    return 1;
  }

  for (int i = 0; i < patternCount; i++) {
    int len = strlen(patterns[i]);
    if (strncmp(design, patterns[i], len) == 0) {
      if (isValidDesign(design + len)) {
        return 1;
      }
    }
  }
  return 0;
}

int CountValidDesigns(FILE *fp) {
  int count = 0;
  char buffer[MAX_BUFFER];
  while (fgets(buffer, MAX_BUFFER, fp) != NULL) {
    count += isValidDesign(buffer);
  }
  return count;
}

void freePatterns(void) {
  for (int i = 0; i < patternCount; i++) {
    free(patterns[i]);
  }
}

int main(void) {
  FILE *fp = fopen(INPUT_FILE, "r");

  readPatterns(fp);

  printf("Answer = %d :)\n", CountValidDesigns(fp));

  freePatterns();
  fclose(fp);
  return 0;
}
