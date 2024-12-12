#define _DEFAULT_SOURCE
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <uthash.h>

#define INPUT_FILE "../res/input.txt"
#define MAX_CONTENT 20
#define BUFFSIZE 256
#define BLINKS 75

int content[MAX_CONTENT];
int content_size = 0;

void readFile(FILE *fp) {
  char buffer[BUFFSIZE];
  if (fgets(buffer, BUFFSIZE, fp) == NULL)
    exit(1);
  char *ptr = buffer;
  char *token;
  while (content_size < MAX_CONTENT &&
         (token = strtok_r(ptr, " ", &ptr)) != NULL) {
    content[content_size++] = atoi(token);
  }
}

struct stone_key {
  uint64_t number;
  int blinks;
};

struct stone {
  struct stone_key key;
  uint64_t result;
  UT_hash_handle hh;
};

struct stone *cache = NULL;

void addCache(uint64_t number, int blink, uint64_t result) {
  struct stone *s = malloc(sizeof(struct stone));
  memset(s, 0, sizeof(struct stone));
  s->result = result;
  s->key.number = number;
  s->key.blinks = blink;
  HASH_ADD(hh, cache, key, sizeof(struct stone_key), s);
}

struct stone *doesExistInCache(uint64_t number, int blink) {
  struct stone_key key;
  memset(&key, 0, sizeof(struct stone_key));
  key.number = number;
  key.blinks = blink;
  struct stone *s;
  HASH_FIND(hh, cache, &key, sizeof(struct stone_key), s);

  return s;
}

uint64_t calculateItem(uint64_t number, int blinks) {
  if (blinks == 0)
    return 1;

  struct stone *s = doesExistInCache(number, blinks);

  if (s)
    return s->result;

  uint64_t ans = 0;
  if (number == 0) {
    ans = calculateItem(1, blinks - 1);
    addCache(number, blinks, ans);
    return ans;
  }

  char num[40];
  int len = snprintf(num, 40, "%lu", number);
  if (len % 2 == 0) {
    int tmp = num[len / 2];
    num[len / 2] = '\0';
    ans += calculateItem(strtoull(num, NULL, 10), blinks - 1);
    num[len / 2] = tmp;
    char *num2 = &num[len / 2];
    ans += calculateItem(strtoull(num2, NULL, 10), blinks - 1);
    addCache(number, blinks, ans);
    return ans;
  }

  ans = calculateItem(number * 2024, blinks - 1);
  addCache(number, blinks, ans);

  return ans;
}

int main(void) {
  FILE *fp = fopen(INPUT_FILE, "r");

  readFile(fp);

  uint64_t ans = 0;
  for (int i = 0; i < content_size; i++) {
    ans += calculateItem(content[i], BLINKS);
  }

  printf("Answer = %lu :)\n", ans);

  fclose(fp);
}
