#include <stdio.h>
#include <stdlib.h>

#define INPUT_FILE "../res/input.txt"

int *content;
int content_size = 0;
int content_cap = 40000;
int freespaces = 0;

void allocateSpace() {
  content_cap *= 2;
  content = realloc(content, sizeof(int) * content_cap);
}

void putContent(int id, int amount) {
  int old_conentsize = content_size;
  for (; content_size < amount + old_conentsize; content_size++) {
    if (content_size >= content_cap)
      allocateSpace();
    content[content_size] = id;
  }
}

void readFile(FILE *fp) {
  int ch = 0;
  int id = 0;
  int i = 0;
  while ((ch = getc(fp)) != EOF) {
    if (i % 2 == 0) {
      putContent(id, ch - '0');
      id++;
    } else {
      putContent(-1, ch - '0');
      freespaces++;
    }
    i++;
  }
}

int lastDisk(int last) {
  while (content[last] == -1)
    last--;
  return last;
}

void moveSpaces(void) {
  int last = lastDisk(content_size - 1);
  int moved = 0;
  for (int i = 0; i < content_size; i++) {
    if (last <= i)
      break;
    if (content[i] == -1) {
      content[i] = content[last];
      content[last] = -1;
      last = lastDisk(last - 1);
      moved++;
    }
  }
}

int calculateChecksum(void) {
  int i = 0;
  int ans = 0;
  while (content[i] != -1) {
    ans += content[i] * i;
    i++;
  }
  return ans;
}

int main(void) {
  FILE *fp = fopen(INPUT_FILE, "r");
  content = malloc(sizeof(int) * content_cap);
  readFile(fp);
  moveSpaces();

  printf("Answer = %d :)\n", calculateChecksum());

  free(content);
  fclose(fp);
  return 0;
}
