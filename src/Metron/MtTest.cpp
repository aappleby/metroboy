#include "Platform.h"
#include "MtModule.h"
#include "MtModLibrary.h"
#include "MtCursor.h"
#include "MtSourceFile.h"

#include <algorithm>

#include "../CoreLib/Log.h"
#include "../CoreLib/Tests.h"

#include "Logic.h"

#include "uart/uart_rx.h"
#include "uart/uart_tx.h"
#include "uart/uart_hello.h"
#include "uart/uart_top.h"

#include "ibex/ibex_pkg.h"
#include "ibex/ibex_alu.h"
#include "ibex/ibex_multdiv_slow.h"

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
// Compare strings and find substrings, ignoring all whitespace. Yes, some
// whitespace is syntactically relevant - we are intentionally not checking that
// here.

bool comp_iws(const char* a, const char* b) {
  if (!a) return false;
  if (!b) return false;

  while(1) {
    while(isspace(*a)) a++;
    while(isspace(*b)) b++;
    if (*a != *b) return false;
    if (*a == 0) break;
    a++;
    b++;
  }

  return true;
}

bool comp_iws(const std::string& a, const std::string& b) {
  return comp_iws(a.c_str(), b.c_str());
}

bool find_iws(const char* a, const char* b) {
  //printf("%s\n", a);
  //printf("%s\n", b);

  if (!a) return false;
  if (!b) return false;

  const char* c = a;
  const char* d = b;

  while(1) {
    while(*c && isspace(*c)) c++;
    while(*d && isspace(*d)) d++;
    if (*d == 0) return true;
    if (*c == 0) return false;
    if (*c != *d) {
      c++;
      d = b;
    }
    else {
      c++;
      d++;
    }
  }
}

bool find_iws(const std::string& a, const std::string& b) {
  return find_iws(a.c_str(), b.c_str());
}

//------------------------------------------------------------------------------

std::string translate_simple(std::string src) {
  blob src_blob;
  src_blob.insert(src_blob.begin(), src.begin(), src.end());
  std::string out;

  MtModLibrary library;

  auto source_file = new MtSourceFile();
  source_file->parse_source("test.h", src_blob);

  auto mod = new MtModule();
  mod->source_file = source_file;
  mod->load_pass1();
  mod->lib = &library;
  mod->load_pass2();
  mod->check_dirty_ticks();
  mod->check_dirty_tocks();

  library.modules.push_back(mod);

  MtCursor cursor(mod, &out);
  cursor.cursor = mod->source_file->source;
  cursor.emit(mod->mod_root);
  cursor.emit("\n");
  for (auto c : out) assert(c > 0);
  out.push_back(0);

  if (0) {
    printf("//----------------------------------------\n");
    mod->dump_banner();
    printf("\n");
    printf(out.c_str());
    printf("//----------------------------------------\n");
  }

  delete mod;
  return out;
}

//------------------------------------------------------------------------------

TestResults test_dummy() {
  TEST_INIT();
  LOG_INDENT_SCOPE();
  TEST_DONE();
}

//------------------------------------------------------------------------------

TestResults test_match() {
  TEST_INIT("Make sure our whitespace-insensitive substring match works.");
  LOG_INDENT_SCOPE();

  EXPECT(+find_iws("foobarbaz",   "bar"));
  EXPECT(+find_iws("foo bar baz", "bar"));
  EXPECT(+find_iws("foo bar baz", "b ar"));
  EXPECT(+find_iws("foo bar baz", "ba r"));
  EXPECT(!find_iws("foobarbaz",   "gar"));
  EXPECT(!find_iws("foo bar baz", "gar"));
  EXPECT(!find_iws("foo bar baz", "g ar"));
  EXPECT(!find_iws("foo bar baz", "ga r"));

  EXPECT(+find_iws("foobarbaz",   "foo"));
  EXPECT(+find_iws("foobarbaz",   "baz"));

  EXPECT(!find_iws("foobarba",    "baz"));

  TEST_DONE();
}

//------------------------------------------------------------------------------

