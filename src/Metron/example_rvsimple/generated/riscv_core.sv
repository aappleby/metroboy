// RISC-V SiMPLE SV -- Single-cycle RISC-V core
// BSD 3-Clause License
// (c) 2017-2019, Arthur Matos, Marcus Vinicius Lamar, Universidade de Brasília,
//                Marek Materzok, University of Wrocław

`ifndef RVSIMPLE_RISCV_CORE_H
`define RVSIMPLE_RISCV_CORE_H

`include "config.sv"
`include "constants.sv"
`include "data_memory_interface.sv"
`include "metron_tools.sv"
`include "singlecycle_ctlpath.sv"
`include "singlecycle_datapath.sv"

module riscv_core
(
  input logic clock,
  input logic reset,
  input logic[31:0] inst,
  input logic[31:0] bus_read_data,
  output logic[31:0] pc,
  output logic[31:0] bus_address,
  output logic[31:0] bus_write_data,
  output logic bus_read_enable,
  output logic bus_write_enable,
  output logic[3:0] bus_byte_enable
);
  // Submodule output port bindings
  logic[31:0] datapath_pc;
  logic[6:0] datapath_inst_funct7;
  logic[2:0] datapath_inst_funct3;
  logic[6:0] datapath_inst_opcode;
  logic[31:0] datapath_data_mem_write_data;
  logic[31:0] datapath_data_mem_address;
  logic datapath_alu_result_equal_zero2;
  logic ctlpath_pc_write_enable;
  logic ctlpath_regfile_write_enable;
  logic ctlpath_alu_operand_a_select;
  logic ctlpath_alu_operand_b_select;
  logic ctlpath_data_mem_read_enable;
  logic ctlpath_data_mem_write_enable;
  logic[2:0] ctlpath_reg_writeback_select;
  logic[1:0] ctlpath_next_pc_select;
  logic[4:0] ctlpath_alu_function;
  logic[31:0] dmem_bus_address;
  logic dmem_bus_write_enable;
  logic dmem_bus_read_enable;
  logic[31:0] dmem_bus_write_data;
  logic[3:0] dmem_bus_byte_enable;
  logic[31:0] dmem_read_data;

 /*public:*/
  /*logic<32> pc;*/
  /*logic<32> bus_address;*/
  /*logic<32> bus_write_data;*/
  /*logic<4> bus_byte_enable;*/
  /*logic<1> bus_read_enable;*/
  /*logic<1> bus_write_enable;*/

  //----------------------------------------

  initial begin : init
    /*datapath.init();*/
  end

  //----------------------------------------

  always_ff @(posedge clock) begin : tick
    /*datapath.tick(reset, ctlpath.pc_write_enable, ctlpath.regfile_write_enable);*/
  end

  //----------------------------------------

  always_comb begin : tock_pc
    /*datapath.tock_pc();*/
    pc = datapath_pc;
  end

  always_comb begin : tock_execute
    /*datapath.tock_decode(inst);*/

    /*datapath.tock_regfile();*/

    /*ctlpath.tock_decode(datapath.inst_opcode);*/

    /*ctlpath.tock_alu_control(datapath.inst_funct3, datapath.inst_funct7);*/

    /*datapath.tock_alu(ctlpath.alu_function, ctlpath.alu_operand_a_select,
                      ctlpath.alu_operand_b_select);*/

    /*ctlpath.tock_next_pc_select(datapath.inst_opcode, datapath.inst_funct3,
                                datapath.alu_result_equal_zero2);*/

    /*datapath.tock_next_pc(ctlpath.next_pc_select);*/

    /*dmem.tock_bus(ctlpath.data_mem_read_enable, ctlpath.data_mem_write_enable,
                  datapath.inst_funct3, datapath.data_mem_address,
                  datapath.data_mem_write_data);*/

    bus_address = dmem_bus_address;
    bus_write_data = dmem_bus_write_data;
    bus_read_enable = dmem_bus_read_enable;
    bus_write_enable = dmem_bus_write_enable;
    bus_byte_enable = dmem_bus_byte_enable;
  end

  always_comb begin : tock_writeback
    /*dmem.tock_read_data(datapath.inst_funct3, datapath.data_mem_address,
                        bus_read_data);*/

    /*datapath.tock_writeback(dmem.read_data, ctlpath.reg_writeback_select);*/
  end

  //----------------------------------------

 /*private:*/
  singlecycle_datapath datapath(
    // Inputs
    .clock(clock),
    .reset(reset), 
    .pc_write_enable(ctlpath_pc_write_enable), 
    .regfile_write_enable(ctlpath_regfile_write_enable), 
    .inst(inst), 
    .alu_function(ctlpath_alu_function), 
    .alu_operand_a_select(ctlpath_alu_operand_a_select), 
    .alu_operand_b_select(ctlpath_alu_operand_b_select), 
    .next_pc_select(ctlpath_next_pc_select), 
    .data_mem_read_data(dmem_read_data), 
    .reg_writeback_select(ctlpath_reg_writeback_select), 
    // Outputs
    .pc(datapath_pc), 
    .inst_funct7(datapath_inst_funct7), 
    .inst_funct3(datapath_inst_funct3), 
    .inst_opcode(datapath_inst_opcode), 
    .data_mem_write_data(datapath_data_mem_write_data), 
    .data_mem_address(datapath_data_mem_address), 
    .alu_result_equal_zero2(datapath_alu_result_equal_zero2)
  );

  singlecycle_ctlpath ctlpath(
    // Inputs
    .clock(clock),
    .inst_opcode(datapath_inst_opcode), 
    .inst_funct3(datapath_inst_funct3), 
    .alu_result_equal_zero(datapath_alu_result_equal_zero2), 
    .inst_funct7(datapath_inst_funct7), 
    // Outputs
    .pc_write_enable(ctlpath_pc_write_enable), 
    .regfile_write_enable(ctlpath_regfile_write_enable), 
    .alu_operand_a_select(ctlpath_alu_operand_a_select), 
    .alu_operand_b_select(ctlpath_alu_operand_b_select), 
    .data_mem_read_enable(ctlpath_data_mem_read_enable), 
    .data_mem_write_enable(ctlpath_data_mem_write_enable), 
    .reg_writeback_select(ctlpath_reg_writeback_select), 
    .next_pc_select(ctlpath_next_pc_select), 
    .alu_function(ctlpath_alu_function)
  );

  data_memory_interface dmem(
    // Inputs
    .clock(clock),
    .read_enable(ctlpath_data_mem_read_enable), 
    .write_enable(ctlpath_data_mem_write_enable), 
    .data_format(datapath_inst_funct3), 
    .address(datapath_data_mem_address), 
    .write_data(datapath_data_mem_write_data), 
    .bus_read_data(bus_read_data), 
    // Outputs
    .bus_address(dmem_bus_address), 
    .bus_write_enable(dmem_bus_write_enable), 
    .bus_read_enable(dmem_bus_read_enable), 
    .bus_write_data(dmem_bus_write_data), 
    .bus_byte_enable(dmem_bus_byte_enable), 
    .read_data(dmem_read_data)
  );

endmodule;

`endif  // RVSIMPLE_RISCV_CORE_H
