// RISC-V SiMPLE SV -- data memory bus
// BSD 3-Clause License
// (c) 2017-2019, Arthur Matos, Marcus Vinicius Lamar, Universidade de Brasília,
//                Marek Materzok, University of Wrocław

`include "config.sv"
`include "constants.sv"

module example_data_memory_bus (
    input  clock,
    input  [31:0] address,
    output [31:0] read_data,
    input  [31:0] write_data,
    input   [3:0] byte_enable,
    input         read_enable,
    input         write_enable
);

    logic [31:0] fetched;
    logic is_data_memory;
    
    assign is_data_memory = address >= `DATA_BEGIN && address <= `DATA_END;
    
    example_data_memory data_memory(
        .clock      (clock),
        .address    (address[`DATA_BITS-1:2]),
        .byteena    (byte_enable),
        .data       (write_data),
        .wren       (write_enable && is_data_memory),
        .q          (fetched)
    );
   
    assign read_data = 
        read_enable && is_data_memory
        ? fetched
        : 32'hxxxxxxxx;

endmodule

