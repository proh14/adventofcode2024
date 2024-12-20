#define _DEFAULT_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_PROG 16
#define INPUT_FILE "../res/input.txt"
#define MAXBUF 256

int regA = 0;
int regB = 0;
int regC = 0;

int ip = 0;

int program[MAX_PROG];
int programLen = 0;

void readFile(FILE *fp) {
  char buf[MAXBUF];
  fgets(buf, MAXBUF, fp);
  sscanf(buf, "Register A: %d", &regA);
  fgets(buf, MAXBUF, fp);
  sscanf(buf, "Register B: %d", &regB);
  fgets(buf, MAXBUF, fp);
  sscanf(buf, "Register C: %d", &regC);
  getc(fp);

  fgets(buf, MAXBUF, fp);

  char *token;
  char *saveptr = strchr(buf, ' ') + 1;

  while ((token = strtok_r(saveptr, ",", &saveptr))) {
    if (programLen >= MAX_PROG) {
      printf("program too long\n");
      exit(1);
    }
    program[programLen++] = atoi(token);
  }
}

int oprandFromCombo(int combo) {

  // Combo operands 0 through 3 represent literal values 0 through 3.
  // Combo operand 4 represents the value of register A.
  // Combo operand 5 represents the value of register B.
  // Combo operand 6 represents the value of register C.
  // Combo operand 7 is reserved and will not appear in valid programs.

  switch (combo) {
  case 0:
    return 0;
  case 1:
    return 1;
  case 2:
    return 2;
  case 3:
    return 3;
  case 4:
    return regA;
  case 5:
    return regB;
  case 6:
    return regC;
  default:
    return -1;
    break;
  }
}

int executeInstruction(int instr, int oprand) {
  // Instruction 0: adv: A = A >> combo oprand
  // Instruction 1: bxl: B = B ^ literal oprand
  // Instruction 2: bst: B = combo oprand % 8
  // Instruction 3: jnz: if A != 0, ip = literal oprand
  // Instruction 4: bxc: B = B ^ C
  // Instruction 5: out: prints combo oprand % 8
  // Instruction 6: bdv: B = B >> combo oprand
  // Instruction 7: cdv: C = A >> combo oprand

  int ipInc = 2;
  switch (instr) {
  case 0:
    regA = regA >> oprandFromCombo(oprand);
    break;
  case 1:
    regB = regB ^ oprand;
    break;
  case 2:
    regB = oprandFromCombo(oprand) % 8;
    break;
  case 3:
    if (regA != 0) {
      ip = oprand;
      ipInc = 0;
    }
    break;
  case 4:
    regB = regB ^ regC;
    break;
  case 5:
    printf("%d,", oprandFromCombo(oprand) % 8);
    break;
  case 6:
    regB = regA >> oprandFromCombo(oprand);
    break;
  case 7:
    regC = regA >> oprandFromCombo(oprand);
    break;
  default:
    break;
  }

  return ipInc;
}

void runProgram(void) {
  while (ip < programLen) {
    int instr = program[ip];
    int oprand = program[ip + 1];
    ip += executeInstruction(instr, oprand);
  }
  printf("\n");
}

int main(void) {
  FILE *fp = fopen(INPUT_FILE, "r");

  readFile(fp);
  runProgram();

  fclose(fp);
}
