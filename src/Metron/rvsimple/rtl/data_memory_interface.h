// RISC-V SiMPLE SV -- data memory interface
// BSD 3-Clause License
// (c) 2017-2019, Arthur Matos, Marcus Vinicius Lamar, Universidade de Brasília,
//                Marek Materzok, University of Wrocław

#ifndef RVSIMPLE_DATA_MEMORY_INTERFACE_H
#define RVSIMPLE_DATA_MEMORY_INTERFACE_H

#include "metron_tools.h"
#include "config.h"
#include "constants.h"

class data_memory_interface {
public:

  logic<32> read_data;
  logic<32> bus_address;
  logic<32> bus_write_data;
  logic<4>  bus_byte_enable;
  logic<1>  bus_read_enable;
  logic<1>  bus_write_enable;

  void init() {
  }

  void tick() {
  }

  void tock_bus(logic<1>  read_enable,
                logic<1>  write_enable,
                logic<3>  data_format,
                logic<32> address,
                logic<32> write_data)
  {
    bus_address      = address;
    bus_write_enable = write_enable;
    bus_read_enable  = read_enable;
    bus_write_data   = write_data << (8*b2(address));
    
    // calculate byte enable
    switch (b2(data_format)) {
        case 0b00: bus_byte_enable = b4(0b0001) << b2(address); break;
        case 0b01: bus_byte_enable = b4(0b0011) << b2(address); break;
        case 0b10: bus_byte_enable = b4(0b1111) << b2(address); break;
        default:   bus_byte_enable = b4(0b0000); break;
    }
  }

  void tock_read_data(logic<3>  data_format,
                      logic<32> address,
                      logic<32> bus_read_data)
  {
    // correct for unaligned accesses
    logic<32> position_fix = b32(bus_read_data >> (8*b2(address)));
    
    // sign-extend if necessary
    logic<32> sign_fix;
    switch (b2(data_format)) {
        case 0b00: sign_fix = cat(dup<24>(b1(~data_format[2] & position_fix[7])),  b8(position_fix)); break;
        case 0b01: sign_fix = cat(dup<16>(b1(~data_format[2] & position_fix[15])), b16(position_fix)); break;
        case 0b10: sign_fix = b32(position_fix); break;
        default:   sign_fix = b32(DONTCARE); break;
    }

    read_data = sign_fix;
  }
};

#endif // RVSIMPLE_DATA_MEMORY_INTERFACE_H
