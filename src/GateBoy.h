#pragma once
#include "Sch_Top.h"
#include "Z80.h"

struct GateBoy {
public:

  GateBoy();

  void run(int phase_count) {
    for (int i = 0; i < phase_count; i++) next_phase();
  }

  void reset(bool verbose = false);
  void next_phase();
  uint64_t next_pass(int old_phase, int new_phase);
  void set_boot_bit();

  uint8_t dbg_read (int addr);
  void    dbg_write(int addr, uint8_t data);

  void NOP() { run(8); }
  void NOPS(int x) { run(8 * x); }

  void LDH_A8_A(uint8_t a8, uint8_t a) {
    NOP();
    NOP();
    dbg_write(0xFF00 | a8, a);
  }

  uint8_t LDH_A_A8(uint8_t a8) {
    NOP();
    NOP();
    uint8_t a = dbg_read(0xFF00 | a8);
    return a;
  }

  void load(const char* filename);

  void tock_ext_bus();
  void tock_oam_bus();
  void tock_vram_bus();
  void tock_zram_bus();

  bool sys_rst = 1;
  bool sys_t1 = 0;
  bool sys_t2 = 0;
  bool sys_clken = 0;
  bool sys_clkgood = 0;
  bool sys_cpuready = 0;

  int phase_total = 0;
  int pass_count = 0;
  int pass_total = 0;

  uint64_t phase_hash = HASH_INIT;
  uint64_t total_hash = HASH_INIT;

  uint8_t mem[65536];
  uint8_t fb[160*144];

  Schematics::SchematicTop top;

  Z80 cpu;
  bool cpu_en = false;

  uint8_t bus_data = 0;

  Req cpu_req;
  Req dbg_req;
};