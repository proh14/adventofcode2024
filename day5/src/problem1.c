#define _DEFAULT_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define INPUT_FILE "../res/input.txt"
#define BUFFSIZE 256
#define MAX_RULES 100
#define MAX_PAGES 25

static int order[MAX_RULES][MAX_RULES] = {0};

void storeRules(FILE *fp) {
  char buffer[BUFFSIZE];

  while (fgets(buffer, BUFFSIZE, fp) != NULL) {
    if (buffer[0] == '\n')
      break;
    int before;
    int after;
    sscanf(buffer, "%d|%d", &before, &after);
    order[before][after] = 1;
  }
}

int lineToPage(char *line, int *pages) {
  char *ptr = line;
  char *token;

  int i = 0;
  while ((token = strtok_r(ptr, ",", &ptr)) != NULL) {
    pages[i++] = atoi(token);
  }
  return i;
}

int validatePage(int *pages, int size) {
  for (int i = 0; i < size; i++) {
    for (int j = 0; j < i; j++) {
      if (!order[pages[j]][pages[i]])
        return 0;
    }
  }

  return pages[size / 2];
}

int main(void) {
  FILE *fp = fopen(INPUT_FILE, "r");

  storeRules(fp);

  char buffer[BUFFSIZE];

  int ans = 0;
  while (fgets(buffer, BUFFSIZE, fp) != NULL) {
    int pages[MAX_PAGES];
    int size = lineToPage(buffer, pages);
    ans += validatePage(pages, size);
  }

  printf("Answer = %d :)\n", ans);

  fclose(fp);
  return 0;
}
