#pragma once
#include "StateManager.h"

//-----------------------------------------------------------------------------

struct MetroboyState {
  int frame = 0;
  int line = 0;
  int cycle = 0;

  Gameboy gb;
  uint8_t fb[160*144];
};

//-----------------------------------------------------------------------------

class MetroBoy : public StateManagerBase<MetroboyState> {
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

  uint8_t* get_vram() const { return current->gb.get_vram(); }

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
