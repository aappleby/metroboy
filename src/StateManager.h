#pragma once
#include "Gameboy.h"

#include <vector>

//-----------------------------------------------------------------------------

struct MetroboyState {
  int frame = 0;
  int line = 0;
  int cycle = 0;

  Gameboy gb;
  uint8_t fb[160*144];
};

//-----------------------------------------------------------------------------

class StateManagerBase {
public:

  void load_dump();
  void save_dump();

  void push_frame();
  void push_line();
  void push_cycle();

  void pop_frame();
  void pop_line();
  void pop_cycle();

  void clear();
  void clear_frame_history();
  void clear_line_history();
  void clear_cycle_history();

protected:

  StateManagerBase();

  MetroboyState* current;
  std::vector<MetroboyState*> mb_frame;
  std::vector<MetroboyState*> mb_line;
  std::vector<MetroboyState*> mb_cycle;
};

//-----------------------------------------------------------------------------

class MetroBoy : public StateManagerBase {
public:

  MetroBoy();
  MetroBoy(const MetroBoy&) = delete;
  MetroBoy& operator=(const MetroBoy&) = delete;

  Gameboy& gb() {
    return current->gb;
  }

  uint8_t* fb() {
    return current->fb;
  }

  int64_t total_tcycles() {
    return cycles;
  }

  const uint32_t* get_trace() const { return tracebuffer; }

  void load_rom(const char* filename, bool run_bootrom);

  void reset(uint16_t new_pc);

  void run_fast(uint8_t buttons, int cycles);
  void run_vsync(uint8_t buttons);
  void run_to(uint16_t breakpoint);

  void step_frame();
  void step_line();
  void step_cycle();
  void step_over();

  void cycle();
  void mcycle();

  Gameboy::HostOut gb_out;

private:

  int64_t cycles;
  bool trace;

  uint32_t tracebuffer[456 * 154];
};

//-----------------------------------------------------------------------------
