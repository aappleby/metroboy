#if 0

logic<3> boop = 0b101;
logic<9> moop = dup<3>(boop);
assert(moop == 0b101101101);


#include "../../CoreLib/Tests.h"
#include "../../CoreLib/Log.h"

TestResults logic_test_truncate() {
  TEST_INIT("logic_test_truncate");

  logic<10> a = 0b1111111111;
  logic<5>  b = 0;

  // A direct assignment should not compile
  //b = a;

  // Assignment after addition should compile and truncate.
  b = a + 0;
  
  EXPECT_EQ(b, 0b11111);

  TEST_DONE();
}

TestResults logic_test_add() {
  TEST_INIT("logic_test_add");

  logic<8> a = 100;
  logic<8> b = 72;
  logic<8> c = a + b;

  EXPECT_EQ(c, 172);

  TEST_DONE();
}

TestResults logic_test_cat() {
  TEST_INIT("logic_test_cat");

  logic<5> x = cat(logic<2>(0b10), logic<3>(0b101));

  EXPECT_EQ(x[4], 1);
  EXPECT_EQ(x[3], 0);
  EXPECT_EQ(x[2], 1);
  EXPECT_EQ(x[1], 0);
  EXPECT_EQ(x[0], 1);

  TEST_DONE();
}

TestResults logic_test_proxy() {
  TEST_INIT(__FUNCTION__);

  logic<8> a = 0;
  a[3] = 1;
  EXPECT_EQ(8, a);

  TEST_DONE();
}

TestResults logic_test_suite() {
  TEST_INIT("logic_test_suite");

  results += logic_test_cat();
  results += logic_test_add();
  results += logic_test_truncate();
  results += logic_test_proxy();

  TEST_DONE();
}

#if 0
int main(int argc, char** argv) {

  auto r = logic_test_suite();

  return 0;
}
#endif


#if 0
#include "Logic.h"

#include "uart/uart_rx.h"
#include "uart/uart_tx.h"
#include "uart/uart_hello.h"
#include "uart/uart_top.h"

#include "../uart_test/ibex_pkg.h"
#include "../uart_test/ibex_alu.h"
#include "../uart_test/ibex_multdiv_slow.h"

#include "../../riscv-simple-sv/synth/config.h"
#include "../../riscv-simple-sv/core/common/constants.h"
#include "../../riscv-simple-sv/core/common/adder.h"
#include "../../riscv-simple-sv/core/common/alu.h"

#include "../../riscv-simple-sv/core/common/alu_control.h"
#include "../../riscv-simple-sv/core/common/control_transfer.h"
#include "../../riscv-simple-sv/core/common/data_memory_interface.h"
#include "../../riscv-simple-sv/core/common/immediate_generator.h"
#include "../../riscv-simple-sv/core/common/instruction_decoder.h"
#include "../../riscv-simple-sv/core/common/multiplexer.h"
#include "../../riscv-simple-sv/core/common/multiplexer2.h"
#include "../../riscv-simple-sv/core/common/multiplexer4.h"
#include "../../riscv-simple-sv/core/common/multiplexer8.h"
#include "../../riscv-simple-sv/core/common/regfile.h"
#include "../../riscv-simple-sv/core/common/register.h"
#include "../../riscv-simple-sv/core/common/example_text_memory.h"
#include "../../riscv-simple-sv/core/common/example_text_memory_bus.h"
#include "../../riscv-simple-sv/core/common/example_data_memory.h"
#include "../../riscv-simple-sv/core/common/example_data_memory_bus.h"
#include "../../riscv-simple-sv/core/singlecycle/riscv_core.h"
#include "../../riscv-simple-sv/core/singlecycle/singlecycle_control.h"
#include "../../riscv-simple-sv/core/singlecycle/singlecycle_ctlpath.h"
#include "../../riscv-simple-sv/core/singlecycle/singlecycle_datapath.h"
#include "../../riscv-simple-sv/core/singlecycle/toplevel.h"

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


void test_uart() {
  uart_top t;
  t.tick(false);
  t.tock(false);
}

  {
    logic<32> a = 0x00000000;
    slice<12>(a, 12) = 0x123;
    printf("a = 0x%08x\n", a.x); // should print "a = 0x00123000"
  }

  {
    logic<33> op_b_shift_q;
    op_b_shift_q = b33(0x0);
  }

  {
    logic<7> a = 0;
    s32(a) = 0xDEADBEEF;
    printf("a = 0x%08x\n", a.get());
    printf("sizeof(a) = %d\n", (int)sizeof(a));
  }


  {
    logic<16> a = b16(0x12345678, 8);
    logic<16> b;
    s8(b, 8) = b8(a);
    printf("0x%08x\n", b.get());
  }

  {
    //logic<8> a = 1;
    //logic<9> b = a;
  }

  {
    const uint32_t temp = 0x0F0F0F0F;
    for (int i = 0; i < 32; i++) {
      logic<8> a = b8(temp, i);
      //logic<8> a = make_slice<8, uint32_t, 32>(temp, i);
      assert(a.get() == ((temp >> i) & 0xFF));
    }
  }

  {
    uint32_t temp = 0x00000000;
    for (int i = 0; i < 32; i++) {
      s1(temp, i) = 1;
      assert(temp == 0xFFFFFFFF >> (32 - i - 1));
    }
  }

  {
    const logic<32> temp = b32(0x0F0F0F0F);
    for (int i = 0; i < 32; i++) {
      logic<8> a = b8(temp, i);
      assert(a == ((temp >> i) & 0xFF));
    }
  }

  {
    logic<32> temp = b32(0x00000000);
    for (int i = 0; i < 32; i++) {
      s1(temp, i) = 1;

      //printf("0x%08x 0x%08x\n", temp.get(), 0xFFFFFFFF >> (32 - i - 1));

      assert(temp == 0xFFFFFFFF >> (32 - i - 1));
    }
  }

  {
    logic<3> a = 0b100;
    int8_t b = a.as_signed();
    assert(a == 4);
    assert(b == -4);
  }

  {
    logic<32> a = b32(0x80000000);
    logic<5>  b = b5(15);

    logic<32> c = b32(unsigned(a) >> b);
    logic<32> d = b32(  signed(a) >> b);

    assert(c == 0x00010000);
    assert(d == 0xFFFF0000);
  }



//------------------------------------------------------------------------------

template<int N>
struct blogic {
  typedef typename bitsize_to_basetype<N>::type type;

  blogic() {}

  blogic& operator=(uint64_t y) { return *this; }

  operator uint64_t() const { return 0; }

  template<int M>
  blogic(logic<M> b) { static_assert(false); }

  template<int M>
  blogic& operator=(const logic<M> b) { static_assert(false); }
  template<int M>
  blogic& operator=(const logic<M>& b) { static_assert(false); }

private:

  type x;
};

void blogic_test() {
  blogic<7> a;
  blogic<8> b;
  b = a;
}


//------------------------------------------------------------------------------

__declspec(noinline) uint32_t test_bitproxy(int offset) {
  logic<32> x = 0;
  b10(x, offset) = 0b1111111111;
  return uint32_t(x);
}

template<int N>
void test_reduce_xor() {
  for (uint64_t i = 0; i < (1ull << N); i++) {
    int parity1 = 0;
    for (int j = 0; j < N; j++) {
      parity1 ^= (i >> j) & 1;
    }
    int parity2 = reduce_xor<N>(i);
    if (parity1 != parity2) {
      printf("%d %d %d\n", i, parity1, parity2);
    }
  }
}


#endif