#define _DEFAULT_SOURCE
#include "uthash.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define _POSIB_IMPLEMENTATION_
#include "possibilities2.h"

#define INPUT_FILE "../res/input.txt"

#define UP 0
#define DIR_A 1
#define LEFT 2
#define DOWN 3
#define RIGHT 4
#define NUMPAD_A 10

#define MAX_SEQ 256
#define BUFSIZE 64

#define ROBOTS 25

char *numberToSequence(char from, char to) {
  int f = from == 'A' ? NUMPAD_A : from - '0';
  int t = to == 'A' ? NUMPAD_A : to - '0';
  return shortest_keypad[f][t];
}

int moveToNumber(char move) {
  switch (move) {
  case '^':
    return UP;
    break;
  case 'v':
    return DOWN;
    break;
  case '<':
    return LEFT;
    break;
  case '>':
    return RIGHT;
    break;
  case 'A':
    return DIR_A;
    break;
  default:
    return -1;
    break;
  }
}

char *moveToSequence(char from, char to) {
  int f = moveToNumber(from);
  int t = moveToNumber(to);
  return shortest_dirpad[f][t];
}

void comboToSequence(char *seq, char *combo) {
  int i = 0;
  int j = 0;
  char from = 'A';
  seq[j++] = 'A';
  while (combo[i] != '\n' && combo[i] != '\0') {
    char *seqs = numberToSequence(from, combo[i]);
    while (*seqs != '\0') {
      seq[j] = *seqs;
      seqs++;
      j++;
    }
    seq[j++] = 'A';
    from = combo[i];
    i++;
  }
  seq[j] = '\0';
}

struct cacheKey {
  char *seq;
  int depth;
  UT_hash_handle hh;
};

struct cache {
  struct cacheKey key;
  uint64_t value;
  UT_hash_handle hh;
};

struct cache *cache = NULL;

void addCache(char *key, uint64_t value, int depth) {
  struct cache *s;
  s = (struct cache *)malloc(sizeof(struct cache));
  memset(s, 0, sizeof(struct cache));
  s->key.seq = key;
  s->key.depth = depth;
  s->value = value;
  HASH_ADD(hh, cache, key, sizeof(struct cacheKey), s);
}

int found = 0;
uint64_t getCache(char *seq, int depth) {
  struct cache *s;
  struct cacheKey key;
  memset(&key, 0, sizeof(struct cacheKey));
  key.seq = seq;
  key.depth = depth;
  HASH_FIND(hh, cache, &key, sizeof(struct cacheKey), s);
  if (s != NULL) {
    found = 1;
    return s->value;
  }
  found = 0;
  return 0;
}

uint64_t sequenceToSequence(char from, char to, int currentDepth) {
  char *seq = moveToSequence(from, to);
  if (currentDepth == 1) {
    return strlen(seq) - 1;
  }

  uint64_t cached = getCache(seq, currentDepth);
  if (found == 1) {
    return cached;
  }

  uint64_t len = 0;

  for (int i = 0; i < (signed)strlen(seq) - 1; i++) {
    len += sequenceToSequence(seq[i], seq[i + 1], currentDepth - 1);
  }

  addCache(seq, len, currentDepth);

  return len;
}

uint64_t calculateComplexity(FILE *fp) {
  char buf[BUFSIZE];

  uint64_t complexity = 0;

  while (fgets(buf, BUFSIZE, fp) != NULL) {

    char seq1[MAX_SEQ];
    comboToSequence(seq1, buf);
    uint64_t len = 0;
    for (int i = 0; i < (signed)strlen(seq1) - 1; i++) {
      len += sequenceToSequence(seq1[i], seq1[i + 1], ROBOTS);
    }

    buf[strlen(buf) - 2] = '\0';
    len *= atoi(buf);
    complexity += len;
  }
  return complexity;
}

int main(void) {
  FILE *fp = fopen(INPUT_FILE, "r");

  printf("Answer = %lu :)\n", calculateComplexity(fp));

  fclose(fp);
  return 0;
}
