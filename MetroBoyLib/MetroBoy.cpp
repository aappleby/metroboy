#include "MetroBoyLib/MetroBoy.h"
#include "AppLib/Audio.h"
#include <assert.h>

extern const int op_sizes[];

//=============================================================================

void MetroBoy::load_rom(const char* filename, bool run_bootrom) {
  load_array(filename, rom);
  current->gb.set_rom(rom.data(), rom.size());
  current->gb.reset(run_bootrom ? 0x0000 : 0x0100);
}

//-----------------------------------------------------------------------------

void MetroBoy::reset(uint16_t new_pc) {
  clear();
  memset(tracebuffer, 0, sizeof(tracebuffer));
  current->gb.set_rom(rom.data(), rom.size());
  current->gb.reset(new_pc);
}

//-----------------------------------------------------------------------------

void MetroBoy::run_fast(uint8_t buttons, int mcycles) {
  auto& gb = current->gb;

  clear();
  gb.set_joypad(~buttons);

  gb.sync_to_mcycle();
  for (int cycle = 0; cycle < mcycles; cycle++) {
    gb.mcycle();
  }
}

//-------------------------------------

void MetroBoy::run_vsync(uint8_t buttons) {
  auto& gb = current->gb;

  clear();
  gb.set_joypad(~buttons);
  gb.sync_to_mcycle();

  while(gb.get_host_data().y != 144) {
    gb.mcycle();
  }

  audio_begin();

  for (int i = 0; i < 154 * 114; i++) {
    gb.mcycle();
    audio_post(current->gb.get_host_data().out_l,
               current->gb.get_host_data().out_r);
  }

  audio_end();
}

//-------------------------------------

void MetroBoy::run_to(uint16_t breakpoint) {
  auto& gb = current->gb;

  clear();
  gb.sync_to_mcycle();

  while (1) {
    uint16_t op_addr = current->gb.get_cpu().get_op_addr();
    if (op_addr == breakpoint) break;
    gb.mcycle();
  }
}

//-----------------------------------------------------------------------------

void MetroBoy::step_phase(int count) {
  push_cycle();

  auto& gb = current->gb;
  for (int i = 0; i < count; i++) {
    gb.halfcycle();
  }
}

void MetroBoy::unstep_phase() {
  pop_cycle();
}

//----------------------------------------

void MetroBoy::step_line(int count) {
  push_line();

  auto& gb = current->gb;
  gb.sync_to_mcycle();

  for (int i = 0; i < count; i++) {
    int old_line = gb.get_host_data().y;

    while(1) {
      gb.mcycle();
      int new_line = gb.get_host_data().y;
      if (old_line != new_line) {
        break;
      }
    }
  }
}

void MetroBoy::unstep_line() {
  pop_line();
}

//----------------------------------------

void MetroBoy::step_frame(int count) {
  push_frame();

  auto& gb = current->gb;
  gb.sync_to_mcycle();

  for (int i = 0; i < count; i++) {
    do {
      gb.mcycle();
    } while (gb.get_host_data().y == 144);

    do {
      gb.mcycle();
    } while (gb.get_host_data().y != 144);
  }
}

void MetroBoy::unstep_frame() {
  pop_frame();
}

//----------------------------------------

/*
void MetroBoy::step_over() {
  push_cycle();

  auto& gb = current->gb;

  int op_addr = gb.get_cpu().get_op_addr();
  int op = get_rom()[op_addr];
  int op_size = op_sizes[op];
  if (op == 0xcb) op_size = 2;

  int next_op_addr = op_addr + op_size;

  gb.sync_to_mcycle();
  int i = 0;
  for (; i < 1000000; i++) {
    if (gb.get_cpu().get_op_addr() == next_op_addr) {
      // step succeeded
      return;
    }
    gb.mcycle();
  }

  // step failed
  pop_cycle();
  return;
}
*/

//-----------------------------------------------------------------------------
