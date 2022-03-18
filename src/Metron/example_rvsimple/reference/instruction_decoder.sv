// RISC-V SiMPLE SV -- instruction decoder
// BSD 3-Clause License
// (c) 2017-2019, Arthur Matos, Marcus Vinicius Lamar, Universidade de Brasília,
//                Marek Materzok, University of Wrocław

`include "config.sv"
`include "constants.sv"

module instruction_decoder(
    input [31:0] inst,
    output [6:0] inst_opcode,
    output [2:0] inst_funct3,
    output [6:0] inst_funct7,
    output [4:0] inst_rd,
    output [4:0] inst_rs1,
    output [4:0] inst_rs2
);

    assign inst_opcode = inst[6:0];
    assign inst_funct3 = inst[14:12];
    assign inst_funct7 = inst[31:25];
    assign inst_rd     = inst[11:7];
    assign inst_rs1    = inst[19:15];
    assign inst_rs2    = inst[24:20];

endmodule
