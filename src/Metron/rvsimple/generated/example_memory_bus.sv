// RISC-V SiMPLE SV -- combined text/data memory bus
// BSD 3-Clause License
// (c) 2017-2019, Arthur Matos, Marcus Vinicius Lamar, Universidade de Brasília,
//                Marek Materzok, University of Wrocław

`ifndef RVSIMPLE_EXAMPLE_MEMORY_BUS_H
`define RVSIMPLE_EXAMPLE_MEMORY_BUS_H

`include "config.sv"
`include "constants.sv"
`include "example_data_memory.sv"
`include "example_text_memory.sv"
`include "metron_tools.sv"

module example_memory_bus
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
  logic[31:0] text_memory_q;

 /*public:*/
  /*logic<32> read_data;*/

  always_ff @(posedge clock) begin : tick
    /*data_memory.tick(
        bx<DATA_BITS - 2>(address),
        write_enable && address >= DATA_BEGIN && address <= DATA_END,
        byte_enable, write_data);*/
  end

  always_comb begin : tock
    /*data_memory.tock(bx<DATA_BITS - 2>(address));*/
    /*text_memory.tock(bx<TEXT_BITS - 2>(address));*/

    if (read_enable) begin
      if (address >= TEXT_BEGIN && address <= TEXT_END)
        read_data = text_memory_q;
      else if (address >= DATA_BEGIN && address <= DATA_END)
        read_data = data_memory_q;
      else
        read_data = 32'x;
    end else begin
      read_data = 32'x;
    end
  end

 /*private:*/
  example_data_memory data_memory(
    // Inputs
    .clock(clock),
    .address((DATA_BITS - 2)'(address)), 
    .wren(write_enable && address >= DATA_BEGIN && address <= DATA_END), 
    .byteena(byte_enable), 
    .data(write_data), 
    // Outputs
    .q(data_memory_q)
  );

  example_text_memory text_memory(
    // Inputs
    .clock(clock),
    .address((TEXT_BITS - 2)'(address)), 
    // Outputs
    .q(text_memory_q)
  );

endmodule;

`endif  // RVSIMPLE_EXAMPLE_MEMORY_BUS_H
