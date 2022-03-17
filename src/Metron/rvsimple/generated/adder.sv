// RISC-V SiMPLE SV -- adder module
// BSD 3-Clause License
// (c) 2017-2019, Arthur Matos, Marcus Vinicius Lamar, Universidade de Brasília,
//                Marek Materzok, University of Wrocław

`ifndef RVSIMPLE_ADDER_H
`define RVSIMPLE_ADDER_H

`include "config.sv"
`include "constants.sv"
`include "metron_tools.sv"

module adder
#(parameter int WIDTH = 32)
(
  input logic clock,
  input logic[WIDTH-1:0] operand_a,
  input logic[WIDTH-1:0] operand_b,
  output logic[WIDTH-1:0] result
);
 /*public:*/
  /*logic<WIDTH> result;*/

  always_comb begin : tock
    result = operand_a + operand_b;
  end
endmodule

`endif  // RVSIMPLE_ADDER_H
