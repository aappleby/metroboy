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

  riscv_core core;
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

  void init() {
    core.init();
    text_memory_bus.init();
    data_memory_bus.init();
  }

  void tick(logic<1> reset) {
    core.tick(reset);
    data_memory_bus.tick(core.bus_address,
                          core.bus_write_enable,
                          core.bus_byte_enable,
                          core.bus_write_data);
  }

  void tock() {
    core.tock_pc();
    text_memory_bus.tock(core.pc);
    core.tock_execute(text_memory_bus.read_data);
    data_memory_bus.tock(core.bus_address, core.bus_read_enable);
    core.tock_writeback(data_memory_bus.read_data);

    bus_read_data = data_memory_bus.read_data;
    bus_address = core.bus_address;
    bus_write_data = core.bus_write_data;
    bus_byte_enable = core.bus_byte_enable;
    bus_read_enable = core.bus_read_enable;
    bus_write_enable = core.bus_write_enable;

    inst = text_memory_bus.read_data;
    pc = core.pc;
  }
};

#endif // RVSIMPLE_TOPLEVEL_H