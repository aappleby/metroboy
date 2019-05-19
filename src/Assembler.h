#pragma once

struct Assembler {
  char* disassemble_one(const uint8_t* code, char* cursor);
  int disassemble(const uint8_t* code, uint16_t code_base, int code_cursor, int opcount, char* cursor);
  uint8_t* assemble(const char* source, uint8_t* code);

  void loc(int x);

  bool emit_source = true;

  std::string generated_source;
};

