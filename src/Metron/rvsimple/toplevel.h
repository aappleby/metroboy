// RISC-V SiMPLE SV -- Toplevel
// BSD 3-Clause License
// (c) 2017-2019, Arthur Matos, Marcus Vinicius Lamar, Universidade de Brasília,
//                Marek Materzok, University of Wrocław

#ifndef RVSIMPLE_TOPLEVEL_H
#define RVSIMPLE_TOPLEVEL_H

#include "metron_tools.h"
#include "config.h"
#include "constants.h"

#include "riscv_core.h"
#include "example_text_memory_bus.h"
#include "example_data_memory_bus.h"

class toplevel {
public:

  riscv_core riscv_core;
  example_text_memory_bus text_memory_bus;
  example_data_memory_bus data_memory_bus;

  logic<32> bus_read_data;
  logic<32> bus_address;
  logic<32> bus_write_data;
  logic<4>  bus_byte_enable;
  logic<1>  bus_read_enable;
  logic<1>  bus_write_enable;

  logic<32> inst;
  logic<32> pc;

  void tick(logic<1> reset) {
    riscv_core.tick(reset);
    data_memory_bus.tick(riscv_core.bus_address,
                          riscv_core.bus_write_enable,
                          riscv_core.bus_byte_enable,
                          riscv_core.bus_write_data);
  }

  void tock() {
    riscv_core.tock_pc();
    text_memory_bus.tock(riscv_core.pc);
    riscv_core.tock_execute(text_memory_bus.read_data);
    data_memory_bus.tock(riscv_core.bus_address, riscv_core.bus_read_enable);
    riscv_core.tock_writeback(data_memory_bus.read_data);

    bus_read_data = data_memory_bus.read_data;
    bus_address = riscv_core.bus_address;
    bus_write_data = riscv_core.bus_write_data;
    bus_byte_enable = riscv_core.bus_byte_enable;
    bus_read_enable = riscv_core.bus_read_enable;
    bus_write_enable = riscv_core.bus_write_enable;

    inst = text_memory_bus.read_data;
    pc = riscv_core.pc;
  }
};

#endif // RVSIMPLE_TOPLEVEL_H