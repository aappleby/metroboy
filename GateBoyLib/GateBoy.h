#pragma once
#include "GateBoyLib/Sch_Top.h"
#include "CoreLib/Z80.h"

//-----------------------------------------------------------------------------

#pragma pack(push, 1)
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

  void load_dump(const char* filename);
  void load_rom(const char* filename);

  void tock_ext_bus();
  void tock_oam_bus();
  void tock_vram_bus();
  void tock_zram_bus();

  //----------------------------------------

  int32_t  phase_total = 0;
  int32_t  pass_count = 0;
  int32_t  pass_total = 0;

  uint64_t phase_hash = HASH_INIT;
  uint64_t total_hash = HASH_INIT;

  int32_t  sys_rst = 1;
  int32_t  sys_t1 = 0;
  int32_t  sys_t2 = 0;
  int32_t  sys_clken = 0;
  int32_t  sys_clkgood = 0;
  int32_t  sys_cpuready = 0;
  int32_t  cpu_en = 0;

  Req* script = nullptr;
  int32_t  script_len = 0;
  
  Schematics::SchematicTop top;
  
  Z80 cpu;
  Req cpu_req = {0};
  Req dbg_req = {0};

  uint8_t mem[65536];
  uint8_t fb[160*144];
};
#pragma pack(pop)

//-----------------------------------------------------------------------------
