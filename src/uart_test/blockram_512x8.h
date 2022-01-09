#pragma once
#include "metron.h"

//=============================================================================
// tiny one-read-port + one-write-port memory to simulate block ram

struct blockram_512x8 {

  int     i_addr = 0;
  uint8_t o_data = 0;

  static const int size = 512;
  uint8_t memory[size];
  int     addr = 0;

  //----------------------------------------

  void initial(const uint8_t* src, int size) {
    assert(size == 512);
    memcpy(memory, src, size);
  }

  void reset() {
    addr = 0;
  }

  void tick() {
    o_data = memory[addr];
  }

  void tock() {
    addr = i_addr;
  }

  //----------------------------------------

  void dump_header() {
    printf("[ram addr o_data] ");
  }

  void dump() {
    printf("[    %-04x %-06x] ", addr, o_data);
  }

  void dump_vcd_header(VcdDump& d) {
    fprintf(d.file, "$scope module ram $end\n");
    fprintf(d.file, "$var wire 9  ram_i_addr i_addr $end\n");
    fprintf(d.file, "$var wire 8  ram_o_data o_data $end\n");
    fprintf(d.file, "$upscope $end\n");
  }

  void dump_value(VcdDump& d) {
    d.set_value("ram_i_addr", i_addr, 9);
    d.set_value("ram_o_data", o_data, 8);
  }
};

//=============================================================================
