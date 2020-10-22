#pragma once

#include "CoreLib/Assembler.h"
#include "CoreLib/Types.h"
#include "CoreLib/StateManager2.h"
#include "CoreLib/File.h"

#include "GateBoyLib/GateBoy.h"

#include <mutex>

struct GateBoyThread {

  void init();
  void reset();

  void sim_push();
  void sim_pop();

  void sim_update(int runmode, int stepmode, int step_forward, int step_backward);
  void sim_run(int phases);

  void toggle_cpu();
  void load_raw_dump();
  void save_raw_dump();
  void load_flat_dump(const char* filename);
  void load_rom(const char* filename);

  void dump1(Dumper& d);
  void dump2(Dumper& d);
  void dump3(Dumper& d);
  void dump4(Dumper& d);
  void dump5(Dumper& d);
  void dump6(Dumper& d);

  StateStack<GateBoy> gb;

  Assembler assembler;

  std::recursive_mutex sim_lock;
  typedef const std::lock_guard<decltype(sim_lock)> sim_guard;

  double sim_time = 0;
  double sim_time_smooth = 0;
  double sim_rate = 0;

  blob rom_buf;
};