#include <stdio.h>

#define INPUT_FILE "../res/input.txt"
#define LINES 1000
#define MAX_NUMS 10
#define BUFFSIZE 256

int readLine(FILE *fp, int *nums) {
  int i = 0;
  char buffer[BUFFSIZE];
  fgets(buffer, BUFFSIZE, fp);
  char *ptr = buffer;
  while (i < MAX_NUMS && sscanf(ptr, "%d", &nums[i]) == 1) {
    while (*ptr && *ptr >= '0' && *ptr <= '9') {
      ptr++;
    }
    if (*ptr == ' ')
      ptr++;
    i++;
  }
  return i;
}

int isValid(int *nums, int size) {

  int inc = 0;
  if (nums[0] == nums[1])
    return 0;
  if (nums[0] < nums[1])
    inc = 1;
  if (nums[0] > nums[1])
    inc = -1;

  for (int i = 0; i < size; i++) {
    if (inc == 1 && i + 1 < size &&
        (nums[i] >= nums[i + 1] || nums[i + 1] - nums[i] > 3))
      return 0;

    if (inc == -1 && i + 1 < size &&
        (nums[i] <= nums[i + 1] || nums[i] - nums[i + 1] > 3))
      return 0;
  }
  return 1;
}

int main(void) {

  FILE *fp = fopen(INPUT_FILE, "r");

  int ans = 0;
  for (int i = 0; i < LINES; i++) {
    int nums[MAX_NUMS] = {0};
    int size = readLine(fp, nums);
    ans += isValid(nums, size);
  }

  printf("Answer = %d :)\n", ans);

  fclose(fp);
  return 0;
}