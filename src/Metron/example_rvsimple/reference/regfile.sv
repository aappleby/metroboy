// RISC-V SiMPLE SV -- register file
// BSD 3-Clause License
// (c) 2017-2019, Arthur Matos, Marcus Vinicius Lamar, Universidade de Brasília,
//                Marek Materzok, University of Wrocław

`include "config.sv"
`include "constants.sv"

module regfile (
    input  clock,
    input  write_enable,
    input  [4:0] rd_address,
    input  [4:0] rs1_address,
    input  [4:0] rs2_address,
    input  [31:0] rd_data,
    output [31:0] rs1_data,
    output [31:0] rs2_data
);

    // 32 registers of 32-bit width
    logic [31:0] register [0:31];
   
    // Read ports for rs1 and rs2
    assign rs1_data = register[rs1_address];
    assign rs2_data = register[rs2_address];

    // Register x0 is always 0
    initial register[0] = 32'b0;

    // Write port for rd
    always_ff @(posedge clock)
        if (write_enable)
            if (rd_address != 5'b0) register[rd_address] <= rd_data;

endmodule

