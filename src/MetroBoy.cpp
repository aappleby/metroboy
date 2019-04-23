#include "MetroBoy.h"
#include "Audio.h"
#include "Assembler.h"

uint8_t rom_buf[1024 * 1024];

//-----------------------------------------------------------------------------

MetroBoy::MetroBoy() {
  current_gameboy = new Gameboy();
  cycles = 0;
  memset(current_gameboy->framebuffer, 0, sizeof(current_gameboy->framebuffer));

  trace = true;
  memset(tracebuffer, 0, sizeof(tracebuffer));
}

//-----------------------------------------------------------------------------

void MetroBoy::load_rom(const char* filename) {
  FILE* rom_file = fopen(filename, "rb");
  fseek(rom_file, 0, SEEK_END);
  int rom_size = ftell(rom_file);
  fseek(rom_file, 0, SEEK_SET);
  fread(rom_buf, 1, rom_size, rom_file);
  fclose(rom_file);

  current_gameboy->reset(rom_size, 0x100);
}

void MetroBoy::load_dump() {
  FILE* dump_file = fopen("dump.MetroBoy", "rb");
  fread(current_gameboy, 1, sizeof(Gameboy), dump_file);
  fread(rom_buf, 1, 1024 * 1024, dump_file);
  fclose(dump_file);
}

void MetroBoy::save_dump() {
  FILE* dump_file = fopen("dump.MetroBoy", "wb");
  fwrite(current_gameboy, 1, sizeof(Gameboy), dump_file);
  fwrite(rom_buf, 1, 1024 * 1024, dump_file);
  fclose(dump_file);
}

//-----------------------------------------------------------------------------

void MetroBoy::reset(uint16_t new_pc) {
  current_gameboy->reset(new_pc);
}

//-----------------------------------------------------------------------------

void MetroBoy::run_fast(uint8_t buttons, int fast_cycles) {
  clear_frame_history();
  clear_line_history();
  clear_cycle_history();

  current_gameboy->set_buttons(~buttons);

  for (int cycle = 0; cycle < fast_cycles; cycle++) mcycle();
}

//-------------------------------------

void MetroBoy::run_vsync(uint8_t buttons) {
  clear_frame_history();
  clear_line_history();
  clear_cycle_history();

  current_gameboy->set_buttons(~buttons);

  while (!current_gameboy->is_frame_done()) {
    cycle();
  }

  audio_begin();

  for (int i = 0; i < 154 * 114; i++) {
    mcycle();
    audio_post(current_gameboy->get_audio_l(),
               current_gameboy->get_audio_r());
  }

  audio_end();
}

//-------------------------------------

void MetroBoy::run_to(uint16_t breakpoint) {
  clear_frame_history();
  clear_line_history();
  clear_cycle_history();

  while (1) {
    uint16_t pc = current_gameboy->get_pc();
    if (pc == breakpoint) break;
    cycle();
  }
}

//-------------------------------------

void MetroBoy::step_frame() {
  clear_line_history();
  clear_cycle_history();

  history_frame.push_back(current_gameboy);

  current_gameboy = new Gameboy();
  memcpy(current_gameboy, history_frame.back(), sizeof(Gameboy));

  do {
    mcycle();
  } while (!current_gameboy->is_frame_done());
}

void MetroBoy::step_line() {
  clear_cycle_history();

  history_line.push_back(current_gameboy);

  current_gameboy = new Gameboy();
  memcpy(current_gameboy, history_line.back(), sizeof(Gameboy));

  int line = current_gameboy->get_line();
  do {
    mcycle();
  } while (current_gameboy->get_line() == line);
}

void MetroBoy::step_cycle() {
  history_cycle.push_back(current_gameboy);

  current_gameboy = new Gameboy();
  memcpy(current_gameboy, history_cycle.back(), sizeof(Gameboy));
  cycle();
}

void MetroBoy::step_over() {
  history_cycle.push_back(current_gameboy);

  current_gameboy = new Gameboy();
  memcpy(current_gameboy, history_cycle.back(), sizeof(Gameboy));

  int op = current_gameboy->get_op();
  int op_size = op_sizes[op];

  int next_pc = current_gameboy->get_pc() + op_size;

  int i = 0;
  for (; i < 1000000; i++) {
    if (current_gameboy->get_pc() == next_pc) {
      // step succeeded
      return;
    }
    cycle();
  }

  // step failed
  unstep_cycle();
  return;
}

//-----------------------------------------------------------------------------

void MetroBoy::unstep_frame() {
  if (history_frame.empty()) return;

  delete current_gameboy;

  current_gameboy = history_frame.back();
  history_frame.pop_back();
}

void MetroBoy::unstep_line() {
  if (history_line.empty()) return;

  delete current_gameboy;

  current_gameboy = history_line.back();
  history_line.pop_back();
}

void MetroBoy::unstep_cycle() {
  if (history_cycle.empty()) return;

  delete current_gameboy;

  current_gameboy = history_cycle.back();
  history_cycle.pop_back();
}

//-----------------------------------------------------------------------------

void MetroBoy::clear_frame_history() {
  for (auto g : history_frame) delete g;
  history_frame.clear();
}

void MetroBoy::clear_line_history() {
  for (auto g : history_line) delete g;
  history_line.clear();
}

void MetroBoy::clear_cycle_history() {
  for (auto g : history_cycle) delete g;
  history_cycle.clear();
}

//-----------------------------------------------------------------------------

void MetroBoy::cycle() {
  current_gameboy->tick();
  current_gameboy->tock();

  if (trace) {
    tracebuffer[current_gameboy->ppu.line2 * 456 + current_gameboy->ppu.counter2] = current_gameboy->trace();
  }

  if (current_gameboy->is_frame_start()) {
    current_gameboy->fbX = 0;
    current_gameboy->fbY = 0;
  }

  if (current_gameboy->get_pix_oe()) {
    current_gameboy->framebuffer[current_gameboy->fbX + (current_gameboy->fbY * 160)] = current_gameboy->get_pix_out();
    current_gameboy->fbX++;
    if (current_gameboy->fbX == 160) {
      current_gameboy->fbX = 0;
      current_gameboy->fbY++;
      if (current_gameboy->fbY == 144) {
        current_gameboy->fbY = 0;
      }
    }
  }

  current_gameboy->check_sentinel();
  cycles++;
}

void MetroBoy::mcycle() {
  cycle();
  if ((current_gameboy->get_tcycle() & 3) == 0) return;
  cycle();
  if ((current_gameboy->get_tcycle() & 3) == 0) return;
  cycle();
  if ((current_gameboy->get_tcycle() & 3) == 0) return;
  cycle();
}

//-----------------------------------------------------------------------------
