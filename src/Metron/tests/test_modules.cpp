#include "test_utils.h"

//------------------------------------------------------------------------------

static TestResults test_comb_assign() {
  TEST_INIT(
      "Assignments in always_ff should be non-blocking, assignments in "
      "always_comb should be blocking");
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

  EXPECT(!find_iws(out, "count ="),
         "Assignments to 'count' should always be non-blocking.");
  EXPECT(+find_iws(out, "count <="),
         "Assignments to 'count' should always be non-blocking.");

  EXPECT(+find_iws(out, "o_blep ="),
         "Assignments to 'o_count' should always be blocking.");
  EXPECT(!find_iws(out, "o_blep <="),
         "Assignments to 'o_count' should always be blocking.");

  TEST_DONE();
}

//------------------------------------------------------------------------------

static TestResults test_simple_module() {
  TEST_INIT("Make sure we can translate a trivial module.");
  LOG_INDENT_SCOPE();

  std::string src = R"(
struct test {
  logic<8> count;
  logic<8> o_count;

  void tick(logic<1> i_rst, logic<1> i_serial) {
    if (i_rst) {
      count = 0;
    } else {
      count = count + i_serial;
    }
  }

  void tock() {
    o_count = count;
  }
};
)";

  auto out = translate_simple(src);

  std::string expected = 1 + R"(
`default_nettype none
module test
(
input logic clk,
input logic i_rst,
input logic i_serial,
output logic[7:0] o_count
);
  logic[7:0] count;
  always_ff @(posedge clk) begin : tick
    if (i_rst) begin
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

  EXPECT(comp_iws(expected, out));

  TEST_DONE();
}

//------------------------------------------------------------------------------

static TestResults test_simple_dirty() {
  TEST_INIT();

  // Reading a field before writing it is OK.
  EXPECT_NE("DCF", translate_simple(R"(
  struct test {
    logic<8> foo;
    logic<8> bar;
    void tick() { foo = bar; bar = 1; }
  };)"));

  // Reading a field after writing it should trigger a dirty check fail.
  EXPECT_EQ("DCF", translate_simple(R"(
  struct test {
    logic<8> foo;
    logic<8> bar;
    void tick() { bar = 1; foo = bar; }
  };)"));

  // Reading an output after writing it is OK.
  EXPECT_NE("DCF", translate_simple(R"(
  struct test {
    logic<8> o_foo;
    logic<8> o_bar;
    void tock() { o_bar = 1; o_foo = o_bar; }
  };)"));

  // Reading an output before writing it should trigger a dirty check fail.
  EXPECT_EQ("DCF", translate_simple(R"(
  struct test {
    logic<8> o_foo;
    logic<8> o_bar;
    void tock() { o_foo = o_bar; o_bar = 1; }
  };)"));

  TEST_DONE();
}

//------------------------------------------------------------------------------

TestResults test_modules() {
  TEST_INIT();
  results += test_comb_assign();
  results += test_simple_module();
  results += test_simple_dirty();
  TEST_DONE();
}

//------------------------------------------------------------------------------
