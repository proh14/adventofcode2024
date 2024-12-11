#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#define INPUT_FILE "../res/input.txt"

int *content;
int content_size = 0;
int content_cap = 40000;
int freespaces = 0;

void allocateSpace(void) {
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

int lenFilesR(int index) {
  int len = 0;
  int i = index;
  while (i < content_size && content[i] == content[index]) {
    len++;
    i++;
  }
  return len;
}

int findLeftmostFreeSpace(int len) {
  int start = -1, current_len = 0;
  for (int i = 0; i < content_size; i++) {
    if (content[i] == -1) {
      if (start == -1)
        start = i;
      current_len++;
      if (current_len >= len)
        return start;
    } else {
      start = -1;
      current_len = 0;
    }
  }
  return -1;
}

void compactFiles(void) {
  for (int id = content_size - 1; id >= 0; id--) {
    if (content[id] != -1 && (id == 0 || content[id] != content[id - 1])) {
      int file_len = lenFilesR(id);
      int free_space_start = findLeftmostFreeSpace(file_len);

      if (free_space_start != -1 && free_space_start < id) {
        for (int i = 0; i < file_len; i++) {
          content[free_space_start + i] = content[id + i];
          content[id + i] = -1;
        }
      }
    }
  }
}

uint64_t calculateChecksum(void) {
  uint64_t ans = 0;
  for (int i = 0; i < content_size; i++) {
    if (content[i] != -1)
      ans += content[i] * i;
  }
  return ans;
}

int main(void) {
  FILE *fp = fopen(INPUT_FILE, "r");
  content = malloc(sizeof(int) * content_cap);
  readFile(fp);
  compactFiles();

  printf("Answer = %lu :)\n", calculateChecksum());

  free(content);
  fclose(fp);
  return 0;
}
