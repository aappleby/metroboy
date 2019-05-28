#pragma once

extern const char* source_header;

typedef std::vector<uint8_t> blob;


struct Assembler {
  template<typename ... Args>
  void assemble(const char* format, Args ... args);
  void disassemble_one(const uint8_t* code, std::string& out);
  void disassemble(const uint8_t* code, size_t code_size, uint16_t code_base, int opcount, std::string& out, bool collapse_nops = true);
  void assemble(const char* source);

  void disassemble(std::string& out);

  void begin(const std::string& new_name) {
    name = new_name;
    bookmarks.clear();
    block_map.clear();
  }

  void link();
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
  std::map<string, uint16_t> label_map;
  std::map<uint16_t, blob> block_map;
  uint16_t block_addr;
  blob* block_code;

  void begin_label(std::string label) {
    label_map[label] = block_addr + (uint16_t)block_code->size();
  }

  void begin_block(uint16_t addr) {
    blob& code = block_map[addr];
    assert(code.empty());
    block_addr = addr;
    block_code = &code;
  }

  void emit(uint8_t x) {
    block_code->push_back(x);
  }

  void emit(uint8_t a, uint8_t b) {
    block_code->push_back(a);
    block_code->push_back(b);
  }

  void emit(uint8_t a, uint8_t b, uint8_t c) {
    block_code->push_back(a);
    block_code->push_back(b);
    block_code->push_back(c);
  }

  FILE* out_file;
};

