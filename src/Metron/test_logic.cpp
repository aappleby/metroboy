#include "Logic.h"
#include "test_utils.h"

/*
  logic<27> a = -8;
  printf("%d\n", int(a));
  printf("%d\n", int(a.as_signed()));
  printf("0x%08x\n", int(a));
  printf("0x%08x\n", int(a.as_signed()));
  printf("0x%08x\n", int(a >> 2));
  printf("0x%08x\n", sra(a, 2));
  return 0;
*/

//------------------------------------------------------------------------------

TestResults test_logic_truncate() {
  TEST_INIT();

  logic<10> a = 0b1111111111;
  logic<5> b = 0;

  b = a + 0;
  EXPECT_EQ(b, 0b11111,
            "Assignment after addition should compile and truncate.");

  TEST_DONE();
}

TestResults test_logic_add() {
  TEST_INIT();

  logic<8> a = 100;
  logic<8> b = 72;
  logic<8> c = a + b;

  EXPECT_EQ(c, 172);

  TEST_DONE();
}

TestResults test_logic_cat() {
  TEST_INIT("test_logic_cat");

  logic<5> x = cat(logic<2>(0b10), logic<3>(0b101));

  EXPECT_EQ(x[4], 1);
  EXPECT_EQ(x[3], 0);
  EXPECT_EQ(x[2], 1);
  EXPECT_EQ(x[1], 0);
  EXPECT_EQ(x[0], 1);

  TEST_DONE();
}

TestResults test_logic_slice() {
  TEST_INIT();

  logic<12> a = 0x00000000;

  a = 0;
  slice<7, 0>(a) = 0xFF;
  EXPECT_EQ(0x00FF, a);
  a = 0;
  slice<8, 1>(a) = 0xFF;
  EXPECT_EQ(0x01FE, a);
  a = 0;
  slice<9, 2>(a) = 0xFF;
  EXPECT_EQ(0x03FC, a);
  a = 0;
  slice<10, 3>(a) = 0xFF;
  EXPECT_EQ(0x07F8, a);
  a = 0;
  slice<11, 4>(a) = 0xFF;
  EXPECT_EQ(0x0FF0, a);
  a = 0;
  slice<12, 5>(a) = 0xFF;
  EXPECT_EQ(0x0FE0, a);
  a = 0;
  slice<13, 6>(a) = 0xFF;
  EXPECT_EQ(0x0FC0, a);
  a = 0;
  slice<14, 7>(a) = 0xFF;
  EXPECT_EQ(0x0F80, a);
  a = 0;
  slice<15, 8>(a) = 0xFF;
  EXPECT_EQ(0x0F00, a);

  logic<20> b = 0x0FF00;

  EXPECT_EQ(0x00FF, b12(b, 8));
  EXPECT_EQ(0x01FE, b12(b, 7));
  EXPECT_EQ(0x03FC, b12(b, 6));
  EXPECT_EQ(0x07F8, b12(b, 5));
  EXPECT_EQ(0x0FF0, b12(b, 4));
  EXPECT_EQ(0x0FE0, b12(b, 3));
  EXPECT_EQ(0x0FC0, b12(b, 2));
  EXPECT_EQ(0x0F80, b12(b, 1));
  EXPECT_EQ(0x0F00, b12(b, 0));

  TEST_DONE();
}

TestResults test_logic_dup() {
  TEST_INIT();
  logic<3> boop = 0b101;
  logic<9> moop = dup<3>(boop);
  EXPECT_EQ(moop, 0b101101101);
  TEST_DONE();
}

//------------------------------------------------------------------------------

TestResults test_logic_decl() {
  TEST_INIT("Translating logic<...> should produce valid declarators");
  LOG_INDENT_SCOPE();

  // One-bit logics should _not_ turn into logic[0:0].
  auto src1 = "struct test { logic<1> n; };";
  EXPECT(!find_iws(translate_simple(src1), "logic[0:0]"));
  EXPECT(find_iws(translate_simple(src1), "logic n"));

  // Multi-bit logics with a literal size should turn into logic[(N-1):0];
  auto src2 = "struct test { logic<2> n; };";
  EXPECT(find_iws(translate_simple(src2), "logic[1:0] n"));

  // Multi-bit logics with a non-literal size should turn into
  // logic[<name>-1:0];
  auto src3 = "struct test { logic<foo> n; };";
  EXPECT(find_iws(translate_simple(src3), "logic[foo-1:0] n"));

  TEST_DONE();
}

//------------------------------------------------------------------------------

TestResults test_logic() {
  TEST_INIT("Test logic<> behavior and translation");

  results += test_logic_truncate();
  results += test_logic_add();
  results += test_logic_cat();
  results += test_logic_slice();
  results += test_logic_dup();
  // results += test_logic_as_signed();

  results += test_logic_decl();

  TEST_DONE();
}

//------------------------------------------------------------------------------
