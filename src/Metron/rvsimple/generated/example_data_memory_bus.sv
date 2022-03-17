// RISC-V SiMPLE SV -- data memory bus
// BSD 3-Clause License
// (c) 2017-2019, Arthur Matos, Marcus Vinicius Lamar, Universidade de Brasília,
//                Marek Materzok, University of Wrocław

`ifndef RVSIMPLE_EXAMPLE_DATA_MEMORY_BUS_H
`define RVSIMPLE_EXAMPLE_DATA_MEMORY_BUS_H

`include "config.sv"
`include "constants.sv"
`include "example_data_memory.sv"
`include "metron_tools.sv"

module example_data_memory_bus
(
  input logic clock,
  input logic[31:0] address,
  input logic write_enable,
  input logic[3:0] byte_enable,
  input logic[31:0] write_data,
  input logic read_enable,
  output logic[31:0] read_data
);
  // Submodule output port bindings
  logic[31:0] data_memory_q;

 /*public:*/
  /*logic<32> read_data;*/

  initial begin : init /*data_memory.init();*/ end

  always_ff @(posedge clock) begin : tick
    /*data_memory.tick(
        bx<DATA_BITS - 2>(address, 2),
        b1(write_enable && address >= DATA_BEGIN && address <= DATA_END),
        byte_enable, write_data);*/
  end

  always_comb begin : tock
    /*data_memory.tock(bx<DATA_BITS - 2>(address, 2));*/

    if (read_enable && address >= DATA_BEGIN && address <= DATA_END) begin
      read_data = data_memory_q;
    end else begin
      read_data = 32'x;
    end
  end

 /*private:*/
  example_data_memory data_memory(
    // Inputs
    .clock(clock),
    .address(address[DATA_BITS - 2+1:2]), 
    .wren(1'(write_enable && address >= DATA_BEGIN && address <= DATA_END)), 
    .byteena(byte_enable), 
    .data(write_data), 
    // Outputs
    .q(data_memory_q)
  );

endmodule;

`endif  // RVSIMPLE_EXAMPLE_DATA_MEMORY_BUS_H
