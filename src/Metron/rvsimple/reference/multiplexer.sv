// RISC-V SiMPLE SV -- multiplexer module
// BSD 3-Clause License
// (c) 2017-2019, Arthur Matos, Marcus Vinicius Lamar, Universidade de Brasília,
//                Marek Materzok, University of Wrocław

`include "config.sv"
`include "constants.sv"

module multiplexer #(
    parameter  WIDTH    = 32,
    parameter  CHANNELS = 2) (
        input  [(CHANNELS * WIDTH) - 1:0]   in_bus,
        input  [$clog2(CHANNELS) - 1:0]     sel,
        output [WIDTH - 1:0]                out
);

    genvar ig;
    
    logic  [WIDTH - 1:0] input_array [0:CHANNELS - 1];
    
    assign out = input_array[sel];
    
    for(ig = 0; ig < CHANNELS; ig = ig + 1) begin: array_assignments
        assign input_array[(CHANNELS - 1) - ig] = in_bus[(ig * WIDTH) +: WIDTH];
    end

endmodule

