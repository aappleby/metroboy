#include "test_utils.h"

#include "MtModLibrary.h"

//------------------------------------------------------------------------------
// The tock functions may have valid deltas individually, but the module may
// still not be linearizable.

const char* tangle = R"(
struct test {
  logic<8> o_out1;
  logic<8> o_out2;

  void tock1() {
    o_out1 = o_out2;
  }

  void tock2() {
    o_out2 = o_out1;
  }
};
)";


TestResults test_tock_tangle() {
  TEST_INIT();

  //auto dst = translate_simple(tangle);
  //printf("%s\n", dst.c_str());

  MtModLibrary lib;
  parse_simple(tangle, lib);

  //auto mod = lib.modules[0];

  TEST_DONE();
}

//------------------------------------------------------------------------------

TestResults test_deltas() {
  TEST_INIT();
  results += test_tock_tangle();
  TEST_DONE();
}

//------------------------------------------------------------------------------
