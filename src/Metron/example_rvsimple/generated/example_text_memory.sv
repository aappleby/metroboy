// RISC-V SiMPLE SV -- text memory model
// BSD 3-Clause License
// (c) 2017-2019, Arthur Matos, Marcus Vinicius Lamar, Universidade de Brasília,
//                Marek Materzok, University of Wrocław

`ifndef RVSIMPLE_EXAMPLE_TEXT_MEMORY_H
`define RVSIMPLE_EXAMPLE_TEXT_MEMORY_H

`include "config.sv"
`include "constants.sv"
`include "metron_tools.sv"

import rv_config::*;

module example_text_memory
(
  input logic clock,
  input logic[TEXT_BITS - 2-1:0] address,
  output logic[31:0] q
);
 /*public:*/
  logic[31:0] mem[2**(TEXT_BITS - 2)];
  /*logic<32> q;*/

  initial begin : init
    string s;
    
    $value$plusargs("text_file=%s", s);
    $readmemh(s, mem);
  end

  always_comb begin : tock q = mem[address]; end
endmodule;

`endif  // RVSIMPLE_EXAMPLE_TEXT_MEMORY_H
