// RISC-V SiMPLE SV -- single-cycle controller
// BSD 3-Clause License
// (c) 2017-2019, Arthur Matos, Marcus Vinicius Lamar, Universidade de Brasília,
//                Marek Materzok, University of Wrocław

`include "config.sv"
`include "constants.sv"

module singlecycle_control (
    input  [6:0] inst_opcode,
    input  take_branch,
    output logic pc_write_enable,
    output logic regfile_write_enable,
    output logic alu_operand_a_select,
    output logic alu_operand_b_select,
    output logic [1:0] alu_op_type,
    output logic data_mem_read_enable,
    output logic data_mem_write_enable,
    output logic [2:0] reg_writeback_select,
    output logic [1:0] next_pc_select
);

    always_comb
        case (inst_opcode)
            `OPCODE_BRANCH: next_pc_select = take_branch ? `CTL_PC_PC_IMM : `CTL_PC_PC4;
            `OPCODE_JALR:   next_pc_select = `CTL_PC_RS1_IMM;
            `OPCODE_JAL:    next_pc_select = `CTL_PC_PC_IMM;
            default:        next_pc_select = `CTL_PC_PC4;
        endcase

    always_comb begin
        pc_write_enable         = 1'b1;
        regfile_write_enable    = 1'b0;
        alu_operand_a_select    = 1'bx;
        alu_operand_b_select    = 1'bx;
        alu_op_type             = 2'bx;
        data_mem_read_enable    = 1'b0;
        data_mem_write_enable   = 1'b0;
        reg_writeback_select    = 3'bx;
    
        case (inst_opcode)
            `OPCODE_LOAD:
            begin
                regfile_write_enable    = 1'b1;
                alu_operand_a_select    = `CTL_ALU_A_RS1;
                alu_operand_b_select    = `CTL_ALU_B_IMM;
                alu_op_type             = `CTL_ALU_ADD;
                data_mem_read_enable    = 1'b1;
                reg_writeback_select    = `CTL_WRITEBACK_DATA;
            end
    
            `OPCODE_MISC_MEM:
            begin
                // Fence - ignore
            end
    
            `OPCODE_OP_IMM:
            begin
                regfile_write_enable    = 1'b1;
                alu_operand_a_select    = `CTL_ALU_A_RS1;
                alu_operand_b_select    = `CTL_ALU_B_IMM;
                alu_op_type             = `CTL_ALU_OP_IMM;
                reg_writeback_select    = `CTL_WRITEBACK_ALU;
            end
    
            `OPCODE_AUIPC:
            begin
                regfile_write_enable    = 1'b1;
                alu_operand_a_select    = `CTL_ALU_A_PC;
                alu_operand_b_select    = `CTL_ALU_B_IMM;
                alu_op_type             = `CTL_ALU_ADD;
                reg_writeback_select    = `CTL_WRITEBACK_ALU;
            end
    
            `OPCODE_STORE:
            begin
                alu_operand_a_select    = `CTL_ALU_A_RS1;
                alu_operand_b_select    = `CTL_ALU_B_IMM;
                alu_op_type             = `CTL_ALU_ADD;
                data_mem_write_enable   = 1'b1;
            end
    
            `OPCODE_OP:
            begin
                regfile_write_enable    = 1'b1;
                alu_operand_a_select    = `CTL_ALU_A_RS1;
                alu_operand_b_select    = `CTL_ALU_B_RS2;
                reg_writeback_select    = `CTL_WRITEBACK_ALU;
                alu_op_type             = `CTL_ALU_OP;
            end
    
            `OPCODE_LUI:
            begin
                regfile_write_enable    = 1'b1;
                alu_operand_a_select    = `CTL_ALU_A_RS1;
                alu_operand_b_select    = `CTL_ALU_B_RS2;
                reg_writeback_select    = `CTL_WRITEBACK_IMM;
            end
    
            `OPCODE_BRANCH:
            begin
                alu_operand_a_select    = `CTL_ALU_A_RS1;
                alu_operand_b_select    = `CTL_ALU_B_RS2;
                alu_op_type             = `CTL_ALU_BRANCH;
            end
    
            `OPCODE_JALR:
            begin
                regfile_write_enable    = 1'b1;
                alu_operand_a_select    = `CTL_ALU_A_RS1;
                alu_operand_b_select    = `CTL_ALU_B_IMM;
                alu_op_type             = `CTL_ALU_ADD;
                reg_writeback_select    = `CTL_WRITEBACK_PC4;
            end
    
            `OPCODE_JAL:
            begin
                regfile_write_enable    = 1'b1;
                alu_operand_a_select    = `CTL_ALU_A_PC;
                alu_operand_b_select    = `CTL_ALU_B_IMM;
                alu_op_type             = `CTL_ALU_ADD;
                reg_writeback_select    = `CTL_WRITEBACK_PC4;
            end
    
            default:
            begin
                pc_write_enable         = 1'bx;
                regfile_write_enable    = 1'bx;
                data_mem_read_enable    = 1'bx;
                data_mem_write_enable   = 1'bx;
            end
        endcase
    end

endmodule

