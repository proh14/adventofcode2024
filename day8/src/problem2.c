#include <stdio.h>

#define INPUT_FILE "../res/input.txt"

#define COLS 50
#define ROWS 50

#define MAX_ANTINODE 1000
#define MAX_ANTENNA 500

struct antinode {
  int row;
  int col;
};

struct antenna {
  char freq;
  int row;
  int col;
};

int antenna_size = 0;
int antinode_size = 0;

struct antenna antennas[MAX_ANTENNA];
struct antinode antinodes[MAX_ANTINODE];

char content[ROWS][COLS] = {0};

void readInput(FILE *fp) {
  int c = 0, r = 0;

  int ch = 0;

  while ((ch = getc(fp)) != EOF) {
    if (ch != '\n') {
      content[r][c++] = ch;
      continue;
    }
    c = 0;
    r++;
  }
}

void findAntennas(void) {
  for (int i = 0; i < ROWS; i++) {
    for (int j = 0; j < COLS; j++) {
      if (content[i][j] != '.' && content[i][j] != '#' &&
          antenna_size < MAX_ANTENNA) {
        antennas[antenna_size].row = i;
        antennas[antenna_size].col = j;
        antennas[antenna_size++].freq = content[i][j];
      }
    }
  }
}

int doesAntinodeExist(int row, int col) {
  for (int i = 0; i < antinode_size; i++) {
    if (antinodes[i].row == row && antinodes[i].col == col)
      return 1;
  }
  return 0;
}

void addAntinode(int row, int col) {
  if (row < 0 || row >= ROWS || col < 0 || col >= COLS)
    return;
  if (doesAntinodeExist(row, col))
    return;

  antinodes[antinode_size].col = col;
  antinodes[antinode_size++].row = row;
}

int findAntenna(char freq, int i) {
  for (; i < antenna_size; i++) {
    if (freq == antennas[i].freq)
      return i;
  }
  return -1;
}

void findAntinodes(void) {
  int j = 0;
  for (int i = 0; i < antenna_size; i++) {
    while (1) {
      j = findAntenna(antennas[i].freq, j);
      if (j == -1)
        break;
      if (j == i) {
        j++;
        continue;
      }
      int dc = antennas[i].col - antennas[j].col;
      int dr = antennas[i].row - antennas[j].row;
      int k = 0;
      while (1) {
        int antic = antennas[i].col + (dc * k);
        int antir = antennas[i].row + (dr * k);
        addAntinode(antir, antic);

        if (antir < 0 || antir >= ROWS || antic < 0 || antic >= COLS)
          break;

        k++;
      }
      j++;
    }
  }
}

int main(void) {
  FILE *fp = fopen(INPUT_FILE, "r");

  readInput(fp);
  findAntennas();
  findAntinodes();

  printf("Answer = %d :)\n", antinode_size);

  fclose(fp);
  return 0;
}
