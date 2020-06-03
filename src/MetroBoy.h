#pragma once
#include "StateManager.h"

//-----------------------------------------------------------------------------

struct MetroboyState {
  int frame = 0;
  int line = 0;
  int cycle = 0;

  Gameboy gb;
  uint8_t fb[160*144] = {0};
};

//-----------------------------------------------------------------------------

class MetroBoy : public StateManagerBase<MetroboyState> {
public:

  MetroBoy() = default;
  MetroBoy(const MetroBoy&) = delete;
  MetroBoy& operator=(const MetroBoy&) = delete;

  Gameboy& gb() {
    return current->gb;
  }

  uint8_t* fb() {
    return current->fb;
  }

  const uint32_t* get_trace() const { return tracebuffer; }

  uint8_t* get_vram() { return current->gb.get_vram(); }
  uint8_t* get_cram() { return current->gb.get_cram(); }
  uint8_t* get_iram() { return current->gb.get_iram(); }
  uint8_t* get_eram() { return current->gb.get_eram(); }

  void load_rom(const char* filename, bool run_bootrom);

  void reset(uint16_t new_pc);

  void run_fast(uint8_t buttons, int mcycles);
  void run_vsync(uint8_t buttons);
  void run_to(uint16_t breakpoint);

  void sync_to_vblank();

  void step_frame();
  void step_line();
  void step_phase();
  void step_over();

  Gameboy::HostOut gb_out;

private:

  blob rom;
  uint32_t tracebuffer[456 * 154] = {0};
  int tracecursor = 0;
};

//-----------------------------------------------------------------------------
