// RISC-V SiMPLE SV -- multiplexer module
// BSD 3-Clause License
// (c) 2017-2019, Arthur Matos, Marcus Vinicius Lamar, Universidade de Brasília,
//                Marek Materzok, University of Wrocław

`include "config.sv"
`include "constants.sv"

module multiplexer4 #(
    parameter  WIDTH = 32
) (
    input  [WIDTH-1:0] in0,
    input  [WIDTH-1:0] in1,
    input  [WIDTH-1:0] in2,
    input  [WIDTH-1:0] in3,
    input  [1:0]       sel,
    output [WIDTH-1:0] out
);

    multiplexer #(
        .WIDTH(WIDTH),
        .CHANNELS(4)
    ) multiplexer (
        .in_bus({in0, in1, in2, in3}),
        .sel(sel),
        .out(out)
    );

endmodule

