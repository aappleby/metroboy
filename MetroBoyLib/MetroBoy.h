#pragma once
#include "CoreLib/StateManager.h"
#include "CoreLib/File.h"
#include "MetroBoyLib/Gameboy.h"

//-----------------------------------------------------------------------------

struct MetroboyState {
  int frame = 0;
  int line = 0;
  int cycle = 0;

  Gameboy gb;
};

//-----------------------------------------------------------------------------

class MetroBoy : private StateManagerBase<MetroboyState> {
public:

  MetroBoy() = default;
  MetroBoy(const MetroBoy&) = delete;
  MetroBoy& operator=(const MetroBoy&) = delete;

  void load_dump() {
    load_obj("dump.MetroBoy", *current);
  }

  void save_dump() {
    save_obj("dump.MetroBoy", *current);
  }

  Gameboy& gb() {
    return current->gb;
  }

  uint8_t* fb() {
    return current->gb.fb;
  }

  const uint32_t* get_trace() const { return tracebuffer; }

  uint8_t* get_cart_rom() { return current->gb.get_cart_rom(); }
  uint8_t* get_vram()     { return current->gb.get_vram(); }
  uint8_t* get_cart_ram() { return current->gb.get_cart_ram(); }
  uint8_t* get_main_ram() { return current->gb.get_main_ram(); }

  void load_rom(const char* filename, bool run_bootrom);

  void reset(uint16_t new_pc);

  void run_fast(uint8_t buttons, int mcycles);
  void run_vsync(uint8_t buttons);
  void run_to(uint16_t breakpoint);

  void sync_to_vblank();

  void step_phase(int count);
  void unstep_phase();

  void step_line(int count);
  void unstep_line();

  void step_frame(int count);
  void unstep_frame();

  //void step_over();

private:

  blob rom;
  uint32_t tracebuffer[456 * 154] = {0};
  int tracecursor = 0;
};

//-----------------------------------------------------------------------------
