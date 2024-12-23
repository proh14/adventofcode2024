#include "uthash.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#define INPUT_FILE "../res/input.txt"
#define BUFSIZE 256
#define MAX_SECRET 2001

uint64_t solve(uint64_t secret) {
  secret = (secret ^ (secret * 64)) % 16777216;
  secret = (secret ^ (secret / 32)) % 16777216;
  secret = (secret ^ (secret * 2048)) % 16777216;
  return secret;
}

struct changes {
  int pricechanges[4];
  int bananas;
  int current_line;
  UT_hash_handle hh;
};

struct changes *changes = NULL;

void addChange(int pricechanges[4], int bananas, int current_line) {
  struct changes *s;
  HASH_FIND(hh, changes, pricechanges, sizeof(int) * 4, s);
  if (s == NULL) {
    s = malloc(sizeof(struct changes));
    memset(s, 0, sizeof(struct changes));
    memcpy(s->pricechanges, pricechanges, sizeof(int) * 4);
    s->bananas = bananas;
    s->current_line = current_line;
    HASH_ADD(hh, changes, pricechanges, sizeof(int) * 4, s);
    return;
  }
  if (s->current_line == current_line) {
    if (s->bananas < bananas) {
      s->bananas = bananas;
    }
    return;
  }
  s->current_line = current_line;
  s->bananas += bananas;
}

int doesExit(int pricechanges[4]) {
  struct changes *s;
  HASH_FIND(hh, changes, pricechanges, sizeof(int) * 4, s);
  return s != NULL;
}

int maxBananas(void) {
  struct changes *s;
  int max = -1;
  int pricechanges[4] = {0, 0, 0, 0};
  for (s = changes; s != NULL; s = (struct changes *)(s->hh.next)) {
    if (s->bananas > max) {
      max = s->bananas;
      memcpy(pricechanges, s->pricechanges, sizeof(int) * 4);
    }
  }
  return max;
}

int main(void) {
  FILE *fp = fopen(INPUT_FILE, "r");
  char buf[BUFSIZE];
  int line = 0;
  while (fgets(buf, BUFSIZE, fp) != NULL) {
    buf[strcspn(buf, "\n")] = 0;
    uint64_t secret = strtoull(buf, NULL, 10);
    int buyers[MAX_SECRET] = {0};
    for (int i = 0; i < MAX_SECRET; i++) {
      buyers[i] = secret % 10;
      secret = solve(secret);
    }
    for (int i = 0; i < MAX_SECRET - 4; i++) {
      int pricechanges[4] = {
          buyers[i + 1] - buyers[i], buyers[i + 1] - buyers[i + 2],
          buyers[i + 3] - buyers[i + 2], buyers[i + 3] - buyers[i + 4]};
      addChange(pricechanges, buyers[i + 4], line);
    }
    line++;
  }

  printf("Answer = %d :)\n", maxBananas());

  fclose(fp);
  return 0;
}
