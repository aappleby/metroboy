// RISC-V SiMPLE SV -- data memory interface
// BSD 3-Clause License
// (c) 2017-2019, Arthur Matos, Marcus Vinicius Lamar, Universidade de Brasília,
//                Marek Materzok, University of Wrocław

`include "config.sv"
`include "constants.sv"

module data_memory_interface (
    input  clock,
    input  read_enable,
    input  write_enable,
    input  [2:0]  data_format,
    input  [31:0] address,
    input  [31:0] write_data,
    output [31:0] read_data,

    output       [31:0] bus_address,
    input        [31:0] bus_read_data,
    output       [31:0] bus_write_data,
    output logic [3:0]  bus_byte_enable,
    output              bus_read_enable,
    output              bus_write_enable
);

    logic [31:0] position_fix;
    logic [31:0] sign_fix;

    assign bus_address      = address;
    assign bus_write_enable = write_enable;
    assign bus_read_enable  = read_enable;
    assign bus_write_data   = write_data << (8*address[1:0]);
    
    // calculate byte enable
    always_comb begin
       bus_byte_enable = 4'b0000;
       case (data_format[1:0])
           2'b00:   bus_byte_enable = 4'b0001 << address[1:0];
           2'b01:   bus_byte_enable = 4'b0011 << address[1:0];
           2'b10:   bus_byte_enable = 4'b1111 << address[1:0];
           default: bus_byte_enable = 4'b0000;
       endcase
    end
    
    // correct for unaligned accesses
    always_comb begin
       position_fix = bus_read_data >> (8*address[1:0]);
    end
    
    // sign-extend if necessary
    always_comb begin
       case (data_format[1:0])
           2'b00:   sign_fix = {{24{~data_format[2] & position_fix[7]}}, position_fix[7:0]};
           2'b01:   sign_fix = {{16{~data_format[2] & position_fix[15]}}, position_fix[15:0]};
           2'b10:   sign_fix = position_fix[31:0];
           default: sign_fix = 32'bx;
       endcase
    end

    assign read_data = sign_fix;
    
endmodule

