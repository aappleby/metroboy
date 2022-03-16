// RISC-V SiMPLE SV -- control transfer unit
// BSD 3-Clause License
// (c) 2017-2019, Arthur Matos, Marcus Vinicius Lamar, Universidade de Brasília,
//                Marek Materzok, University of Wrocław

`include "config.sv"
`include "constants.sv"

module control_transfer (
    input        result_equal_zero,
    input [2:0]  inst_funct3,
    output logic take_branch
);

    always_comb
        case (inst_funct3)
            `FUNCT3_BRANCH_EQ:  take_branch = !result_equal_zero;
            `FUNCT3_BRANCH_NE:  take_branch = result_equal_zero;
            `FUNCT3_BRANCH_LT:  take_branch = !result_equal_zero;
            `FUNCT3_BRANCH_GE:  take_branch = result_equal_zero;
            `FUNCT3_BRANCH_LTU: take_branch = !result_equal_zero;
            `FUNCT3_BRANCH_GEU: take_branch = result_equal_zero;
            default: take_branch = 1'bx;
        endcase

endmodule

