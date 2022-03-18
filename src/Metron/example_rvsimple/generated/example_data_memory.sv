// RISC-V SiMPLE SV -- data memory model
// BSD 3-Clause License
// (c) 2017-2019, Arthur Matos, Marcus Vinicius Lamar, Universidade de Brasília,
//                Marek Materzok, University of Wrocław

`ifndef RVSIMPLE_EXAMPLE_DATA_MEMORY_H
`define RVSIMPLE_EXAMPLE_DATA_MEMORY_H

`include "config.sv"
`include "constants.sv"
`include "metron_tools.sv"

import rv_config::*;

module example_data_memory
(
  input logic clock,
  input logic[DATA_BITS - 2-1:0] address,
  input logic wren,
  input logic[3:0] byteena,
  input logic[31:0] data,
  output logic[31:0] q
);
 /*public:*/
  /*logic<32> q;*/

  initial begin : init
    string s;
    
    $value$plusargs("data_file=%s", s);
    $readmemh(s, mem);
  end

  always_ff @(posedge clock) begin : tick
    if (wren) begin
      logic[31:0] mask;
      mask = 0;
      if (byteena[0]) mask = mask | 32'h000000FF;
      if (byteena[1]) mask = mask | 32'h0000FF00;
      if (byteena[2]) mask = mask | 32'h00FF0000;
      if (byteena[3]) mask = mask | 32'hFF000000;
      mem[address] = (mem[address] & ~mask) | (data & mask);
    end
  end

  always_comb begin : tock q = mem[address]; end

 /*private:*/
  logic[31:0] mem[2**(DATA_BITS - 2)];
endmodule;

`endif  // RVSIMPLE_EXAMPLE_DATA_MEMORY_H
