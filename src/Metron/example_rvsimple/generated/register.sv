// RISC-V SiMPLE SV -- generic register
// BSD 3-Clause License
// (c) 2017-2019, Arthur Matos, Marcus Vinicius Lamar, Universidade de Brasília,
//                Marek Materzok, University of Wrocław

`ifndef RVSIMPLE_REGISTER_H
`define RVSIMPLE_REGISTER_H

`include "config.sv"
`include "constants.sv"
`include "metron_tools.sv"

module single_register
#(parameter int WIDTH = 32, parameter int INITIAL = 0)
(
  input logic clock,
  input logic reset,
  input logic write_enable,
  input logic[WIDTH-1:0] next,
  output logic[WIDTH-1:0] value
);
 /*public:*/
  /*logic<WIDTH> value;*/

  initial begin : init reg_value = INITIAL; end

  always_ff @(posedge clock) begin : tick
    if (reset)
      reg_value <= INITIAL;
    else if (write_enable)
      reg_value <= next;
  end

  always_comb begin : tock value = reg_value; end

 /*private:*/
  logic[WIDTH-1:0] reg_value;
endmodule

`endif  // RVSIMPLE_REGISTER_H
