#pragma once
#include "Sch_Top.h"
#include "Z80.h"

struct GateBoy {
public:

  GateBoy();

  void run(int phase_count) {
    for (int i = 0; i < phase_count; i++) phase();
  }

  void phase();
  uint64_t pass();
  void run_reset_sequence();

  uint8_t dbg_read (int addr);
  void    dbg_write(int addr, uint8_t data);

  void update_cpu_bus (int phase, Req req);
  void update_ext_bus (int phase);
  void update_vrm_bus (int phase);
  void update_oam_bus (int phase);
  void update_zram_bus(int phase);

  void test_reg(const char* regname, uint16_t addr, uint8_t data_in);
  void test_mem(uint16_t addr_start, uint16_t addr_end, uint16_t step, bool test_write);

  bool sys_rst = 1;
  bool sys_t1 = 0;
  bool sys_t2 = 0;
  bool sys_clken = 0;
  bool sys_clkgood = 0;
  bool sys_cpuready = 0;

  int phase_total = -19;
  int pass_count = 0;
  int pass_total = 0;

  uint64_t phase_hash = 0;
  uint64_t total_hash = 0;

  uint8_t mem[65536];
  uint8_t fb[160*144];

  Schematics::SchematicTop top;

  Z80 cpu;
  bool cpu_en = false;

  Req cpu_req;
  Req dbg_req;
};