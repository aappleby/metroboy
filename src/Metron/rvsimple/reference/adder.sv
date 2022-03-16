// RISC-V SiMPLE SV -- adder module
// BSD 3-Clause License
// (c) 2017-2019, Arthur Matos, Marcus Vinicius Lamar, Universidade de Brasília,
//                Marek Materzok, University of Wrocław

`include "config.sv"
`include "constants.sv"

module adder #(
    parameter  WIDTH = 32
) (
    input  [WIDTH-1:0] operand_a,
    input  [WIDTH-1:0] operand_b,
    output [WIDTH-1:0] result
);

    assign result = operand_a + operand_b;

endmodule

