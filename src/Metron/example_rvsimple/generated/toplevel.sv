// RISC-V SiMPLE SV -- Toplevel
// BSD 3-Clause License
// (c) 2017-2019, Arthur Matos, Marcus Vinicius Lamar, Universidade de Brasília,
//                Marek Materzok, University of Wrocław

`ifndef RVSIMPLE_TOPLEVEL_H
`define RVSIMPLE_TOPLEVEL_H

`include "config.sv"
`include "constants.sv"
`include "example_data_memory_bus.sv"
`include "example_text_memory_bus.sv"
`include "metron_tools.sv"
`include "riscv_core.sv"

module toplevel
(
  input logic clock,
  input logic reset,
  output logic[31:0] bus_read_data,
  output logic[31:0] bus_address,
  output logic[31:0] bus_write_data,
  output logic[3:0] bus_byte_enable,
  output logic bus_read_enable,
  output logic bus_write_enable,
  output logic[31:0] inst,
  output logic[31:0] pc
);
  // Submodule output port bindings
  logic[31:0] core_pc;
  logic[31:0] core_bus_address;
  logic[31:0] core_bus_write_data;
  logic core_bus_read_enable;
  logic core_bus_write_enable;
  logic[3:0] core_bus_byte_enable;
  logic[31:0] text_memory_bus_read_data;
  logic[31:0] data_memory_bus_read_data;

 /*public:*/
  /*logic<32> bus_read_data;*/
  /*logic<32> bus_address;*/
  /*logic<32> bus_write_data;*/
  /*logic<4> bus_byte_enable;*/
  /*logic<1> bus_read_enable;*/
  /*logic<1> bus_write_enable;*/

  /*logic<32> inst;*/
  /*logic<32> pc;*/

  //----------------------------------------

  initial begin : init
    /*core.init();*/
    /*text_memory_bus.init();*/
    /*data_memory_bus.init();*/
  end

  //----------------------------------------

  always_ff @(posedge clock) begin : tick
    /*core.tick(reset);*/
    /*data_memory_bus.tick(core.bus_address, core.bus_write_enable,
                         core.bus_byte_enable, core.bus_write_data);*/
  end
  //----------------------------------------

  always_comb begin : tock
    /*core.tock_pc();*/
    /*text_memory_bus.tock(core.pc);*/
    /*core.tock_execute(text_memory_bus.read_data);*/
    /*data_memory_bus.tock(core.bus_address, core.bus_read_enable);*/
    /*core.tock_writeback(data_memory_bus.read_data);*/

    bus_read_data = data_memory_bus_read_data;
    bus_address = core_bus_address;
    bus_write_data = core_bus_write_data;
    bus_byte_enable = core_bus_byte_enable;
    bus_read_enable = core_bus_read_enable;
    bus_write_enable = core_bus_write_enable;

    inst = text_memory_bus_read_data;
    pc = core_pc;
  end

  //----------------------------------------

 /*private:*/
  riscv_core core(
    // Inputs
    .clock(clock),
    .reset(reset), 
    .inst(text_memory_bus_read_data), 
    .bus_read_data(data_memory_bus_read_data), 
    // Outputs
    .pc(core_pc), 
    .bus_address(core_bus_address), 
    .bus_write_data(core_bus_write_data), 
    .bus_read_enable(core_bus_read_enable), 
    .bus_write_enable(core_bus_write_enable), 
    .bus_byte_enable(core_bus_byte_enable)
  );

  example_text_memory_bus text_memory_bus(
    // Inputs
    .clock(clock),
    .address(core_pc), 
    // Outputs
    .read_data(text_memory_bus_read_data)
  );

  example_data_memory_bus data_memory_bus(
    // Inputs
    .clock(clock),
    .address(core_bus_address), 
    .write_enable(core_bus_write_enable), 
    .byte_enable(core_bus_byte_enable), 
    .write_data(core_bus_write_data), 
    .read_enable(core_bus_read_enable), 
    // Outputs
    .read_data(data_memory_bus_read_data)
  );

endmodule;

`endif  // RVSIMPLE_TOPLEVEL_H
