// RISC-V SiMPLE SV -- register file
// BSD 3-Clause License
// (c) 2017-2019, Arthur Matos, Marcus Vinicius Lamar, Universidade de Brasília,
//                Marek Materzok, University of Wrocław

`ifndef RVSIMPLE_REGFILE_H
`define RVSIMPLE_REGFILE_H

`include "config.sv"
`include "constants.sv"
`include "metron_tools.sv"

module regfile
(
  input logic clock,
  input logic write_enable,
  input logic[4:0] rd_address,
  input logic[31:0] rd_data,
  input logic[4:0] rs1_address,
  input logic[4:0] rs2_address,
  output logic[31:0] rs1_data,
  output logic[31:0] rs2_data
);
 /*public:*/
  /*logic<32> rs1_data;*/
  /*logic<32> rs2_data;*/

  // Register x0 is always 0
  initial begin : init regs[0] = 32'd0; end

  // Write port for rd
  always_ff @(posedge clock) begin : tick
    if (write_enable && rd_address != 5'd0) begin
      regs[rd_address] = rd_data;
    end
  end

  always_comb begin : tock
    // Read ports for rs1 and rs2
    rs1_data = regs[rs1_address];
    rs2_data = regs[rs2_address];
  end

 /*private:*/
  // 32 registers of 32-bit width
  logic[31:0] regs[32];
endmodule;

`endif  // RVSIMPLE_REGFILE_H
