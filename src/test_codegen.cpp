#include "Platform.h"

#include "Assembler.h"
#include "Gameboy.h"

// x y scroll delay result delay result
// 0 0   0       62   0x83    63   0x80
// 0 0   1       64   0x83    65   0x80
// 0 0   2       64   0x83    65   0x80
// 0 0   3       64   0x83    65   0x80
// 0 0   4       64   0x83    65   0x80
// 0 0   5       64   0x83    65   0x80
// 0 0   6       64   0x83    65   0x80
// 0 0   7       64   0x83    65   0x80

void test_preloaded(Gameboy& gameboy, const char* name) {
  uint8_t result = 0xFF;
  int i = 0;
  const int ticks = 100000;  // bits_ram_en needs lots of tcycles
  for (; i < ticks; i++) {
    gameboy.tick();
    gameboy.tock();
    result = gameboy.vram.ram[0];
    if (result) break;
  }

  if (i == ticks) {
    printf("%-50s ", name);
    printf("? TIMEOUT @ %d\n", i);
  }
  else if (result == 0x55) {
    //printf(".");
    printf("%-50s ", name);
    printf("  0x%02x PASS @ %d\n", result, i);
  }
  else {
    printf("%-50s ", name);
    printf("X 0x%02x FAIL @ %d\n", result, i);
  }
}

void emit_test_scx_sprite(Assembler& l, int spriteX, int spriteY, int scroll, int delay, int result) {
  l.begin_block(0x150);
  l.lcd_off_unsafe();
  l.load_sprite(0, spriteY, spriteX, 0, 0);
  l.scx(scroll);
  
  l.lcd_on_sprites();
  l.nops(112);

  l.nops(delay);
  l.test_finish_stat(result);
}

void test_scx_sprite() {
  Assembler l;

  for (int scroll = 0; scroll < 8; scroll++) {
    const int spriteX = 0;
    const int spriteY = 32;
    const int delay = scroll == 0 ? 62 : 64;
    const int result = 0x83;

    l.begin(std::string("codegen/ppu_sprite0_scx") + std::to_string(scroll) + std::string("_a"));
    emit_test_scx_sprite(l, spriteX, spriteY, scroll, delay, result);
    l.write_source();
  }

  for (int scroll = 0; scroll < 8; scroll++) {
    const int spriteX = 0;
    const int spriteY = 32;
    const int delay = scroll == 0 ? 64 : 65;
    const int result = 0x80;

    l.begin(std::string("codegen/ppu_sprite0_scx") + std::to_string(scroll) + std::string("_b"));
    emit_test_scx_sprite(l, spriteX, spriteY, scroll, delay, result);
    l.write_source();
  }
}

void test_codegen() {
  test_scx_sprite();
}