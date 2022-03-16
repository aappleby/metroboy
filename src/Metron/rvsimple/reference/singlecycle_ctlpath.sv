// RISC-V SiMPLE SV -- control path
// BSD 3-Clause License
// (c) 2017-2019, Arthur Matos, Marcus Vinicius Lamar, Universidade de Brasília,
//                Marek Materzok, University of Wrocław

`include "config.sv"
`include "constants.sv"

module singlecycle_ctlpath (
    input  [6:0] inst_opcode,
    input  [2:0] inst_funct3,
    input  [6:0] inst_funct7,
    input  alu_result_equal_zero,

    output pc_write_enable,
    output regfile_write_enable,
    output alu_operand_a_select,
    output alu_operand_b_select,
    output data_mem_read_enable,
    output data_mem_write_enable,
    output [2:0] reg_writeback_select,
    output [4:0] alu_function,
    output [1:0] next_pc_select
);

    logic take_branch;
    logic [1:0] alu_op_type;

    singlecycle_control singlecycle_control(
        .inst_opcode            (inst_opcode),
        .pc_write_enable        (pc_write_enable),
        .regfile_write_enable   (regfile_write_enable),
        .alu_operand_a_select   (alu_operand_a_select),
        .alu_operand_b_select   (alu_operand_b_select),
        .alu_op_type            (alu_op_type),
        .data_mem_read_enable   (data_mem_read_enable),
        .data_mem_write_enable  (data_mem_write_enable),
        .reg_writeback_select   (reg_writeback_select),
        .take_branch            (take_branch),
        .next_pc_select         (next_pc_select)
    );

    control_transfer control_transfer (
        .result_equal_zero  (alu_result_equal_zero),
        .inst_funct3        (inst_funct3),
        .take_branch        (take_branch)
    );

    alu_control alu_control(
        .alu_op_type        (alu_op_type),
        .inst_funct3        (inst_funct3),
        .inst_funct7        (inst_funct7),
        .alu_function       (alu_function)
    );

endmodule

