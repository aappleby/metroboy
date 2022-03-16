// RISC-V SiMPLE SV -- ALU controller module
// BSD 3-Clause License
// (c) 2017-2019, Arthur Matos, Marcus Vinicius Lamar, Universidade de Brasília,
//                Marek Materzok, University of Wrocław

`include "config.sv"
`include "constants.sv"

module alu_control (
    input        [1:0] alu_op_type,
    input        [2:0] inst_funct3,
    input        [6:0] inst_funct7,
    output logic [4:0] alu_function
);

    logic [4:0] default_funct;
    logic [4:0] secondary_funct;
    logic [4:0] op_funct;
    logic [4:0] op_imm_funct;
    logic [4:0] branch_funct;
    `ifdef M_MODULE
    logic [4:0] m_extension_funct;
    `endif
    
    always_comb
        case (alu_op_type)
            `CTL_ALU_ADD:           alu_function = `ALU_ADD;
            `CTL_ALU_OP:            alu_function = op_funct;
            `CTL_ALU_OP_IMM:        alu_function = op_imm_funct;
            `CTL_ALU_BRANCH:        alu_function = branch_funct;
            default:                alu_function = 5'bx;
        endcase

    always_comb
        if (inst_funct7[5])     op_funct = secondary_funct;
    `ifdef M_MODULE
        else if(inst_funct7[0]) op_funct = m_extension_funct;
    `endif
        else                    op_funct = default_funct;

    always_comb
        if (inst_funct7[5] && inst_funct3[1:0] == 2'b01)
                op_imm_funct = secondary_funct;
        else    op_imm_funct = default_funct;

    always_comb
        case (inst_funct3)
            `FUNCT3_ALU_ADD_SUB:    default_funct = `ALU_ADD;
            `FUNCT3_ALU_SLL:        default_funct = `ALU_SLL;
            `FUNCT3_ALU_SLT:        default_funct = `ALU_SLT;
            `FUNCT3_ALU_SLTU:       default_funct = `ALU_SLTU;
            `FUNCT3_ALU_XOR:        default_funct = `ALU_XOR;
            `FUNCT3_ALU_SHIFTR:     default_funct = `ALU_SRL;
            `FUNCT3_ALU_OR:         default_funct = `ALU_OR;
            `FUNCT3_ALU_AND:        default_funct = `ALU_AND;
            default:                default_funct = 5'bx;
        endcase
    
    always_comb
        case (inst_funct3)
            `FUNCT3_ALU_ADD_SUB:    secondary_funct = `ALU_SUB;
            `FUNCT3_ALU_SHIFTR:     secondary_funct = `ALU_SRA;
            default:                secondary_funct = 5'bx;
        endcase
    
    always_comb
        case (inst_funct3)
            `FUNCT3_BRANCH_EQ:  branch_funct = `ALU_SEQ;
            `FUNCT3_BRANCH_NE:  branch_funct = `ALU_SEQ;
            `FUNCT3_BRANCH_LT:  branch_funct = `ALU_SLT;
            `FUNCT3_BRANCH_GE:  branch_funct = `ALU_SLT;
            `FUNCT3_BRANCH_LTU: branch_funct = `ALU_SLTU;
            `FUNCT3_BRANCH_GEU: branch_funct = `ALU_SLTU;
            default:            branch_funct = 5'bx;
        endcase
    
    `ifdef M_MODULE
        always_comb
            case (inst_funct3)
                `FUNCT3_ALU_MUL:    m_extension_funct = `ALU_MUL;
                `FUNCT3_ALU_MULH:   m_extension_funct = `ALU_MULH;
                `FUNCT3_ALU_MULHSU: m_extension_funct = `ALU_MULHSU;
                `FUNCT3_ALU_MULHU:  m_extension_funct = `ALU_MULHU;
                `FUNCT3_ALU_DIV:    m_extension_funct = `ALU_DIV;
                `FUNCT3_ALU_DIVU:   m_extension_funct = `ALU_DIVU;
                `FUNCT3_ALU_REM:    m_extension_funct = `ALU_REM;
                `FUNCT3_ALU_REMU:   m_extension_funct = `ALU_REMU;
                default:            m_extension_funct = 5'bx;
            endcase
    `endif

endmodule

