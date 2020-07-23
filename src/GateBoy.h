#pragma once
#include "Sch_Top.h"

class GateBoy {
public:

  void run   (int phase_count, Req req, bool verbose, bool use_fast_impl);
  void phase (Req req, bool verbose, bool use_fast_impl);

  void run_reset_sequence(bool verbose, bool use_fast_impl);
  void run_benchmark(bool use_fast_impl);

  void fuzz_reset_sequence(bool use_fast_impl);

  int phase_total = -17;
  int pass_count = 0;
  int pass_total = 0;

  uint64_t phase_hash = 0;
  uint64_t total_hash = 0;

  uint8_t mem[65536];

  Schematics::SchematicTop top;
};