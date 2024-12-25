#include <stdio.h>
#include <z3.h>

int main(void) {
  // Instruction 0: adv: A = A >> combo oprand
  // Instruction 1: bxl: B = B ^ literal oprand
  // Instruction 2: bst: B = combo oprand % 8
  // Instruction 3: jnz: if A != 0, ip = literal oprand
  // Instruction 4: bxc: B = B ^ C
  // Instruction 5: out: prints combo oprand % 8
  // Instruction 6: bdv: B = B >> combo oprand
  // Instruction 7: cdv: C = A >> combo oprand

  // Combo operands 0 through 3 represent literal values 0 through 3.
  // Combo operand 4 represents the value of register A.
  // Combo operand 5 represents the value of register B.
  // Combo operand 6 represents the value of register C.
  // Combo operand 7 is reserved and will not appear in valid programs.

  Z3_config config = Z3_mk_config();
  Z3_context ctx = Z3_mk_context(config);
  Z3_del_config(config);

  Z3_sort bv_sort = Z3_mk_bv_sort(ctx, 63);
  Z3_ast A = Z3_mk_const(ctx, Z3_mk_string_symbol(ctx, "A"), bv_sort);

  int program[] = {0, 3, 5, 4, 3, 0};
  size_t program_len = sizeof(program) / sizeof(program[0]);

  Z3_solver solver = Z3_mk_solver(ctx);
  Z3_solver_inc_ref(ctx, solver);

  for (size_t i = 0; i < program_len; ++i) {
    A = Z3_mk_bvlshr(ctx, A, Z3_mk_int(ctx, 3, bv_sort));

    Z3_ast constraint =
        Z3_mk_eq(ctx, Z3_mk_bvand(ctx, A, Z3_mk_int(ctx, 7, bv_sort)),
                 Z3_mk_int(ctx, program[i], bv_sort));

    Z3_solver_assert(ctx, solver, constraint);
  }

  Z3_solver_assert(ctx, solver, Z3_mk_eq(ctx, A, Z3_mk_int(ctx, 0, bv_sort)));

  if (Z3_solver_check(ctx, solver) == Z3_L_TRUE) {
    Z3_model model = Z3_solver_get_model(ctx, solver);

    printf("%s", Z3_model_to_string(ctx, model));
    printf("(Convert to decimal)\n");
  } else {
    printf("No solution found :(\n");
  }

  Z3_solver_dec_ref(ctx, solver);
  Z3_del_context(ctx);
  return 0;
}
