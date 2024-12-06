#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define INPUT_FILE "../res/input.txt"

int main(void) {
  FILE *fp = fopen(INPUT_FILE, "r");

  fseek(fp, 0, SEEK_END);
  long fsize = ftell(fp);
  fseek(fp, 0, SEEK_SET);

  char *content = malloc(sizeof(char) * fsize + 1);

  fread(content, fsize, 1, fp);

  char *ptr = content;

  int ans = 0;
  while ((ptr = strstr(ptr, "mul(")) != NULL) {
    ptr += 4;

    char num1[10];
    char num2[10];

    int i = 0;
    while (isdigit(*ptr)) {
      num1[i] = *ptr;
      i++;
      ptr++;
    }
    if (!i)
      continue;

    num1[i] = '\0';

    if (*ptr != ',')
      continue;
    ptr++;

    i = 0;
    while (isdigit(*ptr)) {
      num2[i] = *ptr;
      i++;
      ptr++;
    }

    if (!i)
      continue;

    num2[i] = '\0';

    if (*ptr != ')')
      continue;

    int x = atoi(num1);
    int y = atoi(num2);

    ans += x * y;

    ptr++;
  }

  printf("Answer = %d :)\n", ans);

  free(content);
  fclose(fp);
  return 0;
}