TestResults test_logic_decl() {
  TEST_INIT("Translating logic<...> should produce valid declarators");
  LOG_INDENT_SCOPE();

  // One-bit logics should _not_ turn into logic[0:0].
  auto src1 = "struct test { logic<1> n; };";
  EXPECT(!find_iws(translate_simple(src1), "logic[0:0]"));

  // Multi-bit logics with a literal size should turn into logic[(N-1):0];
  auto src2 = "struct test { logic<2> n; };";
  EXPECT(find_iws(translate_simple(src2), "logic[1:0]"));

  // Multi-bit logics with a non-literal size should turn into logic[<name>-1:0];
  auto src3 = "struct test { logic<foo> n; };";
  EXPECT(find_iws(translate_simple(src3), "logic[foo-1:0]"));

  TEST_DONE();
}

//------------------------------------------------------------------------------

TestResults test_comb_assign() {
  TEST_INIT("Assignments in always_ff should be non-blocking, assignments in always_comb should be blocking");
  LOG_INDENT_SCOPE();

  std::string src = R"(
struct test {
  logic<8> count;
  logic<8> o_blep;

  void tick(bool rst_n, logic<1> i_serial) {
    if (!rst_n) {
      count = 0;
    } else {
      count = count + 1;
    }
  }

  void tock(bool rst_n) {
    o_blep = count;
  }
};)";

  auto out = translate_simple(src);

  EXPECT(!find_iws(out, "count ="),   "Assignments to 'count' should always be non-blocking.");
  EXPECT(+find_iws(out, "count <="),  "Assignments to 'count' should always be non-blocking.");

  EXPECT(+find_iws(out, "o_blep ="),  "Assignments to 'o_count' should always be blocking.");
  EXPECT(!find_iws(out, "o_blep <="), "Assignments to 'o_count' should always be blocking.");

  TEST_DONE();
}

//------------------------------------------------------------------------------

TestResults test_simple_module() {
  TEST_INIT("Make sure we can translate a trivial module.");
  LOG_INDENT_SCOPE();

  std::string src = 1 + R"(
struct test {
  logic<8> count;
  logic<8> o_count;

  void tick(bool rst_n, logic<1> i_serial) {
    if (!rst_n) {
      count = 0;
    } else {
      count = count + i_serial;
    }
  }

  void tock(bool rst_n) {
    o_count = count;
  }
};
)";

  std::string expected = 1 + R"(
`default_nettype none

module test(input logic clk,input logic rst_n,input logic i_serial,output logic[7:0] o_count);
  logic[7:0] count;
  logic[7:0] o_count;

  always_ff @(posedge clk, negedge rst_n) begin : tick
    if (!rst_n) begin
      count <= 0;
    end else begin
      count <= count + i_serial;
    end
  end

  always_comb begin : tock
    o_count = count;
  end
endmodule
)";

  EXPECT(comp_iws(expected, translate_simple(src)));

  TEST_DONE();
}

//------------------------------------------------------------------------------

void metron_test_suite() {

  TestResults results;
  {
    results += test_match();
    results += test_comb_assign();
    results += test_logic_decl();
    results += test_simple_module();
  }
  LOG_G("%s: %6d expect pass\n", __FUNCTION__, results.expect_pass);
  LOG_G("%s: %6d test pass\n", __FUNCTION__,   results.test_pass);

  LOG_R("%s: %6d expect fail\n", __FUNCTION__, results.expect_fail);
  LOG_R("%s: %6d test fail\n", __FUNCTION__,   results.test_fail);

  if (results.test_fail) {
    LOG_R("\n");
    LOG_R("########################################\n");
    LOG_R("##               FAIL                 ##\n");
    LOG_R("########################################\n");
    LOG_R("\n");
  }

  /*
  std::string src_in = R"(
  struct test {
    logic<8> count;
    logic<8> o_count;

    void tick(bool rst_n, logic<1> i_serial) {
      if (!rst_n) {
        count = 0;
      } else {
        count = count + 1;
      }
    }

    void tock(bool rst_n) {
      o_count = count;
    }
  };
  )";

  std::string expected = R"(
  `default_nettype none

  module test
  (
  input logic clk,
  input logic rst_n,
  );
    logic[7:0] count;
    logic[7:0] o_count;

    always_ff @(posedge clk, negedge rst_n) begin : tick
      if (!rst_n) begin
        count = 0;
      end else begin
        count = count + 1;
      end
    end

    always_comb begin : tock
      o_count = count;
    end
  endmodule
  )";

  std::string src_out = translate(src_in);

  auto len = src_out.size() < expected.size() ? src_out.size() : expected.size();

  if (comp_no_whitespace(expected, src_out)) printf("MATCH\n");
  */
}

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