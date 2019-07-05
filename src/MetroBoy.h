#pragma once
#include "Gameboy.h"

#include <vector>

//-----------------------------------------------------------------------------

class MetroBoy {
public:

  MetroBoy();
  MetroBoy(const MetroBoy&) = delete;
  MetroBoy& operator=(const MetroBoy&) = delete;

  Gameboy& gb() {
    return *current_gameboy;
  }

  int64_t total_tcycles() {
    return cycles;
  }

  const uint32_t* get_trace() const { return tracebuffer; }

  void load_rom(int model, const char* filename, bool run_bootrom);
  void load_dump();
  void save_dump();

  void reset(uint16_t new_pc);

  void run_fast(uint8_t buttons, int cycles);
  void run_vsync(uint8_t buttons);
  void run_to(uint16_t breakpoint);

  void step_frame();
  void step_line();
  void step_cycle();
  void step_over();

  void unstep_frame();
  void unstep_line();
  void unstep_cycle();

  void cycle();
  void mcycle();

  void clear_frame_history();
  void clear_line_history();
  void clear_cycle_history();

private:

  Gameboy* current_gameboy;
  std::vector<Gameboy*> history_frame;
  std::vector<Gameboy*> history_line;
  std::vector<Gameboy*> history_cycle;

  int64_t cycles;
  bool trace;

  uint32_t tracebuffer[456 * 154];
};

//-----------------------------------------------------------------------------
