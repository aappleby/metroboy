#pragma once
#include "Assembler.h"

extern const char* source_header;

struct Linker {
  Linker(uint8_t* code_buf);

  template<typename ... Args>
  void assemble(const char* format, Args ... args);

  template<>
  void assemble(const char* format);

  void loc(int x);

  //----------

  void nops(int n);
  void lcd_on_sprites();
  void scx(int x);
  void lcd_off_unsafe();

  void jr_bookmark();

  void test_finish();

  void load_sprite(int i, int y, int x, int p, int f);
  void assemble_jr();
  void patch_jr();
  void test_finish_stat(int result);

  uint8_t* code_buf;
  uint8_t* cursor;
  char source_buf[1024];
  std::vector<uint8_t*> bookmarks;

  Assembler a;

  FILE* out_file;
};

void link(const char* source);