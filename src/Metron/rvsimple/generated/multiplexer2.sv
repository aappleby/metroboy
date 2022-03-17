// RISC-V SiMPLE SV -- multiplexer module
// BSD 3-Clause License
// (c) 2017-2019, Arthur Matos, Marcus Vinicius Lamar, Universidade de Brasília,
//                Marek Materzok, University of Wrocław

`ifndef RVSIMPLE_MULTIPLEXER2_H
`define RVSIMPLE_MULTIPLEXER2_H

`include "config.sv"
`include "constants.sv"
`include "metron_tools.sv"

module multiplexer2
#(parameter int WIDTH = 32)
(
  input logic clock,
  input logic sel,
  input logic[WIDTH-1:0] in0,
  input logic[WIDTH-1:0] in1,
  output logic[WIDTH-1:0] out
);
 /*public:*/
  /*logic<WIDTH> out;*/

  always_comb begin : tock
    case (sel) 
      /*case*/ 0:
        out = in0;
        /*break;*/
      /*case*/ 1:
        out = in1;
        /*break;*/
      default:
        out = WIDTH'(1'bx);
        /*break;*/
    endcase
  end
endmodule

`endif  // RVSIMPLE_MULTIPLEXER2_H

