// RISC-V SiMPLE SV -- multiplexer module
// BSD 3-Clause License
// (c) 2017-2019, Arthur Matos, Marcus Vinicius Lamar, Universidade de Brasília,
//                Marek Materzok, University of Wrocław

`ifndef RVSIMPLE_MULTIPLEXER_H
`define RVSIMPLE_MULTIPLEXER_H


`include "config.sv"
`include "constants.sv"
`include "metron_tools.sv"

module multiplexer
#(parameter int WIDTH = 32, parameter int CHANNELS = 2, parameter int SEL_BITS = $clog2(CHANNELS))
(
  input logic clock,
  input logic[WIDTH-1:0] in_bus[CHANNELS],
  input logic[SEL_BITS-1:0] sel,
  output logic[WIDTH-1:0] out
);
 /*public:*/
  /*logic<WIDTH> out;*/

  always_comb begin : tock
    out = in_bus[sel];
  end
endmodule

`endif  // RVSIMPLE_MULTIPLEXER_H
