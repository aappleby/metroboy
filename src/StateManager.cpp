#include "StateManager.h"

#include "Audio.h"

#include <assert.h>

extern uint8_t rom_buf[];
extern const int op_sizes[];

#pragma warning(disable : 4996)

//=============================================================================

StateManagerBase::StateManagerBase() {
  current = new MetroboyState();
}

//-----------------------------------------------------------------------------

void StateManagerBase::load_dump() {
  FILE* dump_file = fopen("dump.MetroBoy", "rb");
  size_t size = fread(current, 1, sizeof(MetroboyState), dump_file);
  assert(size == sizeof(Gameboy));
  size = fread(rom_buf, 1, 1024 * 1024, dump_file);
  assert(size == 1024 * 1024);
  fclose(dump_file);
}

void StateManagerBase::save_dump() {
  FILE* dump_file = fopen("dump.MetroBoy", "wb");
  fwrite(current, 1, sizeof(MetroboyState), dump_file);
  fwrite(rom_buf, 1, 1024 * 1024, dump_file);
  fclose(dump_file);
}

//-----------------------------------------------------------------------------

void StateManagerBase::push_frame() {
  clear_line_history();
  clear_cycle_history();

  mb_frame.push_back(current);
  current = new MetroboyState();
  *current = *mb_frame.back();
}

void StateManagerBase::push_line() {
  clear_cycle_history();

  mb_line.push_back(current);
  current = new MetroboyState();
  *current = *mb_line.back();
}

void StateManagerBase::push_cycle() {
  mb_cycle.push_back(current);
  current = new MetroboyState();
  *current = *mb_cycle.back();
}

//-----------------------------------------------------------------------------

void StateManagerBase::pop_frame() {
  if (mb_frame.empty()) return;

  delete current;
  current = mb_frame.back();
  mb_frame.pop_back();
}

void StateManagerBase::pop_line() {
  if (mb_line.empty()) return;

  delete current;
  current = mb_line.back();
  mb_line.pop_back();
}

void StateManagerBase::pop_cycle() {
  if (mb_cycle.empty()) return;

  delete current;
  current = mb_cycle.back();
  mb_cycle.pop_back();
}

//-----------------------------------------------------------------------------

void StateManagerBase::clear() {
  clear_frame_history();
  clear_line_history();
  clear_cycle_history();
}

void StateManagerBase::clear_frame_history() {
  for (auto m : mb_frame) delete m;
  mb_frame.clear();
}

void StateManagerBase::clear_line_history() {
  for (auto m : mb_line) delete m;
  mb_line.clear();
}

void StateManagerBase::clear_cycle_history() {
  for (auto m : mb_cycle) delete m;
  mb_cycle.clear();
}
 
//=============================================================================

MetroBoy::MetroBoy()
{
  gb_out = {};
  cycles = 0;
  trace = true;
  memset(tracebuffer, 0, sizeof(tracebuffer));
}

//-----------------------------------------------------------------------------

void MetroBoy::load_rom(const char* filename, bool run_bootrom) {
  FILE* rom_file = fopen(filename, "rb");
  fseek(rom_file, 0, SEEK_END);
  size_t rom_size = ftell(rom_file);
  fseek(rom_file, 0, SEEK_SET);
  rom_size = fread(rom_buf, 1, rom_size, rom_file);
  fclose(rom_file);

  current->gb.reset(rom_size, run_bootrom ? 0x0000 : 0x0100);
}

//-----------------------------------------------------------------------------

void MetroBoy::reset(uint16_t new_pc) {
  clear();
  memset(tracebuffer, 0, sizeof(tracebuffer));
  current->gb.reset(new_pc);
}

//-----------------------------------------------------------------------------

void MetroBoy::run_fast(uint8_t buttons, int fast_cycles) {
  clear();

  current->gb.set_buttons(~buttons);

  for (int cycle = 0; cycle < fast_cycles; cycle++) mcycle();
}

//-------------------------------------

void MetroBoy::run_vsync(uint8_t buttons) {
  clear();

  current->gb.set_buttons(~buttons);

  while(gb_out.y != 144) {
    cycle();
  }

  audio_begin();

  for (int i = 0; i < 154 * 114; i++) {
    mcycle();
    audio_post(current->gb.get_host_data().out_l,
               current->gb.get_host_data().out_r);
  }

  audio_end();
}

//-------------------------------------

void MetroBoy::run_to(uint16_t breakpoint) {
  clear();

  while (1) {
    uint16_t op_addr = current->gb.get_cpu().get_op_addr();
    if (op_addr == breakpoint) break;
    cycle();
  }
}

//-------------------------------------

void MetroBoy::step_frame() {
  push_frame();
  
  do {
    mcycle();
  } while (gb_out.y == 144);

  do {
    mcycle();
  } while (gb_out.y != 144);
}

void MetroBoy::step_line() {
  push_line();

  int line = gb_out.y;
  do {
    mcycle();
  } while (gb_out.y == line);
}

void MetroBoy::step_cycle() {
  push_cycle();
  cycle();
}

void MetroBoy::step_over() {
  push_cycle();

  int op_addr = current->gb.get_cpu().get_op_addr();
  int op = rom_buf[op_addr];
  int op_size = op_sizes[op];
  if (op == 0xcb) op_size = 2;

  int next_op_addr = op_addr + op_size;

  int i = 0;
  for (; i < 1000000; i++) {
    if (current->gb.get_cpu().get_op_addr() == next_op_addr) {
      // step succeeded
      return;
    }
    cycle();
  }

  // step failed
  pop_cycle();
  return;
}

//-----------------------------------------------------------------------------

void MetroBoy::cycle() {
  current->gb.tick();
  current->gb.tock();

  gb_out = current->gb.get_host_data();

  if (trace) {
    tracebuffer[gb_out.y * 456 + gb_out.counter] = gb_out.trace;
  }

  if (gb_out.pix_oe) {
    int x = gb_out.x - 1;
    int y = gb_out.y;

    if (x >= 0 && x < 160 && y >= 0 && y < 144) {
      current->fb[x + y * 160] = gb_out.pix;
    }
  }

  current->gb.check_sentinel();
  cycles++;
}

void MetroBoy::mcycle() {
  cycle();
  if ((current->gb.get_tcycle() & 3) == 0) return;
  cycle();
  if ((current->gb.get_tcycle() & 3) == 0) return;
  cycle();
  if ((current->gb.get_tcycle() & 3) == 0) return;
  cycle();
}

//-----------------------------------------------------------------------------
