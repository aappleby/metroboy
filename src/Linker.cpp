#include "Platform.h"
#include "Linker.h"

#include "Assembler.h"

#include <memory>
#include <iostream>
#include <string>
#include <cstdio>

const char* source_header = R"(
.gbheader
name "microtest"
nintendologo
.endgb
.memorymap
defaultslot 0
slot 0 start $0000 size $4000
.endme
.rombanksize $4000
.rombanks 2
.bank 0 slot 0

.macro nops ARGS count
.repeat count
nop
.endr
.endm

)";

//-----------------------------------------------------------------------------

Linker::Linker(uint8_t* code_buf) {
  this->code_buf = code_buf;
  cursor = code_buf;
}

//-----------------------------------------------------------------------------

template<typename ... Args>
void Linker::assemble(const char* format, Args ... args)
{
  snprintf(source_buf, 1024, format, args ...);

  //printf("%s\n", source_buf);
  cursor = a.assemble(source_buf, cursor);
}

template<>
void Linker::assemble(const char* format)
{
  //printf("%s\n", format);
  cursor = a.assemble(format, cursor);
}

//-----------------------------------------------------------------------------

void Linker::loc(int x) {
  cursor = code_buf + x;
  a.loc(x);
}

void Linker::nops(int n) {
  for (int i = 0; i < n; i++) *cursor++ = 0;
}

void Linker::lcd_on_sprites() {
  assemble("ld a, $93; ldh ($40), a");
}

void Linker::scx(int x) {
  assemble("ld a, $%02x; ldh ($43), a;", x);
}

void Linker::lcd_off_unsafe() {
  assemble("ld a, $00; ldh ($40), a");
}


void Linker::jr_bookmark() {
  uint8_t* dst = bookmarks.back();
  bookmarks.pop_back();

  assemble("jr $00");
  cursor[-1] = uint8_t(dst - cursor);
}

void Linker::test_finish() {
  bookmarks.push_back(cursor);
  assemble("ld ($8000), a");
  jr_bookmark();
}

void Linker::load_sprite(int i, int y, int x, int p, int f) {
  assemble(R"(
    ld hl, $%04x;
    ld a, $%02x; ld (hl+), a;
    ld a, $%02x; ld (hl+), a;
    ld a, $%02x; ld (hl+), a;
    ld a, $%02x; ld (hl+), a;)",
    0xFE00 + 4 * i, y, x, p, f);
}
  
void Linker::assemble_jr() {
  bookmarks.push_back(cursor);
  assemble("jr z, $00");
}

void Linker::patch_jr() {
  uint8_t* patch = bookmarks.back();
  bookmarks.pop_back();
  patch[1] = uint8_t(cursor - patch - 2);
}

void Linker::test_finish_stat(int result) {
  assemble("ldh a, ($41)");
  assemble("cp $%02x", result);
  assemble_jr();
  assemble("ld a, $FF");
  test_finish();
  patch_jr();
  assemble("ld a, $55");
  test_finish();
}


/*
Linker l(rom_buf);

const int sx = 0;
const int scroll = 0;
const int delay = 61;
const int result = 0x83;

l.loc(0x100);
l.assemble("nop");
l.assemble("jp $0150");

l.loc(0x150);
l.lcd_off_unsafe();
l.load_sprite(0, 0, sx, 0, 0);
l.scx(scroll);
l.lcd_on_sprites();
l.nops(114 - 2);
l.nops(delay);
l.test_finish_stat(result);

char source_buf[1024];
disassemble(rom_buf, 0, 0x150, 30, source_buf);
printf("%s\n", source_buf);

metroboy.reset(0x0100);
runmode = RUN_FAST;
*/
