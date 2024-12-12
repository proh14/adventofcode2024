#define _DEFAULT_SOURCE
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <uthash.h>

#define INPUT_FILE "../res/input.txt"
#define MAX_CONTENT 20
#define BUFFSIZE 256
#define BLINKS 25

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
  int number;
  int blinks;
};

struct stone {
  struct stone_key key;
  uint64_t result;
  UT_hash_handle hh;
};

struct stone *cache = NULL;

void addCache(int number, int blink, int result) {
  struct stone *s = malloc(sizeof(struct stone));
  memset(s, 0, sizeof(struct stone));
  s->result = result;
  s->key.number = number;
  s->key.blinks = blink;
  HASH_ADD(hh, cache, key, sizeof(struct stone_key), s);
}

int doesExistInCache(int number, int blink) {
  struct stone_key key;
  memset(&key, 0, sizeof(struct stone_key));
  key.number = number;
  key.blinks = blink;
  struct stone *s;
  HASH_FIND(hh, cache, &key, sizeof(struct stone_key), s);

  if (!s)
    return -1;

  return s->result;
}

int calculateItem(uint64_t number, int blinks) {
  if (blinks == 0)
    return 1;

  int find = doesExistInCache(number, blinks);

  if (find != -1)
    return find;

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
    ans += calculateItem(atoi(num), blinks - 1);
    num[len / 2] = tmp;
    char *num2 = &num[len / 2];
    ans += calculateItem(atoi(num2), blinks - 1);
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

  int ans = 0;
  for (int i = 0; i < content_size; i++) {
    ans += calculateItem(content[i], BLINKS);
  }

  printf("Answer = %d :)\n", ans);

  fclose(fp);
}
