#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define _POSIB_IMPLEMENTATION_
#include "possibilities.h"

#define INPUT_FILE "../res/input.txt"

#define UP 0
#define DIR_A 1
#define LEFT 2
#define DOWN 3
#define RIGHT 4
#define NUMPAD_A 10

#define MAX_SEQ 256
#define BUFSIZE 64

#define ROBOTS 2

char *numberToSequence(char from, char to) {
  int f = from == 'A' ? NUMPAD_A : from - '0';
  int t = to == 'A' ? NUMPAD_A : to - '0';
  return shortest_keypad[f][t];
}

int moveToNumber(char move) {
  switch (move) {
  case '^':
    return UP;
    break;
  case 'v':
    return DOWN;
    break;
  case '<':
    return LEFT;
    break;
  case '>':
    return RIGHT;
    break;
  case 'A':
    return DIR_A;
    break;
  default:
    return -1;
    break;
  }
}

char *moveToSequence(char from, char to) {
  int f = moveToNumber(from);
  int t = moveToNumber(to);
  return shortest_dirpad[f][t];
}

void comboToSequence(char *seq, char *combo) {
  int i = 0;
  int j = 0;
  char from = 'A';
  while (combo[i] != '\n' && combo[i] != '\0') {
    char *seqs = numberToSequence(from, combo[i]);
    while (*seqs != '\0') {
      seq[j] = *seqs;
      seqs++;
      j++;
    }
    seq[j++] = 'A';
    from = combo[i];
    i++;
  }
  seq[j] = '\0';
}

void sequenceToSequence(char *from, char *to) {
  int i = 0;
  int j = 0;
  int f = 'A';
  while (from[i] != '\0') {
    char *seqs = moveToSequence(f, from[i]);
    while (*seqs != '\0') {
      to[j] = *seqs;
      seqs++;
      j++;
    }
    to[j++] = 'A';
    f = from[i];
    i++;
  }
  to[j] = '\0';
}

int calculateComplexity(FILE *fp) {
  char buf[BUFSIZE];

  int complexity = 0;

  while (fgets(buf, BUFSIZE, fp) != NULL) {
    char *seq1 = malloc(MAX_SEQ * sizeof(char));
    char *seq2 = malloc(MAX_SEQ * sizeof(char));

    comboToSequence(seq1, buf);

    for (int i = 0; i < ROBOTS; i++) {
      sequenceToSequence(seq1, seq2);
      char *tmp = seq1;
      seq1 = seq2;
      seq2 = tmp;
    }
    buf[strlen(buf) - 2] = '\0';
    int number = atoi(buf);
    number *= strlen(seq1);
    complexity += number;
    free(seq1);
    free(seq2);
  }
  return complexity;
}

int main(void) {
  FILE *fp = fopen(INPUT_FILE, "r");

  printf("Answer = %d :)\n", calculateComplexity(fp));

  fclose(fp);
  return 0;
}
