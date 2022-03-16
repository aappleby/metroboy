// RISC-V SiMPLE SV -- program text memory bus
// BSD 3-Clause License
// (c) 2017-2019, Arthur Matos, Marcus Vinicius Lamar, Universidade de Brasília,
//                Marek Materzok, University of Wrocław

`include "config.sv"
`include "constants.sv"

module example_text_memory_bus (
    input  clock,
    input  [31:0] address,
    output [31:0] read_data
);

    logic [31:0] fetched;
    
    example_text_memory text_memory(
        .address    (address[`TEXT_BITS-1:2]),
        .clock      (clock),
        .q          (fetched)
    );
   
    assign read_data = 
        address >= `TEXT_BEGIN && address <= `TEXT_END
        ? fetched
        : 32'hxxxxxxxx;

endmodule

