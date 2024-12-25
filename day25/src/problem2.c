#include <stdio.h>

#define INPUT_FILE "../res/input.txt"

#define MAX_LOCKS_AND_KEYS 2000

#define ROWS 7
#define COLS 5

int lock_heights[MAX_LOCKS_AND_KEYS][5];
int numLocks = 0;
int key_heights[MAX_LOCKS_AND_KEYS][5];
int numKeys = 0;

void processLock(int index, char section[ROWS][COLS]) {
  for (int i = 0; i < COLS; i++) {
    for (int j = 0; j < ROWS; j++) {
      if (section[j][i] == '#') {
        lock_heights[index][i]++;
      }
    }
    lock_heights[index][i]--;
  }
}
void processKey(int index, char section[ROWS][COLS]) {
  for (int i = 0; i < COLS; i++) {
    for (int j = 0; j < ROWS; j++) {
      if (section[j][i] == '#') {
        key_heights[index][i]++;
      }
    }
    key_heights[index][i]--;
  }
}

void processHeights(char section[ROWS][COLS]) {
  if (section[0][0] == '#') {
    processLock(numLocks++, section);
    return;
  }
  processKey(numKeys++, section);
}

void readFile(FILE *fp) {
  char section[ROWS][COLS];

  int ch = 0, row = 0, col = 0;

  while (1) {
    ch = fgetc(fp);
    if ((ch == '\n' || ch == EOF) && row == ROWS) {
      row = 0;
      col = 0;
      processHeights(section);
      if (ch == EOF) {
        break;
      }
      continue;
    }
    if (ch == '\n') {
      row++;
      col = 0;
      continue;
    }
    section[row][col] = ch;
    col++;
  }
}

int isValidKey(int lockIndex, int keyIndex) {
  for (int i = 0; i < COLS; i++) {
    if (ROWS - 1 - lock_heights[lockIndex][i] <= key_heights[keyIndex][i]) {
      return 0;
    }
  }
  return 1;
}

int validKeyLockPairs(void) {
  int count = 0;
  for (int i = 0; i < numLocks; i++) {
    for (int j = 0; j < numKeys; j++) {
      if (isValidKey(i, j)) {
        count++;
      }
    }
  }
  return count;
}

int main(void) {
  FILE *fp = fopen(INPUT_FILE, "r");

  readFile(fp);
  printf("Answer = %d :)\n", validKeyLockPairs());

  fclose(fp);
  return 0;
}
