// RISC-V SiMPLE SV -- multiplexer module
// BSD 3-Clause License
// (c) 2017-2019, Arthur Matos, Marcus Vinicius Lamar, Universidade de Brasília,
//                Marek Materzok, University of Wrocław

`include "config.sv"
`include "constants.sv"

module multiplexer8 #(
    parameter  WIDTH = 32
) (
    input  [WIDTH-1:0] in0,
    input  [WIDTH-1:0] in1,
    input  [WIDTH-1:0] in2,
    input  [WIDTH-1:0] in3,
    input  [WIDTH-1:0] in4,
    input  [WIDTH-1:0] in5,
    input  [WIDTH-1:0] in6,
    input  [WIDTH-1:0] in7,
    input  [2:0]       sel,
    output [WIDTH-1:0] out
);

    multiplexer #(
        .WIDTH(WIDTH),
        .CHANNELS(8)
    ) multiplexer (
        .in_bus({in0, in1, in2, in3, in4, in5, in6, in7}),
        .sel(sel),
        .out(out)
    );

endmodule

