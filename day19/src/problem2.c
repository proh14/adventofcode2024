#define _DEFAULT_SOURCE
#include "uthash.h"
#include <stdint.h>
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

struct cache {
  char *key;
  uint64_t value;
  UT_hash_handle hh;
};

struct cache *cache = NULL;

void addCache(char *key, uint64_t value) {
  struct cache *c;
  HASH_FIND_STR(cache, key, c);
  if (c == NULL) {
    c = malloc(sizeof(struct cache));
    c->key = strdup(key);
    c->value = value;
    HASH_ADD_KEYPTR(hh, cache, c->key, strlen(c->key), c);
  }
}

uint64_t getCache(char *key) {
  struct cache *c;
  HASH_FIND_STR(cache, key, c);
  if (c == NULL) {
    return 0;
  }
  return c->value;
}

uint64_t isValidDesign(char *design) {
  if (*design == '\n' || *design == '\0') {
    return 1;
  }

  uint64_t cached = getCache(design);

  if (cached != 0) {
    return cached;
  }

  uint64_t sum = 0;

  for (int i = 0; i < patternCount; i++) {
    int len = strlen(patterns[i]);
    if (strncmp(design, patterns[i], len) == 0) {
      sum += isValidDesign(design + len);
    }
  }
  addCache(design, sum);
  return sum;
}

uint64_t CountValidDesigns(FILE *fp) {
  uint64_t count = 0;
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

  printf("Answer = %lu :)\n", CountValidDesigns(fp));

  freePatterns();
  fclose(fp);
  return 0;
}
