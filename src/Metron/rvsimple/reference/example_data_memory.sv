// RISC-V SiMPLE SV -- data memory model
// BSD 3-Clause License
// (c) 2017-2019, Arthur Matos, Marcus Vinicius Lamar, Universidade de Brasília,
//                Marek Materzok, University of Wrocław

`include "config.sv"
`include "constants.sv"

module example_data_memory (
	input [`DATA_BITS-3:0] address,
	input [3:0] byteena,
	input clock,
	input [31:0] data,
	input wren,
	output [31:0] q
);

    (* nomem2reg *)
    logic [31:0] mem[0:2**(`DATA_BITS-2)-1];

    assign q = mem[address];

    always_ff @(posedge clock)
        if (wren) begin
            if (byteena[0]) mem[address][0+:8] <= data[0+:8];
            if (byteena[1]) mem[address][8+:8] <= data[8+:8];
            if (byteena[2]) mem[address][16+:8] <= data[16+:8];
            if (byteena[3]) mem[address][24+:8] <= data[24+:8];
        end

`ifdef DATA_HEX
    initial begin
        //$display("Loading %s", `DATA_HEX);
        $readmemh(`DATA_HEX, mem);
    end
`endif

endmodule

