#define _DEFAULT_SOURCE

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define INPUT_FILE "../res/input.txt"
#define MAX_NUMS 25
#define BUFFSIZE 256

int readLine(char *str, int *nums, unsigned long *cala) {
  unsigned long cal = 0;
  sscanf(str, "%lu:", &cal);

  while (*str && *str != ' ')
    str++;

  char *token;
  char *saveptr = str;

  int i = 0;
  while ((token = strtok_r(saveptr, " ", &saveptr)) != NULL) {
    nums[i++] = atoi(token);
  }
  *cala = cal;

  return i;
}
int validateCal(int *nums, int idx, int numsize, unsigned long cal,
                unsigned long res) {
  if (idx >= numsize)
    return res == cal;

  return validateCal(nums, idx + 1, numsize, cal, nums[idx] + res) ||
         validateCal(nums, idx + 1, numsize, cal, nums[idx] * res);
}

int main(void) {

  FILE *fp = fopen(INPUT_FILE, "r");
  uint64_t ans = 0;
  char buffer[BUFFSIZE];

  while (fgets(buffer, BUFFSIZE, fp)) {
    int nums[MAX_NUMS];
    unsigned long cal = 0;
    int numlen = readLine(buffer, nums, &cal);
    if (validateCal(nums, 0, numlen, cal, 0))
      ans += cal;
  }

  printf("Answer = %lu :)\n", ans);

  fclose(fp);
  return 0;
}
