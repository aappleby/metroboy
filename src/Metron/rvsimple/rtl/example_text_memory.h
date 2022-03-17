// RISC-V SiMPLE SV -- text memory model
// BSD 3-Clause License
// (c) 2017-2019, Arthur Matos, Marcus Vinicius Lamar, Universidade de Brasília,
//                Marek Materzok, University of Wrocław

#ifndef RVSIMPLE_EXAMPLE_TEXT_MEMORY_H
#define RVSIMPLE_EXAMPLE_TEXT_MEMORY_H

#include "config.h"
#include "constants.h"
#include "metron_tools.h"

using namespace rv_config;

class example_text_memory {
 public:
  logic<32> mem[pow2(TEXT_BITS - 2)];
  logic<32> q;

  void init() {
    std::string s;
    value_plusargs("text_file=%s", s);
    readmemh(s, mem);
  }

  void tock(logic<TEXT_BITS - 2> address) { q = mem[address]; }
};

#endif  // RVSIMPLE_EXAMPLE_TEXT_MEMORY_H