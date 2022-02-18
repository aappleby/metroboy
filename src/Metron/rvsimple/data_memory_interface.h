// RISC-V SiMPLE SV -- data memory interface
// BSD 3-Clause License
// (c) 2017-2019, Arthur Matos, Marcus Vinicius Lamar, Universidade de Brasília,
//                Marek Materzok, University of Wrocław

#pragma once
#include "../../../src/uart_test/metron_tools.h"
#include "../../synth/config.h"
#include "constants.h"

struct data_memory_interface {

  logic<32> o_read_data;
  logic<32> o_bus_address;
  logic<32> o_bus_write_data;
  logic<4>  o_bus_byte_enable;
  logic<1>  o_bus_read_enable;
  logic<1>  o_bus_write_enable;

  void tock_bus(logic<1>  i_read_enable,
                logic<1>  i_write_enable,
                logic<3>  i_data_format,
                logic<32> i_address,
                logic<32> i_write_data)
  {
    o_bus_address      = i_address;
    o_bus_write_enable = i_write_enable;
    o_bus_read_enable  = i_read_enable;
    o_bus_write_data   = i_write_data << (8*b2(i_address));
    
    // calculate byte enable
    o_bus_byte_enable = b4(0b0000);
    switch (b2(i_data_format)) {
        case 0b00: o_bus_byte_enable = b4(0b0001) << b2(i_address); break;
        case 0b01: o_bus_byte_enable = b4(0b0011) << b2(i_address); break;
        case 0b10: o_bus_byte_enable = b4(0b1111) << b2(i_address); break;
        default:   o_bus_byte_enable = b4(0b0000); break;
    }
  }

  void tock_read_data(logic<3>  i_data_format,
                      logic<32> i_address,
                      logic<32> i_bus_read_data)
  {
    // correct for unaligned accesses
    logic<32> position_fix = b32(i_bus_read_data >> (8*b2(i_address)));
    
    // sign-extend if necessary
    logic<32> sign_fix;
    switch (b2(i_data_format)) {
        case 0b00: sign_fix = cat(dup<24>(b1(~i_data_format[2] & position_fix[7])),  b8(position_fix)); break;
        case 0b01: sign_fix = cat(dup<16>(b1(~i_data_format[2] & position_fix[15])), b16(position_fix)); break;
        case 0b10: sign_fix = b32(position_fix); break;
        default:   sign_fix = b32(DONTCARE); break;
    }

    o_read_data = sign_fix;
  }
};
