#pragma once
#include "Sch_Top.h"

struct GateBoy {
public:

  GateBoy();

  void run   (int phase_count, Req req, bool verbose, bool use_fast_impl);
  void phase (Req req, bool verbose, bool use_fast_impl);
  void run_reset_sequence(bool verbose, bool use_fast_impl);

  uint8_t dbg_read(uint16_t addr, bool use_fast_impl);
  void    dbg_write(uint16_t addr, uint8_t data, bool use_fast_impl);

  void update_cpu_bus(int phase, Req req);
  void update_ext_bus(int phase);
  void update_vrm_bus(int phase);
  void update_oam_bus(int phase);

  int phase_total = -17;
  int pass_count = 0;
  int pass_total = 0;

  uint64_t phase_hash = 0;
  uint64_t total_hash = 0;

  uint8_t mem2[65536];

  Schematics::SchematicTop top;
};