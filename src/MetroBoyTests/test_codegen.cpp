#include "metrolib/gameboy/Assembler.h"
#include "MetroBoyLib/MetroBoy.h"

#include <string>

// no sprites, no scroll - last render is 61
// sprite at 0, no scroll - last render is 63

// last cycle in render phase
int spritex_vs_scx[17][9] = {
  { 63, 64, 64, 64, 64, 65, 65, 65, 63 },
  { 63, 63, 63, 63, 63, 63, 64, 65, 63 },
  { 63, 63, 63, 63, 63, 63, 65, 65, 63 },
  { 63, 63, 63, 63, 63, 65, 65, 65, 63 },
  { 62, 62, 63, 63, 64, 64, 64, 64, 62 },
  { 62, 62, 63, 64, 64, 64, 64, 64, 62 },
  { 62, 62, 64, 64, 64, 64, 64, 64, 62 },
  { 62, 64, 64, 64, 64, 64, 64, 64, 62 },

  { 63, 63, 63, 63, 63, 63, 64, 64, 63 },
  { 63, 63, 63, 63, 63, 63, 64, 65, 63 },
  { 63, 63, 63, 63, 63, 63, 65, 65, 63 },
  { 63, 63, 63, 63, 63, 65, 65, 65, 63 },
  { 62, 62, 63, 63, 64, 64, 64, 64, 62 },
  { 62, 62, 63, 64, 64, 64, 64, 64, 62 },
  { 62, 62, 64, 64, 64, 64, 64, 64, 62 },
  { 62, 64, 64, 64, 64, 64, 64, 64, 62 },

  { 63, 63, 63, 63, 63, 63, 64, 64, 63 },
};

void test_preloaded(MetroBoy& gameboy, const std::string& /*name*/) {
  uint8_t result = 0xFF;
  int phase = 0;
  const int timeout = 100000 * 8;  // bits_ram_en needs lots of mcycles
  for (; phase < timeout; phase++) {
    gameboy.next_phase();
    result = gameboy.vram.ram[0];
    if (result) break;
  }

  if (phase == timeout) {
    printf("?");
  }
  else if (result == 0x55) {
    printf(".");
  }
  else {
    printf("X");
  }
}

void emit_test_scx_sprite(Assembler& l, int spriteX, int spriteY, int scroll, int delay, int result) {
  l.begin_block(0x150);
  l.lcd_off_unsafe();
  l.clear_oam();
  l.load_sprite(0, spriteY, spriteX, 0, 0);
  l.scx(scroll);
  l.lcd_on_sprites();
  l.nops(112);
  l.nops(delay);
  l.test_finish_stat(result);
}

/*
void test_scx_sprite() {
  Assembler* l = new Assembler();
  Gameboy* g = new Gameboy();

  for (int spriteX = 0; spriteX < 17; spriteX++) {
    const int spriteY = 16;
    for (int scrollX = 0; scrollX < 9; scrollX++) {
      const int delay = spritex_vs_scx[spriteX][scrollX];
      const int result = 0x83;

      std::string name;
      sprintf(name, "sprite_vs_scx_%02d_%02d_%02d_%02d", spriteX, scrollX, delay, result);
      l->begin(name);

      emit_test_scx_sprite(*l, spriteX, spriteY, scrollX, delay, result);
      l->link();

      g->reset(0x100);
      test_preloaded(*g, name);
    }
    printf("\n");

    for (int scrollX = 0; scrollX < 9; scrollX++) {
      const int delay = spritex_vs_scx[spriteX][scrollX] + 1;
      const int result = 0x80;

      std::string name;
      sprintf(name, "sprite_vs_scx_%02d_%02d_%02d_%02d", spriteX, scrollX, delay, result);
      l->begin(name);

      emit_test_scx_sprite(*l, spriteX, spriteY, scrollX, delay, result);
      l->link();

      g->reset(0x100);
      test_preloaded(*g, name);
    }
    printf("\n");
  }
}

void test_codegen() {
  test_scx_sprite();
}
*/
