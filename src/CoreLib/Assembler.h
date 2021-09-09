#pragma once

#include "CoreLib/Types.h"
#include "CoreLib/File.h"

#include <vector>
#include <map>
#include <string>

extern const char* source_header;

struct Assembler {
  Assembler() = default;

  static blob create_dummy_cart();

  template<typename ... Args>
  void assemble(const char* format, Args ... args);
  void disassemble_one(const uint8_t* code, Dumper& dump);

  void disassemble(
    const uint8_t* code, int code_size,
    int code_base,
    int pc,
    int opcount,
    Dumper& dump,
    bool collapse_nops = true);

  void assemble(const char* source);

  void disassemble(Dumper& dump);

  void begin(const std::string& new_name) {
    name = new_name;
    bookmarks.clear();
    block_map.clear();
  }

  blob link();
  void link_to(uint8_t* rom_buf);
  void write_source();

  //----------

  void nops(int n);
  void jr_bookmark();
  void assemble_jr();
  void patch_jr();

  void clear_oam();
  void lcd_off_unsafe();
  void lcd_on_sprites();
  void scx(int x);
  void load_sprite(int i, int y, int x, int p, int f);
  void test_finish();
  void test_finish_stat(int result);

  //----------

  std::string name;
  std::vector<size_t> bookmarks;
  std::map<std::string, uint16_t> label_map;
  std::map<uint16_t, blob> block_map;
  uint16_t block_addr = 0;
  blob* block_code = nullptr;

  void begin_label(std::string label);
  void begin_block(uint16_t addr);
  void emit(uint8_t x);
  void emit(uint8_t a, uint8_t b);
  void emit(uint8_t a, uint8_t b, uint8_t c);

  FILE* out_file = nullptr;
};

