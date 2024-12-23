#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#define INPUT_FILE "../res/input.txt"
#define BUFSIZE 256
#define MAX_SECRET 2000

uint64_t solve(uint64_t secret) {
  secret = (secret ^ (secret * 64)) % 16777216;
  secret = (secret ^ (secret / 32)) % 16777216;
  secret = (secret ^ (secret * 2048)) % 16777216;
  return secret;
}

int main(void) {
  FILE *fp = fopen(INPUT_FILE, "r");
  char buf[BUFSIZE];

  uint64_t ans = 0;
  while (fgets(buf, BUFSIZE, fp) != NULL) {
    uint64_t secret = strtoull(buf, NULL, 10);
    for (int i = 0; i < MAX_SECRET; i++) {
      secret = solve(secret);
    }
    ans += secret;
  }
  printf("%lu\n", ans);

  fclose(fp);
  return 0;
}
