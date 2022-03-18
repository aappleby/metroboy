#if 0

//------------------------------------------------------------------------------

void test_ibex_alu() {
  using namespace ibex_pkg;

  ibex_alu<rv32b_e::RV32BNone> alu;

  logic<1>  instr_first_cycle_i = 0;
  alu_op_e  operator_i = alu_op_e::ALU_XPERM_N;
  logic<32> operand_a_i = 0;
  logic<32> operand_b_i = 0;

  alu.tock1(0);
  alu.tock2(ibex_pkg::alu_op_e::ALU_XPERM_N);
  alu.tock3(0, 0, 0, 0, 0);
  alu.tock4(ibex_pkg::alu_op_e::ALU_XPERM_N, 0, 0);
  alu.tock5(instr_first_cycle_i, operator_i, operand_a_i, operand_b_i);
  alu.tock6(0, ibex_pkg::alu_op_e::ALU_XPERM_N, 0, 0);
  alu.tock8(ibex_pkg::alu_op_e::ALU_XPERM_N, 0, 0);
}

void test_ibex_multdiv_slow() {
  using namespace ibex_pkg;

  ibex_multdiv_slow multdiv;

  logic<34> imd_val_q_i[2];

  multdiv.tick(false);
  multdiv.tock1(0, 0, 0, 0, md_op_e::MD_OP_DIV, 0, 0, 0, 0, 0, 0, 0, imd_val_q_i, 0);
  multdiv.tock2(0, 0, 0, 0, md_op_e::MD_OP_DIV, 0, 0, 0, 0, 0, 0, 0, imd_val_q_i, 0);
  multdiv.tock3(false, md_op_e::MD_OP_DIV, 0);
  multdiv.tock4(0ull);
  multdiv.tock5(0, 0);
  multdiv.tock6(0, 0, 0);
  multdiv.tock7(imd_val_q_i);
}

void test_rvs() {
  toplevel t;
  t.tick(1);
  t.tock();
  t.tick(0);
  t.tock();
}
#endif
